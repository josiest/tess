#include "gtest/gtest.h"
#include "tess.hpp"
#include "utility.hpp"

#include <cmath>    // min, max
#include <limits> // numeric_limits
#include <random> // std::random_device, std::uniform_real_distribution

using namespace tess;
using long_hex = hex<long>;

TEST(HexRoundTest, AllComponentsZero) {
    dhex const rhex{0., 0.};
    auto h = hex_round<int>(rhex);
    EXPECT_EQ(h, ihex::zero);
}

TEST(HexRoundTest, CenterZeroQCrossR) {
    std::random_device seed;
    std::uniform_real_distribution qdist(0.f, 0.5f);
    auto const q = qdist(seed);

    std::uniform_real_distribution rdist(0.f, 0.5f-q);
    fhex const rhex{q, rdist(seed)};

    auto h = hex_round<int>(rhex);
    EXPECT_EQ(h, ihex::zero);
}

TEST(HexRoundTest, CenterPositiveQCrossS) {
    std::random_device seed;

    std::uniform_int_distribution cdist(1, 50);
    auto const Q = cdist(seed);
    auto const S = cdist(seed);

    std::uniform_real_distribution qdist(0.l, .5l);
    auto const q = qdist(seed);

    std::uniform_real_distribution sdist(0.l, .5l-q);
    hex rhex{Q+q, -Q-S-q-sdist(seed)};

    hex const expected_val{Q, -Q-S};
    auto h = hex_round<int>(rhex);
    EXPECT_EQ(h, expected_val);
}

TEST(HexRoundTest, CenterNegativeRCrossS) {
    std::random_device seed;

    std::uniform_int_distribution cdist(-50, -1);
    auto const R = cdist(seed);
    auto const S = cdist(seed);

    std::uniform_real_distribution rdist(0., .5);
    double const r = rdist(seed);

    std::uniform_real_distribution sdist(0., .5-r);
    dhex const rhex{-R-S-r-sdist(seed), R+r};

    hex const expected_val{-R-S, R};
    auto h = hex_round<int>(rhex);
    EXPECT_EQ(h, expected_val);
}

TEST(HexRoundTest, LeftUpCenterQNegative) {
    std::random_device seed;
    std::uniform_int_distribution idist(-127, -51);
    std::uniform_real_distribution rdist(0.334f, 0.499f);

    int const Q = idist(seed);
    float const s = rdist(seed);
    fhex const rhex{Q-s, 0};

    hex const expected_val{Q, 0};
    auto h = hex_round<int>(rhex);
    EXPECT_EQ(h, expected_val);
}

TEST(HexRoundTest, ForwardLeftCenterRNegative) {
    std::random_device seed;
    std::uniform_int_distribution idist(-512, -128);
    std::uniform_real_distribution rdist(.3334l, .4999l);

    int const R = idist(seed);
    long double const s = rdist(seed);
    hex<long double> const rhex{0, R-s};

    hex const expected_val{0, R};
    auto h = hex_round<int>(rhex);
    EXPECT_EQ(h, expected_val);
}

TEST(HexRoundTest, ForwardDownQSNegativeRPositive) {
    std::random_device seed;
    std::uniform_int_distribution cdist{-2048l, -513l};
    std::uniform_real_distribution qdist{0., .5};

    auto const Q = cdist(seed);
    auto const S = cdist(seed);

    auto const q = qdist(seed);
    auto const rub = q-.5;
    auto const rlb = std::max(2.*q-1., .5*q-.5);

    std::uniform_real_distribution rdist{rlb, rub};
    auto const r = rdist(seed);
    dhex const rhex{Q+q, -Q-S+r};

    long_hex const expected_val{Q, -Q-S};
    auto h = hex_round<long>(rhex);
    EXPECT_EQ(h, expected_val);
}

TEST(HexRoundTest, RightDownQSPositiveRNegative) {
    std::random_device seed;
    std::uniform_int_distribution cdist{128l, 512l};
    std::uniform_real_distribution qdist{0.f, 0.5f};

    auto const Q = cdist(seed);
    auto const S = cdist(seed);

    auto const q = qdist(seed);
    auto const sub = q-0.5f;
    auto const slb = std::max(2.f*q-1.f, 0.5f*q-0.5f);

    std::uniform_real_distribution sdist{slb, sub};
    auto const s = sdist(seed);
    fhex const rhex{Q+q, -Q-S-q-s};

    long_hex const expected_val{Q, -Q-S};
    auto h = hex_round<long>(rhex);
    EXPECT_EQ(h, expected_val);
}

TEST(HexRoundTest, BackUpQPositiveRSNegative) {
    std::random_device seed;
    std::uniform_int_distribution cdist {-4096l, -1024l};
    std::uniform_real_distribution qdist{-.5l, 0.l};

    long const R = cdist(seed);
    long const S = cdist(seed);

    long double const q = qdist(seed);
    long double const rlb = q + 0.5l;
    long double const rub = std::min(2.l*q+1.l, 0.5l*q+0.5l);

    std::uniform_real_distribution rdist{rlb, rub};
    long double const r = rdist(seed);
    hex<long double> const rhex{-R-S+q, R+r};

    long_hex const expected_val{-R-S, R};
    auto h = hex_round<long>(rhex);
    EXPECT_EQ(h, expected_val);
}
