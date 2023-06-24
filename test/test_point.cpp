#include "gtest/gtest.h"
#include "tess.hpp"
#include "utility.hpp"
#include <limits>
#include <random>
#include <cstdlib>

using namespace tess;
using namespace std;

TEST(PointTest, BothComponentsZeroInt) {
    double const eps = 0.0001;

    int_point const p(0, 0);
    EXPECT_EQ(p.x, 0);
    EXPECT_EQ(p.y, 0);
    EXPECT_EQ(norm(p), 0.0);

    auto const pn = -p;
    EXPECT_EQ(pn, int_point::zero);

    auto const pa = p+p;
    EXPECT_EQ(pa, int_point::zero);

    auto const ps = p-p;
    EXPECT_EQ(ps, int_point::zero);
}

TEST(PointTest, BothComponentsZeroReal) {
    pointd const p(0.0, 0.0);
    EXPECT_EQ(p.x, 0.0);
    EXPECT_EQ(p.y, 0.0);
    EXPECT_EQ(norm(p), 0.0);

    auto const pn = -p;
    EXPECT_EQ(pn, pointd::zero);

    auto const pa = p+p;
    EXPECT_EQ(pa, pointd::zero);
}

TEST(PointTest, OneComponentZeroOtherPositiveAndSmallInt) {
    double const eps = 0.0001;

    random_device seed;
    uniform_int_distribution<> dist(2, 9);

    int const y1 = dist(seed);
    int_point const p1{0, y1};

    EXPECT_EQ(p1.x, 0);
    EXPECT_EQ(p1.y, y1);
    EXPECT_LT(abs(norm(p1)-y1), eps);

    int_point const ep1n{0, -y1};
    auto const ap1n = -p1;
    EXPECT_EQ(ap1n, ep1n);

    int const y2 = dist(seed);
    int_point const p2{0, y2};

    int_point const ep21s{0, y2-y1};
    auto const ap21s = p2-p1;
    EXPECT_EQ(ap21s, ep21s);
}

TEST(PointTest, OneComponentZeroOtherPositiveAndSmallReal) {
    random_device seed;
    double const eps = 0.001;
    uniform_real_distribution<> dist(1.0, 10.0);

    double const x1 = dist(seed);
    pointd const p1(x1, 0.0);

    EXPECT_LT(p1.x-x1, eps);
    EXPECT_EQ(p1.y, 0.0);
    EXPECT_LT(abs(norm(p1)-x1), eps);

    double const x2 = dist(seed);
    pointd const p2(x2, 0.0);
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
    random_device seed;

    uniform_int_distribution<int> dist(20, 99);
    uniform_int_distribution<int> diff_dist(-9, 9);

    int const x1 = dist(seed);
    int const y1 = x1 + diff_dist(seed);
    double const norm1 = sqrt(x1*x1 + y1*y1);
    basic_point<int> const p1(x1, y1);

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

    basic_point<int> const p2(x2, y2);
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
    random_device seed;

    uniform_int_distribution<long>
        xdist(numeric_limits<long>::min()/2, numeric_limits<long>::min()/10);
    uniform_int_distribution<long>
        ydist(numeric_limits<long>::max()/10, numeric_limits<long>::max()/2);

    long const x1 = xdist(seed);
    long const y1 = ydist(seed);
    basic_point<long> const p1(x1, y1);
    EXPECT_EQ(p1.x, x1);
    EXPECT_EQ(p1.y, y1);

    long const x2 = xdist(seed);
    long const y2 = ydist(seed);
    basic_point<long> const p2(x2, y2);

    auto const p12a = p1+p2;
    EXPECT_EQ(p12a.x, x1+x2);
    EXPECT_EQ(p12a.y, y1+y2);

    auto const p12s = p1-p2;
    EXPECT_EQ(p12s.x, x1-x2);
    EXPECT_EQ(p12s.y, y1-y2);
}

TEST(PointTest, XPositiveInfinityYFinite) {
    random_device seed;

    uniform_real_distribution<long double>
        rdist(numeric_limits<long double>::min(),
              numeric_limits<long double>::max());

    long double const ry1 = rdist(seed);
    const auto inf = numeric_limits<long double>::infinity();
    basic_point<long double> const rp1(inf, ry1);
    EXPECT_EQ(rp1.x, numeric_limits<long double>::infinity());
    EXPECT_EQ(norm(rp1), numeric_limits<double>::infinity());

    long double const rx2 = rdist(seed);
    long double const ry2 = rdist(seed);
    basic_point<long double> const rp2(rx2, ry2);

    auto const rp12a = rp1+rp2;
    EXPECT_EQ(rp12a.x, numeric_limits<long double>::infinity());
    EXPECT_EQ(norm(rp12a), numeric_limits<long double>::infinity());
}

TEST(PointTest, XFiniteAndYNegativeInfinity) {
    random_device seed;

    uniform_real_distribution<float> dist(numeric_limits<float>::min(),
                                          numeric_limits<float>::max());

    float const x1 = dist(seed);
    point const p1(x1, -numeric_limits<float>::infinity());
    EXPECT_EQ(p1.x, x1);
    EXPECT_EQ(p1.y, -numeric_limits<float>::infinity());
    EXPECT_EQ(norm(p1), numeric_limits<float>::infinity());

    auto const p1n = -p1;
    EXPECT_EQ(p1n.x, -x1);
    EXPECT_EQ(p1n.y, numeric_limits<float>::infinity());
    EXPECT_EQ(norm(p1n), numeric_limits<double>::infinity());

    float const x2 = dist(seed);
    point const p2(x2, dist(seed));

    auto const p12s = p1-p2;
    EXPECT_EQ(p12s.x, x1-x2);
    EXPECT_EQ(p12s.y, -numeric_limits<float>::infinity());
    EXPECT_EQ(norm(p12s), numeric_limits<double>::infinity());
}

TEST(PointTest, PointZero) {
    int_point const expected{0, 0};
    EXPECT_EQ(int_point::zero, expected);
}

TEST(PointTest, PointLeft) {
    basic_point<long> const expected{-1, 0};
    EXPECT_EQ(basic_point<long>::left, expected);
}

TEST(PointTest, PointUp) {
    pointd const expected{0.0, 1.0};
    EXPECT_LT(norm(pointd::up-expected), 0.0001);
}

TEST(PointTest, PointRight) {
    point const expected{1.0f, 0.0f};
    EXPECT_LT(norm(point::right-expected), 0.001);
}

TEST(PointTest, PointDown) {
    basic_point<long double> const expected{0.0l, -1.0l};
    EXPECT_LT(norm(basic_point<long double>::down-expected), 0.0001);
}
