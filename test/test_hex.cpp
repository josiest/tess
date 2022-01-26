#include "gtest/gtest.h"
#include "tess.hpp"
#include <limits>           // infinity, min, max
#include <random>           /* random_device, uniform_int_distribution
                             * unifom_real_distribution */
#include <cstdlib>          // 
#include <iostream>

using namespace tess;
using namespace std;

TEST(HexTest, AllIntComponentsZero) {
    Hex<int> const hex{0, 0};
    EXPECT_EQ(hex.q(), 0);
    EXPECT_EQ(hex.r(), 0);
    EXPECT_EQ(hex.s(), 0);
    EXPECT_EQ(hex_norm(hex), 0);

    auto const hexn = -hex;
    EXPECT_EQ(hexn, Hex<int>::zero);

    auto const hexa = hex+hex;
    EXPECT_EQ(hexa, Hex<int>::zero);

    auto const hexs = hex-hex;
    EXPECT_EQ(hexs, Hex<int>::zero);
}

TEST(HexTest, AllRealComponentsZero) {
    double const eps = 0.0001;

    Hex<double> const hex(0.0, 0.0);
    EXPECT_EQ(hex.q(), 0.0);
    EXPECT_EQ(hex.r(), 0.0);
    EXPECT_EQ(hex.s(), 0.0);
    EXPECT_EQ(hex_norm(hex), 0.0);

    auto const hexr = hex_round<int>(hex);
    EXPECT_EQ(hexr.q(), 0);
    EXPECT_EQ(hexr.r(), 0);
    EXPECT_EQ(hexr.s(), 0);

    auto const hexn = -hex;
    EXPECT_LT(abs(hexn.q()), eps);
    EXPECT_LT(abs(hexn.r()), eps);
    EXPECT_LT(abs(hexn.s()), eps);

    auto const hexa = hex+hex;
    EXPECT_LT(abs(hexa.q()), eps);
    EXPECT_LT(abs(hexa.r()), eps);
    EXPECT_LT(abs(hexa.s()), eps);
}

TEST(HexTest, IntOneComponentZeroOtherPositiveAndSmall) {
    double const eps = 0.0001;

    random_device seed;
    uniform_int_distribution<> dist(2, 9);

    int const r1 = dist(seed);
    Hex<int> const hex1(0, r1);

    EXPECT_EQ(hex1.q(), 0);
    EXPECT_EQ(hex1.r(), r1);
    EXPECT_EQ(hex1.s(), -r1);
    EXPECT_EQ(hex_norm(hex1), r1);

    auto const hex1n = -hex1;
    EXPECT_EQ(hex1n.q(), 0);
    EXPECT_EQ(hex1n.r(), -r1);
    EXPECT_EQ(hex1n.s(), r1);

    int const r2 = dist(seed);
    Hex<int> const hex2(0, r2);
    auto const hex21s = hex2-hex1;

    EXPECT_EQ(hex21s.q(), 0);
    EXPECT_EQ(hex21s.r(), r2-r1);
    EXPECT_EQ(hex21s.s(), r1-r2);
}

TEST(HexTest, RealOneComponentZeroOtherPositiveAndSmall) {
    double const eps = 0.0001;

    random_device seed;
    uniform_real_distribution<> dist(1.0001, 9.9999);

    double const q1 = dist(seed);
    Hex<double> const hex1(q1, 0.0);

    EXPECT_LT(hex1.q()-q1, eps);
    EXPECT_EQ(hex1.r(), 0.0);
    EXPECT_LT(hex1.s()+q1, eps);
    EXPECT_LT(abs(hex_norm(hex1)-q1), eps);

    double const q2 = dist(seed);
    Hex<double> const hex2(q2, 0.0);

    auto const hex12a = hex1 + hex2;
    EXPECT_LT(hex12a.q()-(q1+q2), eps);
    EXPECT_EQ(hex12a.r(), 0.0);
    EXPECT_LT(hex12a.s()+(q1+q2), eps);

    auto const hex12s = hex1 - hex2;
    EXPECT_LT(hex12s.q()-abs(q1-q2), eps);
    EXPECT_EQ(hex12s.r(), 0.0);
    EXPECT_LT(hex12s.s()-abs(q2-q1), eps);
}

TEST(HexTest, BothIntComponentsEqualInMagnitudeOpposingSigns) {
    double const eps = 0.0001;
    random_device seed;

    uniform_int_distribution<long> dist1(20, 99);
    auto const n1 = dist1(seed);

    Hex<long> const h1(-n1, n1);
    EXPECT_EQ(h1.q(), -n1);
    EXPECT_EQ(h1.r(), n1);
    EXPECT_EQ(h1.s(), 0);
    EXPECT_EQ(hex_norm(h1), n1);

    auto const h1n = -h1;
    EXPECT_EQ(h1n.q(), n1);
    EXPECT_EQ(h1n.r(), -n1);
    EXPECT_EQ(h1n.s(), 0);

    uniform_int_distribution<long> dist2(100, numeric_limits<long>::max());
    auto const n2 = dist2(seed);
    Hex<long> const h2(n2, -n2);

    auto const h21s = h2 - h1;
    EXPECT_EQ(h21s.q(), n2 + n1);
    EXPECT_EQ(h21s.r(), -n2 - n1);
    EXPECT_EQ(h21s.s(), 0);
}

