#include "mersenne_twister_simd.hpp"

#include <bit>
#include <iostream>
#include <limits>
#include <vector>

static std::vector<uint8_t> AES_SBOX = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, 0xca, 0x82, 0xc9,
    0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f,
    0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07,
    0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3,
    0x29, 0xe3, 0x2f, 0x84, 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58,
    0xcf, 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, 0x51, 0xa3,
    0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, 0xcd, 0x0c, 0x13, 0xec, 0x5f,
    0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
    0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac,
    0x62, 0x91, 0x95, 0xe4, 0x79, 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a,
    0xae, 0x08, 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, 0x70,
    0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, 0xe1, 0xf8, 0x98, 0x11,
    0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42,
    0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};

unsigned int cpu_supports() {
    unsigned int eax, ebx, ecx, edx;
    __get_cpuid(1, &eax, &ebx, &ecx, &edx);
    if (!(ecx & (1 << 27)))
        return false;
    if (!(ecx & (1 << 28)))
        return false;
    __cpuid_count(7, 0, eax, ebx, ecx, edx);
    return ebx;
}

bool cpu_supports_avx2() {
    return (cpu_supports() & (1 << 5));
}

bool cpu_supports_avx512() {
    return (cpu_supports() & (1 << 16));
}

#if defined(__AVX__) && defined(__AVX2__)
// TODO: Move in utils
void print_m256i(const __m256i &vec) {
    uint32_t values[8];
    _mm256_storeu_si256((__m256i *)values, vec);

    for (int i = 0; i < 8; i++) {
        std::cout << values[i] << " " << std::endl;
    }
    std::cout << std::endl;
}

MersenneTwister32AVX2::MersenneTwister32AVX2(const uint32_t &seed) {
    mt[0] = seed;
    for (size_t i = 1; i < N; i++) {
        mt[i] = (F * (mt[i - 1] ^ (mt[i - 1] >> (W - 2))) + i);
    }
    index_state = N;
}

__m256i MersenneTwister32AVX2::tempering_simd(__m256i y) const {
    y = _mm256_xor_si256(y, _mm256_and_si256(_mm256_srli_epi32(y, U), _mm256_set1_epi32(D)));
    y = _mm256_xor_si256(y, _mm256_and_si256(_mm256_slli_epi32(y, S), _mm256_set1_epi32(B)));
    y = _mm256_xor_si256(y, _mm256_and_si256(_mm256_slli_epi32(y, T), _mm256_set1_epi32(C)));
    y = _mm256_xor_si256(y, _mm256_srli_epi32(y, L));
    return y;
}

uint32_t MersenneTwister32AVX2::tempering_scalar(uint32_t y) const {
    y ^= (y >> U) & D;
    y ^= (y << S) & B;
    y ^= (y << T) & C;
    y ^= (y >> L);
    return y;
}

void MersenneTwister32AVX2::twist() {
    const __m256i upper_mask = _mm256_set1_epi32(UPPER_MASK);
    const __m256i lower_mask = _mm256_set1_epi32(LOWER_MASK);
    const __m256i one = _mm256_set1_epi32(1);

    size_t i = 0;
    size_t j = M;
    for (; i + 8 <= N && j + 8 <= N; i += 8, j += 8) {
        __m256i mt_i = _mm256_load_si256((__m256i *)&mt[i]);
        __m256i mt_i1 = _mm256_loadu_si256((__m256i *)&mt[i + 1]);
        __m256i x = _mm256_or_si256(_mm256_and_si256(mt_i, upper_mask), _mm256_and_si256(mt_i1, lower_mask));

        __m256i xA = _mm256_srli_epi32(x, 1);

        __mmask8 mask = _mm256_test_epi32_mask(x, one);
        xA = _mm256_mask_xor_epi32(xA, mask, xA, _mm256_set1_epi32(A));

        __m256i mt_im = _mm256_loadu_si256((__m256i *)&mt[j]);
        __m256i res = _mm256_xor_si256(mt_im, xA);

        _mm256_storeu_si256((__m256i *)&mt[i], res);
    }
    for (; i + 8 <= N && j < N; j++, i++) {
        size_t x = (mt[i] & UPPER_MASK) | (mt[(i + 1) % N] & LOWER_MASK);
        mt[i] = mt[j] ^ (x >> 1) ^ ((x & 1) ? A : 0);
    }
    j = 0;
    for (; i + 8 <= N && j + 8 <= N; i += 8, j += 8) {
        __m256i mt_i = _mm256_loadu_si256((__m256i *)&mt[i]);
        __m256i mt_i1 = _mm256_loadu_si256((__m256i *)&mt[i + 1]);
        __m256i x = _mm256_or_si256(_mm256_and_si256(mt_i, upper_mask), _mm256_and_si256(mt_i1, lower_mask));

        __m256i xA = _mm256_srli_epi32(x, 1);

        __mmask8 mask = _mm256_test_epi32_mask(x, one);
        xA = _mm256_mask_xor_epi32(xA, mask, xA, _mm256_set1_epi32(A));

        __m256i mt_im = _mm256_loadu_si256((__m256i *)&mt[j]);
        __m256i res = _mm256_xor_si256(mt_im, xA);

        _mm256_storeu_si256((__m256i *)&mt[i], res);
    }

    for (; i < N; i++) {
        size_t x = (mt[i] & UPPER_MASK) | (mt[(i + 1) % N] & LOWER_MASK);
        mt[i] = mt[(i + M) % N] ^ (x >> 1) ^ ((x & 1) ? A : 0);
    }
    index_state = 0;
}

