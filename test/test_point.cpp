#include "gtest/gtest.h"
#include "tess.hpp"
#include "utility.hpp"

#include <limits>
#include <random>
#include <cstdlib>

using namespace tess;
using long_point = point<long>;

TEST(PointTest, BothComponentsZeroInt) {
    double const eps = 0.0001;

    point const p(0, 0);
    EXPECT_EQ(p.x, 0);
    EXPECT_EQ(p.y, 0);
    EXPECT_EQ(norm(p), 0.0);

    auto const pn = -p;
    EXPECT_EQ(pn, ipoint::zero);

    auto const pa = p+p;
    EXPECT_EQ(pa, ipoint::zero);

    auto const ps = p-p;
    EXPECT_EQ(ps, ipoint::zero);
}

TEST(PointTest, BothComponentsZeroReal) {
    point const p(0.0, 0.0);
    EXPECT_EQ(p.x, 0.0);
    EXPECT_EQ(p.y, 0.0);
    EXPECT_EQ(norm(p), 0.0);

    auto const pn = -p;
    EXPECT_EQ(pn, dpoint::zero);

    auto const pa = p+p;
    EXPECT_EQ(pa, dpoint::zero);
}

TEST(PointTest, OneComponentZeroOtherPositiveAndSmallInt) {
    double const eps = 0.0001;

    std::random_device seed;
    std::uniform_int_distribution dist(2, 9);

    int const y1 = dist(seed);
    point const p1{0, y1};

    EXPECT_EQ(p1.x, 0);
    EXPECT_EQ(p1.y, y1);
    EXPECT_LT(abs(norm(p1)-y1), eps);

    point const ep1n{0, -y1};
    auto const ap1n = -p1;
    EXPECT_EQ(ap1n, ep1n);

    int const y2 = dist(seed);
    point const p2{0, y2};

    point const ep21s{0, y2-y1};
    auto const ap21s = p2-p1;
    EXPECT_EQ(ap21s, ep21s);
}

TEST(PointTest, OneComponentZeroOtherPositiveAndSmallReal) {
    std::random_device seed;
    double const eps = 0.001;
    std::uniform_real_distribution dist(1., 10.);

    double const x1 = dist(seed);
    point const p1(x1, 0.0);

    EXPECT_LT(p1.x-x1, eps);
    EXPECT_EQ(p1.y, 0.0);
    EXPECT_LT(abs(norm(p1)-x1), eps);

    double const x2 = dist(seed);
    point const p2(x2, 0.0);
    auto const p12a = p1+p2;

    EXPECT_LT(p12a.x-(x1+x2), eps);
    EXPECT_EQ(p12a.y, 0.0);
    EXPECT_LT(abs(norm(p12a)-(x1+x2)), eps);

    auto const p12s = p1-p2;
    EXPECT_LT(p12s.x-abs(x1-x2), eps);
    EXPECT_EQ(p12s.y, 0.0);
    EXPECT_LT(abs(norm(p12s)-abs(x1-x2)), eps);
}

TEST(PointTest, BothComponentsPositiveAndLargeWithSmallDifferenceInt) {
    double eps = 0.0001;
    std::random_device seed;

    std::uniform_int_distribution dist(20, 99);
    std::uniform_int_distribution diff_dist(-9, 9);

    int const x1 = dist(seed);
    int const y1 = x1 + diff_dist(seed);
    double const norm1 = sqrt(x1*x1 + y1*y1);
    point const p1(x1, y1);

    EXPECT_EQ(p1.x, x1);
    EXPECT_EQ(p1.y, y1);
    EXPECT_GE(norm(p1), 0.0f);
    EXPECT_LT(abs(norm(p1)-norm1), eps);

    auto const p1n = -p1;
    EXPECT_EQ(p1n.x, -x1);
    EXPECT_EQ(p1n.y, -y1);
    EXPECT_GE(norm(p1n), 0.0);
    EXPECT_LT(abs(norm(p1n)-norm1), eps);

    int const x2 = x1 + diff_dist(seed);
    int const y2 = y1 + diff_dist(seed);
    double const norm12a = sqrt((x1+x2)*(x1+x2) + (y1+y2)*(y1+y2));

    point const p2(x2, y2);
    auto const p12a = p1+p2;
    EXPECT_EQ(p12a.x, x1+x2);
    EXPECT_EQ(p12a.y, y1+y2);
    EXPECT_GE(norm(p12a), 0.0);
    EXPECT_LT(abs(norm(p12a)-norm12a), eps);
}

