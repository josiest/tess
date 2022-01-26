#include "gtest/gtest.h"
#include "tess.hpp"
#include <limits>
#include <random>

using namespace tess;
using namespace std;

TEST(HexRoundTest, AllComponentsZero) {
    Hex<double> const rhex{0.0, 0.0};

    auto ihex = hex_round<int>(rhex);
    EXPECT_EQ(ihex, Hex<int>::zero);
}

TEST(HexRoundTest, CenterZeroQCrossR) {
    random_device seed;
    uniform_real_distribution<float> qdist(0.0, 0.5);
    float const q = qdist(seed);

    uniform_real_distribution<float> rdist(0.0, 0.5-q);
    Hex<float> const rhex{q, rdist(seed)};

    auto ihex = hex_round<int>(rhex);
    EXPECT_EQ(ihex, Hex<int>::zero);
}

TEST(HexRoundTest, CenterPositiveQCrossS) {
    random_device seed;

    uniform_int_distribution<int> cdist(1, 50);
    int const Q = cdist(seed);
    int const S = cdist(seed);

    uniform_real_distribution<long double> qdist(0.0, 0.5);
    long double const q = qdist(seed);

    uniform_real_distribution<long double> sdist(0.0, 0.5-q);
    Hex<long double> rhex{Q+q, -Q-S-q-sdist(seed)};

    Hex<int> const expected{Q, -Q-S};
    auto ihex = hex_round<int>(rhex);
    EXPECT_EQ(ihex, expected);
}

TEST(HexRoundTest, CenterNegativeRCrossS) {
    random_device seed;

    uniform_int_distribution<short> cdist(-50, -1);
    short const R = cdist(seed);
    short const S = cdist(seed);

    uniform_real_distribution<double> rdist(0.0, 0.5);
    double const r = rdist(seed);

    uniform_real_distribution<double> sdist(0.0, 0.5-r);
    Hex<double> const rhex{-R-S-r-sdist(seed), R+r};

    Hex<short> const expected{(short)(-R-S), R};
    auto ihex = hex_round<short>(rhex);
    EXPECT_EQ(ihex, expected) << "rhex: " << rhex << "\n";
}

TEST(HexRoundTest, LeftUpCenterQNegative) {
    random_device seed;
    uniform_int_distribution<short> idist(-127, -51);
    uniform_real_distribution<float> rdist(0.334, 0.499);

    short const Q = idist(seed);
    float const s = rdist(seed);
    Hex<float> const rhex{Q-s, 0};

    Hex<short> const expected{Q, 0};
    auto ihex = hex_round<short>(rhex);
    EXPECT_EQ(ihex, expected);
}

TEST(HexRoundTest, ForwardLeftCenterRNegative) {
    random_device seed;
    uniform_int_distribution<short> idist(-512, -128);
    uniform_real_distribution<long double> rdist(0.3334, 0.4999);

    short const R = idist(seed);
    long double const s = rdist(seed);
    Hex<long double> const rhex{0, R-s};

    Hex<short> const expected{0, R};
    auto ihex = hex_round<short>(rhex);
    EXPECT_EQ(ihex, expected);
}

TEST(HexRoundTest, ForwardDownQSNegativeRPositive) {
    random_device seed;
    uniform_int_distribution<long> cdist{-2048, -513};
    uniform_real_distribution<double> qdist{0, 0.5};

    long const Q = cdist(seed);
    long const S = cdist(seed);

    double const q = qdist(seed);
    double const rub = q-0.5;
    double const rlb = max(2*q-1, 0.5*q-0.5);

    uniform_real_distribution<double> rdist{rlb, rub};
    double const r = rdist(seed);
    Hex<double> const rhex{Q+q, -Q-S+r};

    Hex<long> const expected{Q, -Q-S};
    auto ihex = hex_round<long>(rhex);
    EXPECT_EQ(ihex, expected);
}

TEST(HexRoundTest, RightDownQSPositiveRNegative) {
    random_device seed;
    uniform_int_distribution<long> cdist{128, 512};
    uniform_real_distribution<float> qdist{0.0f, 0.5f};

    long const Q = cdist(seed);
    long const S = cdist(seed);

    float const q = qdist(seed);
    float const sub = q-0.5f;
    float const slb = max(2.0f*q-1.0f, 0.5f*q-0.5f);

    uniform_real_distribution<float> sdist{slb, sub};
    float const s = sdist(seed);
    Hex<float> const rhex{Q+q, -Q-S-q-s};

    Hex<long> const expected{Q, -Q-S};
    auto ihex = hex_round<long>(rhex);
    EXPECT_EQ(ihex, expected);
}

TEST(HexRoundTest, BackUpQPositiveRSNegative) {
    random_device seed;
    uniform_int_distribution<long> cdist {-4096, -1024};
    uniform_real_distribution<long double> qdist{-0.5, 0};

    long const R = cdist(seed);
    long const S = cdist(seed);

    long double const q = qdist(seed);
    long double const rlb = q + 0.5l;
    long double const rub = min(2.0l*q+1.0l, 0.5l*q+0.5l);

    uniform_real_distribution<long double> rdist{rlb, rub};
    long double const r = rdist(seed);
    Hex<long double> const rhex{-R-S+q, R+r};

    Hex<long> const expected{-R-S, R};
    auto ihex = hex_round<long>(rhex);
    EXPECT_EQ(ihex, expected);
}
