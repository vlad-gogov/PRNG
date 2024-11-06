#include <gtest/gtest.h>
#include "generators/well_generator.hpp"

TEST(Well, initialization) {
    WELL512a generator(12345U);
    ASSERT_NE(generator(), 0U);
}

TEST(Well, sequence_reproducibility) {
    WELL512a gen1(12345U);
    WELL512a gen2(12345U);
    
    for (int i = 0; i < 1000; ++i) {
        ASSERT_EQ(gen1(), gen2());
    }
}

TEST(Well, different_seeds) {
    WELL512a gen1(12345U);
    WELL512a gen2(54321U);
    
    bool different = false;
    for (int i = 0; i < 10; ++i) {
        if (gen1() != gen2()) {
            different = true;
            break;
        }
    }
    ASSERT_TRUE(different);
}

TEST(Well, discard) {
    WELL512a gen1(12345U);
    WELL512a gen2(12345U);
    
    gen1.discard(100);
    
    for (int i = 0; i < 100; ++i) {
        gen2();
    }
    
    ASSERT_EQ(gen1(), gen2());
}