uint32_t MersenneTwister32AVX2::operator()() {
    if (index_state >= N) {
        twist();
    }
    return tempering_scalar(mt[index_state++]);
}

void MersenneTwister32AVX2::generate_bulk(uint32_t *output, size_t len) {
    size_t j = 0;
    size_t i = 0;
    for (i = 0, j = 0; i + 8 <= len; i += 8, j += 8) {
        if (index_state >= N) {
            twist();
            j = 0;
        }
        __m256i data = _mm256_load_si256((__m256i *)&mt[j]);
        _mm256_storeu_si256((__m256i *)&output[i], tempering_simd(data));
        index_state += 8;
    }
    for (; i < len; i++, j++) {
        if (index_state >= N) {
            twist();
            j = 0;
        }
        output[i] = tempering_scalar(mt[j]);
        index_state += 1;
    }
}

uint32_t MersenneTwister32AVX2::min() const {
    return 0u;
}

uint32_t MersenneTwister32AVX2::max() const {
    return std::numeric_limits<uint32_t>::max();
}

void MersenneTwister32AVX2::seed(const uint32_t new_seed) {
    *this = MersenneTwister32AVX2(new_seed);
}

void MersenneTwister32AVX2::discard(const std::uint64_t z) {
    for (size_t i = 0; i < z; ++i) {
        this->operator()();
    }
}

MersenneTwister32SboxRotr31AVX2::MersenneTwister32SboxRotr31AVX2(const uint32_t &seed) {
    mt[0] = seed;
    for (size_t i = 1; i < N; i++) {
        mt[i] = (F * (mt[i - 1] ^ (mt[i - 1] >> (W - 2))) + i);
    }
    index_state = N;
}

__m256i MersenneTwister32SboxRotr31AVX2::tempering_simd(__m256i y) const {
    constexpr size_t bytes = sizeof(uint32_t);
    y = _mm256_xor_si256(y, _mm256_and_si256(_mm256_srli_epi32(y, U), _mm256_set1_epi32(D)));
    y = _mm256_xor_si256(y, _mm256_and_si256(_mm256_slli_epi32(y, S), _mm256_set1_epi32(B)));
    y = _mm256_xor_si256(y, _mm256_and_si256(_mm256_slli_epi32(y, T), _mm256_set1_epi32(C)));
    y = _mm256_xor_si256(y, _mm256_srli_epi32(y, L));

    // Apply AES S-box transformation
    alignas(32) uint8_t temp[32];
    alignas(32) uint8_t out[32];
    _mm256_store_si256((__m256i *)temp, y);
    for (size_t i = 0; i < 32; i += 4) {
        for (size_t j = 0; j < 4; ++j) {
            out[i + j] = AES_SBOX[temp[i + 3 - j]];
        }
    }
    y = _mm256_load_si256((__m256i *)out);

    // Apply rotation
    y = _mm256_xor_epi32(_mm256_srli_epi32(y, shift), _mm256_slli_epi32(y, (sizeof(uint32_t) * 8 - shift)));
    return y;
}

uint32_t MersenneTwister32SboxRotr31AVX2::tempering_scalar(uint32_t y) const {
    constexpr uint32_t bytes = sizeof(uint32_t);
    y ^= (y >> U) & D;
    y ^= (y << S) & B;
    y ^= (y << T) & C;
    y ^= (y >> L);
    uint32_t result = 0u;
    for (size_t i = 0; i < bytes; ++i) {
        result |= (AES_SBOX[(y >> (i * 8)) & 0xFF]) << ((bytes - i - 1) * 8);
    }
    result = std::rotr(result, shift);
    return result;
}