TEST(HexTest, RealComponentsDifferentMagnitudeSameSign) {
    double const eps = 0.001;
    random_device seed;

    uniform_real_distribution<float> dist(256.0f, 511.9999f);
    auto const q1 = dist(seed);
    auto const r1 = dist(seed);

    Hex<float> const h1{q1, r1};
    EXPECT_EQ(h1.q(), q1);
    EXPECT_EQ(h1.r(), r1);
    EXPECT_LT(h1.s()+q1+r1, eps);
    EXPECT_LT(hex_norm(h1)-q1-r1, eps);

    auto const q2 = dist(seed);
    auto const r2 = dist(seed);
    Hex<float> const h2{q2, r2};

    auto const h12a = h1 + h2;
    EXPECT_LT(h12a.q()-q1-q2, eps);
    EXPECT_LT(h12a.r()-r1-r2, eps);
    EXPECT_LT(h12a.s()+q1+r1+q2+r2, eps);
}

TEST(HexTest, IntComponentsDifferentSignAndMagnitude) {
    random_device seed;

    uniform_int_distribution<short> pdist(1, numeric_limits<short>::max()/2);
    uniform_int_distribution<short> ndist(numeric_limits<short>::min()/2, -1);
    short const q1 = pdist(seed);
    short const r1 = ndist(seed);

    Hex<short> const h1{q1, r1};
    EXPECT_EQ(h1.q(), q1);
    EXPECT_EQ(h1.r(), r1);
    EXPECT_EQ(h1.s(), -q1-r1);
    EXPECT_EQ(hex_norm(h1), (q1-r1+abs(-q1-r1))/2);

    short const q2 = ndist(seed);
    short const r2 = pdist(seed);
    Hex<short> const h2{q2, r2};

    auto const h12a = h1 + h2;
    EXPECT_EQ(h12a.q(), q1+q2);
    EXPECT_EQ(h12a.r(), r1+r2);
    EXPECT_EQ(h12a.s(), -q1-r1-q2-r2);

    auto const h12s = h1 - h2;
    EXPECT_EQ(h12s.q(), q1-q2);
    EXPECT_EQ(h12s.r(), r1-r2);
    EXPECT_EQ(h12s.s(), -q1-r1+q2+r2);
}

TEST(HexTest, OneComponentInfinite) {
    random_device seed;

    uniform_real_distribution<long double>
        dist(numeric_limits<long double>::min(),
             numeric_limits<long double>::max());

    bool failed = false;
    try {
        Hex<long double> const
            hex(numeric_limits<long double>::infinity(), dist(seed));
    }
    catch (...) {
        failed = true;
    }
    EXPECT_TRUE(failed);
}

TEST(HexTest, OneComponentNaN) {
    random_device seed;
    uniform_real_distribution<long double>
        dist(numeric_limits<long double>::min(),
             numeric_limits<long double>::max());

    bool failed = false;
    try {
        Hex<long double> const h(dist(seed), nanl("1"));
    }
    catch (...) {
        failed = true;
    }
    EXPECT_TRUE(failed);
}

TEST(HexTest, OneComponentNaNOneInfinite){
    bool failed = false;
    try {
        Hex<float> const h(0.0f/0.0f, numeric_limits<float>::infinity());
    }
    catch (...) {
        failed = true;
    }
    EXPECT_TRUE(failed);
}

TEST(HexTest, HexZero) {
    Hex<int> expected{0, 0};
    EXPECT_EQ(Hex<int>::zero, expected);
}

TEST(HexTest, HexLeftUp) {
    Hex<int> expected{0, -1};
    EXPECT_EQ(Hex<int>::left_up, expected);
    EXPECT_EQ(expected.s(), 1);
}

TEST(HexTest, HexForwardLeft) {
    Hex<short> expected{1, -1};
    EXPECT_EQ(Hex<short>::forward_left, expected);
    EXPECT_EQ(expected.s(), 0);
}

TEST(HexTest, HexForwardDown) {
    Hex<long> expected{1, 0};
    EXPECT_EQ(Hex<long>::forward_down, expected);
    EXPECT_EQ(expected.s(), -1);
}

TEST(HexTest, HexRightDown) {
    double const eps = 0.0001;
    Hex<double> expected{0.0, 1.0};
    EXPECT_LT(hex_norm(Hex<double>::right_down-expected), eps);
    EXPECT_LT(abs(expected.s()+1.0), eps);
}

TEST(HexTest, HexBackRight) {
    double const eps = 0.001;
    Hex<float> expected{-1.0f, 1.0f};
    EXPECT_LT(hex_norm(Hex<float>::back_right-expected), eps);
    EXPECT_LT(abs(expected.s()), eps);
}

TEST(HexTest, HexBackUp) {
    double const eps = 0.0001;
    Hex<long double> expected{-1.0l, 0.0l};
    EXPECT_LT(hex_norm(Hex<long double>::back_up-expected), eps);
    EXPECT_LT(abs(expected.s()-1.0), eps);
}
