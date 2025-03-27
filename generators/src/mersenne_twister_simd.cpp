#include "mersenne_twister_simd.hpp"

#include <iostream>
#include <limits>

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