void MersenneTwister32SboxRotr31AVX2::twist() {
    const __m256i upper_mask = _mm256_set1_epi32(UPPER_MASK);
    const __m256i lower_mask = _mm256_set1_epi32(LOWER_MASK);
    const __m256i one = _mm256_set1_epi32(1);

    size_t i = 0;
    size_t j = M;
    for (; i + 8 <= N && j + 8 <= N; i += 8, j += 8) {
        __m256i mt_i = _mm256_load_si256((__m256i *)&mt[i]);
        __m256i mt_i1 = _mm256_loadu_si256((__m256i *)&mt[i + 1]);
        __m256i x = _mm256_or_si256(_mm256_and_si256(mt_i, upper_mask), _mm256_and_si256(mt_i1, lower_mask));

        __m256i xA = _mm256_srli_epi32(x, 1);

        __mmask8 mask = _mm256_test_epi32_mask(x, one);
        xA = _mm256_mask_xor_epi32(xA, mask, xA, _mm256_set1_epi32(A));

        __m256i mt_im = _mm256_loadu_si256((__m256i *)&mt[j]);
        __m256i res = _mm256_xor_si256(mt_im, xA);

        _mm256_storeu_si256((__m256i *)&mt[i], res);
    }
    for (; i + 8 <= N && j < N; j++, i++) {
        size_t x = (mt[i] & UPPER_MASK) | (mt[(i + 1) % N] & LOWER_MASK);
        mt[i] = mt[j] ^ (x >> 1) ^ ((x & 1) ? A : 0);
    }
    j = 0;
    for (; i + 8 <= N && j + 8 <= N; i += 8, j += 8) {
        __m256i mt_i = _mm256_loadu_si256((__m256i *)&mt[i]);
        __m256i mt_i1 = _mm256_loadu_si256((__m256i *)&mt[i + 1]);
        __m256i x = _mm256_or_si256(_mm256_and_si256(mt_i, upper_mask), _mm256_and_si256(mt_i1, lower_mask));

        __m256i xA = _mm256_srli_epi32(x, 1);

        __mmask8 mask = _mm256_test_epi32_mask(x, one);
        xA = _mm256_mask_xor_epi32(xA, mask, xA, _mm256_set1_epi32(A));

        __m256i mt_im = _mm256_loadu_si256((__m256i *)&mt[j]);
        __m256i res = _mm256_xor_si256(mt_im, xA);

        _mm256_storeu_si256((__m256i *)&mt[i], res);
    }

    for (; i < N; i++) {
        size_t x = (mt[i] & UPPER_MASK) | (mt[(i + 1) % N] & LOWER_MASK);
        mt[i] = mt[(i + M) % N] ^ (x >> 1) ^ ((x & 1) ? A : 0);
    }
    index_state = 0;
}

uint32_t MersenneTwister32SboxRotr31AVX2::operator()() {
    if (index_state >= N) {
        twist();
    }
    return tempering_scalar(mt[index_state++]);
}

void MersenneTwister32SboxRotr31AVX2::generate_bulk(uint32_t *output, size_t len) {
    size_t j = 0;
    size_t i = 0;
    for (i = 0, j = 0; i + 8 <= len; i += 8, j += 8) {
        if (index_state >= N) {
            twist();
            j = 0;
        }
        __m256i data = _mm256_load_si256((__m256i *)&mt[j]);
        _mm256_storeu_si256((__m256i *)&output[i], tempering_simd(data));
        index_state += 8;
    }
    for (; i < len; i++, j++) {
        if (index_state >= N) {
            twist();
            j = 0;
        }
        output[i] = tempering_scalar(mt[j]);
        index_state += 1;
    }
}

uint32_t MersenneTwister32SboxRotr31AVX2::min() const {
    return 0u;
}

uint32_t MersenneTwister32SboxRotr31AVX2::max() const {
    return std::numeric_limits<uint32_t>::max();
}

void MersenneTwister32SboxRotr31AVX2::seed(const uint32_t new_seed) {
    *this = MersenneTwister32SboxRotr31AVX2(new_seed);
}

void MersenneTwister32SboxRotr31AVX2::discard(const std::uint64_t z) {
    for (size_t i = 0; i < z; ++i) {
        this->operator()();
    }
}

#endif

#ifdef __AVX512F__
// TODO: Move in utils
void print_m512(const __m512i &vec) {
    uint32_t values[16];
    _mm512_store_si512((__m512i *)values, vec);

    for (int i = 0; i < 16; i++) {
        std::cout << values[i] << " " << std::endl;
    }
    std::cout << std::endl;
}

MersenneTwister32AVX512::MersenneTwister32AVX512(const uint32_t &seed) {
    mt[0] = seed;
    for (size_t i = 1; i < N; i++) {
        mt[i] = (F * (mt[i - 1] ^ (mt[i - 1] >> (W - 2))) + i);
    }
    index_state = N;
}

