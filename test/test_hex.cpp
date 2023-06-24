#include "gtest/gtest.h"
#include "tess.hpp"
#include "utility.hpp"

#include <limits>   // numeric_limits<T>::min
                    // numeric_limits<T>::max

#include <random>   // random_device, uniform_int_distribution
                    // uniform_real_distribution

#include <cmath>    // abs

using namespace tess;
using long_hex = basic_hex<long>;

TEST(HexTest, AllIntComponentsZero) {
    hex const h{0, 0};
    EXPECT_EQ(h.q, 0);
    EXPECT_EQ(h.r, 0);
    EXPECT_EQ(h.s(), 0);
    EXPECT_EQ(hex_norm(h), 0);

    auto const hexn = -h;
    EXPECT_EQ(hexn, hex::zero);

    auto const hexa = h+h;
    EXPECT_EQ(hexa, hex::zero);

    auto const hexs = h-h;
    EXPECT_EQ(hexs, hex::zero);
}

TEST(HexTest, AllRealComponentsZero) {
    double const eps = 0.0001;

    hexd const h(0.0, 0.0);
    EXPECT_EQ(h.q, 0.0);
    EXPECT_EQ(h.r, 0.0);
    EXPECT_EQ(h.s(), 0.0);
    EXPECT_EQ(hex_norm(h), 0.0);

    auto const hexr = hex_round<int>(h);
    EXPECT_EQ(hexr.q, 0);
    EXPECT_EQ(hexr.r, 0);
    EXPECT_EQ(hexr.s(), 0);

    auto const hexn = -h;
    EXPECT_LT(std::abs(hexn.q), eps);
    EXPECT_LT(std::abs(hexn.r), eps);
    EXPECT_LT(std::abs(hexn.s()), eps);

    auto const hexa = h+h;
    EXPECT_LT(std::abs(hexa.q), eps);
    EXPECT_LT(std::abs(hexa.r), eps);
    EXPECT_LT(std::abs(hexa.s()), eps);
}

TEST(HexTest, IntOneComponentZeroOtherPositiveAndSmall) {
    double const eps = 0.0001;

    std::random_device seed;
    std::uniform_int_distribution dist(2, 9);

    int const r1 = dist(seed);
    hex const hex1(0, r1);

    EXPECT_EQ(hex1.q, 0);
    EXPECT_EQ(hex1.r, r1);
    EXPECT_EQ(hex1.s(), -r1);
    EXPECT_EQ(hex_norm(hex1), r1);

    auto const hex1n = -hex1;
    EXPECT_EQ(hex1n.q, 0);
    EXPECT_EQ(hex1n.r, -r1);
    EXPECT_EQ(hex1n.s(), r1);

    int const r2 = dist(seed);
    hex const hex2(0, r2);
    auto const hex21s = hex2-hex1;

    EXPECT_EQ(hex21s.q, 0);
    EXPECT_EQ(hex21s.r, r2-r1);
    EXPECT_EQ(hex21s.s(), r1-r2);
}

TEST(HexTest, RealOneComponentZeroOtherPositiveAndSmall) {
    double const eps = 0.0001;

    std::random_device seed;
    std::uniform_real_distribution dist{ 1.0001, 9.9999 };

    double const q1 = dist(seed);
    hexd const hex1(q1, 0.0);

    EXPECT_LT(hex1.q-q1, eps);
    EXPECT_EQ(hex1.r, 0.0);
    EXPECT_LT(hex1.s()+q1, eps);
    EXPECT_LT(std::abs(hex_norm(hex1)-q1), eps);

    double const q2 = dist(seed);
    hexd const hex2(q2, 0.0);

    auto const hex12a = hex1 + hex2;
    EXPECT_LT(hex12a.q-(q1+q2), eps);
    EXPECT_EQ(hex12a.r, 0.0);
    EXPECT_LT(hex12a.s()+(q1+q2), eps);

    auto const hex12s = hex1 - hex2;
    EXPECT_LT(hex12s.q-std::abs(q1-q2), eps);
    EXPECT_EQ(hex12s.r, 0.0);
    EXPECT_LT(hex12s.s()-std::abs(q2-q1), eps);
}