TEST(PointTest, BothComponentsPositiveAndLargeWithSmallDifferenceReal) {
    double eps = 0.0001;

    float const y1 = 87.2481;
    float const x1 = y1 - 1.51828;
    double const norm1 = sqrt(x1*x1 + y1*y1);
    point const p1(x1, y1);

    EXPECT_LT(p1.x-x1, eps);
    EXPECT_LT(p1.y-y1, eps);
    EXPECT_GE(norm(p1), 0.0f);
    EXPECT_LT(abs(norm(p1)-norm1), eps);

    float const x2 = x1 + 8.17967;
    float const y2 = y2 + 2.19761;
    point const p2(x2, y2);

    double const norm12a = sqrt((x1+x2)*(x1+x2) + (y1+y2)*(y1+y2));
    auto const p12a = p1 + p2;

    EXPECT_LT(abs(p12a.x-(x1+x2)), eps);
    EXPECT_LT(abs(p12a.y-(y1+y2)), eps);
    EXPECT_GE(norm(p12a), 0.0f);
    EXPECT_LT(abs(norm(p12a)-norm12a), eps);

    double const norm21s = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
    auto const p21s = p2 - p1;
    EXPECT_LT(abs(p21s.x-(x2-x1)), eps);
    EXPECT_LT(abs(p21s.y-(y2-y1)), eps);
    EXPECT_GE(norm(p21s), 0.0);
    EXPECT_LT(abs(norm(p21s)-norm21s), eps);
}

TEST(PointTest, XVeryLargeAndNegativeYVeryLargeAndPositive) {
    std::random_device seed;

    constexpr auto long_min = std::numeric_limits<long>::min();
    constexpr auto long_max = std::numeric_limits<long>::min();

    std::uniform_int_distribution xdist(long_min/2, long_max/10);
    std::uniform_int_distribution ydist(long_max/10, long_max/2);

    long const x1 = xdist(seed);
    long const y1 = ydist(seed);
    long_point const p1(x1, y1);
    EXPECT_EQ(p1.x, x1);
    EXPECT_EQ(p1.y, y1);

    long const x2 = xdist(seed);
    long const y2 = ydist(seed);
    long_point const p2(x2, y2);

    auto const p12a = p1+p2;
    EXPECT_EQ(p12a.x, x1+x2);
    EXPECT_EQ(p12a.y, y1+y2);

    auto const p12s = p1-p2;
    EXPECT_EQ(p12s.x, x1-x2);
    EXPECT_EQ(p12s.y, y1-y2);
}

TEST(PointTest, XPositiveInfinityYFinite) {
    std::random_device seed;

    constexpr auto long_min = std::numeric_limits<long double>::min();
    constexpr auto long_max = std::numeric_limits<long double>::max();
    std::uniform_real_distribution rdist(long_min, long_max);

    auto const ry1 = rdist(seed);
    constexpr auto long_inf = std::numeric_limits<long double>::infinity();
    point const rp1(long_inf, ry1);
    EXPECT_EQ(rp1.x, long_inf);
    EXPECT_EQ(norm(rp1), long_inf);

    auto const rx2 = rdist(seed);
    auto const ry2 = rdist(seed);
    point const rp2(rx2, ry2);

    auto const rp12a = rp1+rp2;
    EXPECT_EQ(rp12a.x, long_inf);
    EXPECT_EQ(norm(rp12a), long_inf);
}

TEST(PointTest, XFiniteAndYNegativeInfinity) {
    std::random_device seed;

    constexpr auto float_min = std::numeric_limits<float>::min();
    constexpr auto float_max = std::numeric_limits<float>::max();
    std::uniform_real_distribution dist(float_min, float_max);

    float const x1 = dist(seed);
    point const p1(x1, -std::numeric_limits<float>::infinity());
    EXPECT_EQ(p1.x, x1);
    EXPECT_EQ(p1.y, -std::numeric_limits<float>::infinity());
    EXPECT_EQ(norm(p1), std::numeric_limits<float>::infinity());

    auto const p1n = -p1;
    EXPECT_EQ(p1n.x, -x1);
    EXPECT_EQ(p1n.y, std::numeric_limits<float>::infinity());
    EXPECT_EQ(norm(p1n), std::numeric_limits<double>::infinity());

    float const x2 = dist(seed);
    point const p2(x2, dist(seed));

    auto const p12s = p1-p2;
    EXPECT_EQ(p12s.x, x1-x2);
    EXPECT_EQ(p12s.y, -std::numeric_limits<float>::infinity());
    EXPECT_EQ(norm(p12s), std::numeric_limits<double>::infinity());
}

TEST(PointTest, PointZero) {
    point const expected_val{0, 0};
    EXPECT_EQ(ipoint::zero, expected_val);
}

TEST(PointTest, PointLeft) {
    long_point const expected_val{-1, 0};
    EXPECT_EQ(long_point::left, expected_val);
}

TEST(PointTest, PointUp) {
    point const expected_val{0.0, 1.0};
    EXPECT_LT(norm(dpoint::up-expected_val), 0.0001);
}

TEST(PointTest, PointRight) {
    point const expected_val{1.0f, 0.0f};
    EXPECT_LT(norm(fpoint::right-expected_val), 0.001f);
}

TEST(PointTest, PointDown) {
    point const expected_val{0.0l, -1.0l};
    EXPECT_LT(norm(point<long double>::down-expected_val), 0.0001l);
}