__m512i MersenneTwister32AVX512::tempering_simd(__m512i y) const {
    y = _mm512_xor_si512(y, _mm512_and_si512(_mm512_srli_epi32(y, U), _mm512_set1_epi32(D)));
    y = _mm512_xor_si512(y, _mm512_and_si512(_mm512_slli_epi32(y, S), _mm512_set1_epi32(B)));
    y = _mm512_xor_si512(y, _mm512_and_si512(_mm512_slli_epi32(y, T), _mm512_set1_epi32(C)));
    y = _mm512_xor_si512(y, _mm512_srli_epi32(y, L));
    return y;
}

uint32_t MersenneTwister32AVX512::tempering_scalar(uint32_t y) const {
    y ^= (y >> U) & D;
    y ^= (y << S) & B;
    y ^= (y << T) & C;
    y ^= (y >> L);
    return y;
}

void MersenneTwister32AVX512::twist() {
    const __m512i upper_mask = _mm512_set1_epi32(UPPER_MASK);
    const __m512i lower_mask = _mm512_set1_epi32(LOWER_MASK);
    const __m512i one = _mm512_set1_epi32(1);

    size_t i = 0;
    size_t j = M;
    for (; i + 16 <= N && j + 16 <= N; i += 16, j += 16) {
        __m512i mt_i = _mm512_load_si512((__m512i *)&mt[i]);
        __m512i mt_i1 = _mm512_loadu_si512((__m512i *)&mt[i + 1]);
        __m512i x = _mm512_or_si512(_mm512_and_si512(mt_i, upper_mask), _mm512_and_si512(mt_i1, lower_mask));

        __m512i xA = _mm512_srli_epi32(x, 1);

        __mmask16 mask = _mm512_test_epi32_mask(x, one);
        xA = _mm512_mask_xor_epi32(xA, mask, xA, _mm512_set1_epi32(A));

        __m512i mt_im = _mm512_loadu_si512((__m512i *)&mt[j]);
        __m512i res = _mm512_xor_si512(mt_im, xA);

        _mm512_storeu_si512((__m512i *)&mt[i], res);
    }
    for (; i + 16 <= N && j < N; j++, i++) {
        size_t x = (mt[i] & UPPER_MASK) | (mt[(i + 1) % N] & LOWER_MASK);
        mt[i] = mt[j] ^ (x >> 1) ^ ((x & 1) ? A : 0);
    }
    j = 0;
    for (; i + 16 <= N && j + 16 <= N; i += 16, j += 16) {
        __m512i mt_i = _mm512_loadu_si512((__m512i *)&mt[i]);
        __m512i mt_i1 = _mm512_loadu_si512((__m512i *)&mt[i + 1]);
        __m512i x = _mm512_or_si512(_mm512_and_si512(mt_i, upper_mask), _mm512_and_si512(mt_i1, lower_mask));

        __m512i xA = _mm512_srli_epi32(x, 1);

        __mmask16 mask = _mm512_test_epi32_mask(x, one);
        xA = _mm512_mask_xor_epi32(xA, mask, xA, _mm512_set1_epi32(A));

        __m512i mt_im = _mm512_loadu_si512((__m512i *)&mt[j]);
        __m512i res = _mm512_xor_si512(mt_im, xA);

        _mm512_storeu_si512((__m512i *)&mt[i], res);
    }

    for (; i < N; i++) {
        size_t x = (mt[i] & UPPER_MASK) | (mt[(i + 1) % N] & LOWER_MASK);
        mt[i] = mt[(i + M) % N] ^ (x >> 1) ^ ((x & 1) ? A : 0);
    }
    index_state = 0;
}

uint32_t MersenneTwister32AVX512::operator()() {
    if (index_state >= N) {
        twist();
    }
    return tempering_scalar(mt[index_state++]);
}

void MersenneTwister32AVX512::generate_bulk(uint32_t *output, size_t len) {
    size_t j = 0;
    size_t i = 0;
    for (i = 0, j = 0; i + 16 <= len; i += 16, j += 16) {
        if (index_state >= N) {
            twist();
            j = 0;
        }
        __m512i data = _mm512_load_si512((__m512i *)&mt[j]);
        _mm512_storeu_si512((__m512i *)&output[i], tempering_simd(data));
        index_state += 16;
    }
    for (; i < len; i++, j++) {
        if (index_state >= N) {
            twist();
            j = 0;
        }
        output[i] = tempering_scalar(mt[j]);
        index_state += 1;
    }
}

uint32_t MersenneTwister32AVX512::min() const {
    return 0u;
}

uint32_t MersenneTwister32AVX512::max() const {
    return std::numeric_limits<uint32_t>::max();
}

void MersenneTwister32AVX512::seed(const uint32_t new_seed) {
    *this = MersenneTwister32AVX512(new_seed);
}

void MersenneTwister32AVX512::discard(const std::uint64_t z) {
    for (size_t i = 0; i < z; ++i) {
        this->operator()();
    }
}

#endif
