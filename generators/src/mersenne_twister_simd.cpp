#include "mersenne_twister_simd.hpp"

#include <iostream>
#include <limits>

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
#endif
