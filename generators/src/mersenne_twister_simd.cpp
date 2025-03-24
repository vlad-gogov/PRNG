#include "mersenne_twister_simd.hpp"

#include <limits>

MersenneTwister32AVX2::MersenneTwister32AVX2(const uint32_t &seed) : index(N) {
    mt[0] = seed;
    for (size_t i = 1; i < N; ++i) {
        mt[i] = 0x6C078965 * (mt[i - 1] ^ (mt[i - 1] >> 30)) + i;
    }
}

// Векторизованное преобразование tempering
__m256i MersenneTwister32AVX2::temper(__m256i y) const {
    y = _mm256_xor_si256(y, _mm256_srli_epi32(y, 11));

    __m256i s = _mm256_slli_epi32(y, 7);
    s = _mm256_and_si256(s, _mm256_set1_epi32(0x9D2C5680));
    y = _mm256_xor_si256(y, s);

    s = _mm256_slli_epi32(y, 15);
    s = _mm256_and_si256(s, _mm256_set1_epi32(0xEFC60000));
    y = _mm256_xor_si256(y, s);

    y = _mm256_xor_si256(y, _mm256_srli_epi32(y, 18));
    return y;
}

// Скалярное преобразование tempering
uint32_t MersenneTwister32AVX2::temper_scalar(uint32_t y) const {
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9D2C5680;
    y ^= (y << 15) & 0xEFC60000;
    y ^= (y >> 18);
    return y;
}

void MersenneTwister32AVX2::twist() {
    const __m256i upper_mask = _mm256_set1_epi32(UPPER_MASK);
    const __m256i lower_mask = _mm256_set1_epi32(LOWER_MASK);
    const __m256i matrix_a = _mm256_set1_epi32(MATRIX_A);
    const __m256i one = _mm256_set1_epi32(1);

    size_t i = 0;

    // Основная часть с проверкой границ
    for (; i + 8 <= N - M; i += 8) { // Гарантируем что i+M+7 < N
        __m256i mt_i = _mm256_load_si256((__m256i *)&mt[i]);
        __m256i mt_i1 = _mm256_loadu_si256((__m256i *)&mt[i + 1]);

        __m256i y = _mm256_or_si256(_mm256_and_si256(mt_i, upper_mask), _mm256_and_si256(mt_i1, lower_mask));

        __m256i y_shift = _mm256_srli_epi32(y, 1);
        __m256i mask = _mm256_and_si256(y, one);
        mask = _mm256_cmpeq_epi32(mask, one);
        mask = _mm256_and_si256(mask, matrix_a);

        // Используем безопасную невыровненную загрузку
        __m256i mt_im = _mm256_loadu_si256((__m256i *)&mt[i + M]);
        __m256i res = _mm256_xor_si256(mt_im, y_shift);
        res = _mm256_xor_si256(res, mask);

        _mm256_store_si256((__m256i *)&mt[i], res);
    }

    // Обработка оставшихся элементов скалярно
    for (; i < N; ++i) {
        uint32_t y = (mt[i] & UPPER_MASK) | (mt[(i + 1) % N] & LOWER_MASK);
        size_t j = (i + M) % N;
        mt[i] = mt[j] ^ (y >> 1) ^ ((y & 1) ? MATRIX_A : 0);
    }
}

uint32_t MersenneTwister32AVX2::operator()() {
    if (index >= N) {
        twist();
        index = 0;
    }
    return temper_scalar(mt[index++]);
}

void MersenneTwister32AVX2::generate_bulk(uint32_t *output, size_t len) {
    size_t generated = 0;
    while (generated < len) {
        if (index >= N) {
            twist();
            index = 0;
        }

        size_t to_gen = std::min(len - generated, N - index);
        size_t vec_len = to_gen & ~7ULL;
        size_t i = 0;

        // Векторизованная часть
        for (; i < vec_len; i += 8) {
            __m256i data = _mm256_load_si256((__m256i *)&mt[index + i]);
            data = temper(data);
            _mm256_storeu_si256((__m256i *)&output[generated + i], data);
        }

        // Скалярная часть
        for (; i < to_gen; ++i) {
            output[generated + i] = temper_scalar(mt[index + i]);
        }

        generated += to_gen;
        index += to_gen;
    }
}

// MersenneTwister32AVX2::MersenneTwister32AVX2(const uint32_t &seed) {
//     state[0] = _mm256_set1_epi32(seed);
//     for (int i = 1; i < N / 8; ++i) {
//         __m256i prev = state[i - 1];
//         __m256i multiplier = _mm256_set1_epi32(1812433253);
//         __m256i i_vec = _mm256_set1_epi32(i * 8);
//         state[i] = _mm256_xor_si256(_mm256_mullo_epi32(_mm256_xor_si256(prev, _mm256_srli_epi32(prev, 30)),
//         multiplier),
//                                     i_vec);
//     }
// }

// void MersenneTwister32AVX2::twist() {
//     __m256i mag = _mm256_set1_epi32(MATRIX_A);
//     __m256i mask_upper = _mm256_set1_epi32(UPPER_MASK);
//     __m256i mask_lower = _mm256_set1_epi32(LOWER_MASK);

//     for (int i = 0; i < N / 8; ++i) {
//         __m256i x = _mm256_or_si256(_mm256_and_si256(state[i], mask_upper),
//                                     _mm256_and_si256(state[(i + 1) % (N / 8)], mask_lower));
//         __m256i xA = _mm256_srli_epi32(x, 1);
//         __m256i even_mask = _mm256_and_si256(x, _mm256_set1_epi32(1));
//         __m256i magA = _mm256_and_si256(even_mask, mag);

//         state[i] = _mm256_xor_si256(_mm256_xor_si256(state[(i + M / 8) % (N / 8)], xA), magA);
//     }
//     index = 0;
// }

// uint32_t MersenneTwister32AVX2::operator()() {
//     if (index >= N) {
//         twist();
//         index = 0;
//     }

//     alignas(32) uint32_t vals[8];
//     _mm256_store_si256(reinterpret_cast<__m256i *>(vals), state[index / 8]);
//     return vals[index++ % 8];
// }

uint32_t MersenneTwister32AVX2::min() const {
    return 0u;
}

uint32_t MersenneTwister32AVX2::max() const {
    return std::numeric_limits<uint32_t>::max();
}