TEST(HexTest, BothIntComponentsEqualInMagnitudeOpposingSigns) {
    double const eps = 0.0001;
    std::random_device seed;

    std::uniform_int_distribution<long> dist1(20, 99);
    auto const n1 = dist1(seed);

    long_hex const h1(-n1, n1);
    EXPECT_EQ(h1.q, -n1);
    EXPECT_EQ(h1.r, n1);
    EXPECT_EQ(h1.s(), 0);
    EXPECT_EQ(hex_norm(h1), n1);

    auto const h1n = -h1;
    EXPECT_EQ(h1n.q, n1);
    EXPECT_EQ(h1n.r, -n1);
    EXPECT_EQ(h1n.s(), 0);

    constexpr long long_max = std::numeric_limits<long>::max();
    std::uniform_int_distribution<long> dist2(100, long_max);
    auto const n2 = dist2(seed);
    long_hex const h2(n2, -n2);

    auto const h21s = h2 - h1;
    EXPECT_EQ(h21s.q, n2 + n1);
    EXPECT_EQ(h21s.r, -n2 - n1);
    EXPECT_EQ(h21s.s(), 0);
}

TEST(HexTest, RealComponentsDifferentMagnitudeSameSign) {
    double const eps = 0.001;
    std::random_device seed;

    std::uniform_real_distribution<float> dist(256.0f, 511.9999f);
    auto const q1 = dist(seed);
    auto const r1 = dist(seed);

    hexf const h1{q1, r1};
    EXPECT_EQ(h1.q, q1);
    EXPECT_EQ(h1.r, r1);
    EXPECT_LT(h1.s()+q1+r1, eps);
    EXPECT_LT(hex_norm(h1)-q1-r1, eps);

    auto const q2 = dist(seed);
    auto const r2 = dist(seed);
    hexf const h2{q2, r2};

    auto const h12a = h1 + h2;
    EXPECT_LT(h12a.q-q1-q2, eps);
    EXPECT_LT(h12a.r-r1-r2, eps);
    EXPECT_LT(h12a.s()+q1+r1+q2+r2, eps);
}

TEST(HexTest, IntComponentsDifferentSignAndMagnitude) {
    std::random_device seed;

    constexpr int int_max = std::numeric_limits<int>::max();
    constexpr int int_min = std::numeric_limits<int>::min();

    std::uniform_int_distribution<int> pdist{ 1, int_max/2 };
    std::uniform_int_distribution<int> ndist{ int_min/2, -1 };
    int const q1 = pdist(seed);
    int const r1 = ndist(seed);

    hex const h1{q1, r1};
    EXPECT_EQ(h1.q, q1);
    EXPECT_EQ(h1.r, r1);
    EXPECT_EQ(h1.s(), -q1-r1);
    EXPECT_EQ(hex_norm(h1), (q1-r1+std::abs(-q1-r1))/2);

    int const q2 = ndist(seed);
    int const r2 = pdist(seed);
    hex const h2{q2, r2};

    auto const h12a = h1 + h2;
    EXPECT_EQ(h12a.q, q1+q2);
    EXPECT_EQ(h12a.r, r1+r2);
    EXPECT_EQ(h12a.s(), -q1-r1-q2-r2);

    auto const h12s = h1 - h2;
    EXPECT_EQ(h12s.q, q1-q2);
    EXPECT_EQ(h12s.r, r1-r2);
    EXPECT_EQ(h12s.s(), -q1-r1+q2+r2);
}

TEST(HexTest, HexZero) {
    hex expected_hex{0, 0};
    EXPECT_EQ(hex::zero, expected_hex);
}

TEST(HexTest, HexLeftUp) {
    hex expected_hex{0, -1};
    EXPECT_EQ(hex::left_up, expected_hex);
    EXPECT_EQ(expected_hex.s(), 1);
}

TEST(HexTest, HexForwardLeft) {
    hex expected_hex{1, -1};
    EXPECT_EQ(hex::forward_left, expected_hex);
    EXPECT_EQ(expected_hex.s(), 0);
}

TEST(HexTest, HexForwardDown) {
    long_hex expected_hex{1, 0};
    EXPECT_EQ(long_hex::forward_down, expected_hex);
    EXPECT_EQ(expected_hex.s(), -1);
}

TEST(HexTest, HexRightDown) {
    double const eps = 0.0001;
    hexd expected_hex{0.0, 1.0};
    EXPECT_LT(hex_norm(hexd::right_down-expected_hex), eps);
    EXPECT_LT(std::abs(expected_hex.s()+1.0), eps);
}

TEST(HexTest, HexBackRight) {
    double const eps = 0.001;
    hexf expected_hex{-1.0f, 1.0f};
    EXPECT_LT(hex_norm(hexf::back_right-expected_hex), eps);
    EXPECT_LT(std::abs(expected_hex.s()), eps);
}

TEST(HexTest, HexBackUp) {
    double const eps = 0.0001;
    basic_hex<long double> expected_hex{-1.0l, 0.0l};
    EXPECT_LT(hex_norm(basic_hex<long double>::back_up-expected_hex), eps);
    EXPECT_LT(std::abs(expected_hex.s()-1.0), eps);
}
