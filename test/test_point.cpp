#include "gtest/gtest.h"
#include "hax.hpp"
#include <limits>
#include <random>
#include <cstdlib>

using namespace hax;
using namespace std;

TEST(PointTest, BothComponentsZero) {
    double const eps = 0.0001;
    float const epsf = 0.0001f;

    Point<int> const ip(0, 0);
    EXPECT_EQ(ip.x(), 0);
    EXPECT_EQ(ip.y(), 0);
    EXPECT_EQ(norm(ip), 0.0);
    EXPECT_EQ(normf(ip), 0.0f);

    auto const ipn = -ip;
    EXPECT_EQ(ipn.x(), 0);
    EXPECT_EQ(ipn.y(), 0);
    EXPECT_EQ(norm(ipn), 0.0);

    auto const ipa = ip+ip;
    EXPECT_EQ(ipa.x(), 0);
    EXPECT_EQ(ipa.y(), 0);
    EXPECT_EQ(normf(ipa), 0.0f);

    auto const ips = ip-ip;
    EXPECT_EQ(ips.x(), 0);
    EXPECT_EQ(ips.y(), 0);
    EXPECT_EQ(norm(ips), 0.0);

    Point<double> const dp(0.0, 0.0);
    EXPECT_EQ(dp.x(), 0.0);
    EXPECT_EQ(dp.y(), 0.0);
    EXPECT_EQ(norm(dp), 0.0);
    EXPECT_EQ(normf(dp), 0.0f);

    auto const dpn = -dp;
    EXPECT_LT(abs(dpn.x()), eps);
    EXPECT_LT(abs(dpn.y()), eps);
    EXPECT_GE(normf(dpn), 0.0f);
    EXPECT_LT(normf(dpn), epsf);

    auto const dpa = dp+dp;
    EXPECT_LT(abs(dpa.x()), eps);
    EXPECT_LT(abs(dpa.y()), eps);
    EXPECT_GE(norm(dpa), 0.0);
    EXPECT_LT(norm(dpa), eps);
}

TEST(PointTest, OneComponentZeroOtherPositiveAndSmall) {
    double const eps = 0.0001;

    random_device rd;
    uniform_int_distribution<> idist(2, 9);

    int const iy1 = idist(rd);
    Point<int> const ip1(0, iy1);

    EXPECT_EQ(ip1.x(), 0);
    EXPECT_EQ(ip1.y(), iy1);
    EXPECT_LT(abs(norm(ip1)-iy1), eps);
    EXPECT_LT(abs(normf(ip1)-iy1), eps);

    auto const ip1n = -ip1;
    EXPECT_EQ(ip1n.x(), 0);
    EXPECT_EQ(ip1n.y(), -iy1);
    EXPECT_GE(norm(ip1n), 0.0);
    EXPECT_LT(abs(norm(ip1n)-iy1), eps);

    int const iy2 = idist(rd);
    Point<int> const ip2(0, iy2);
    auto const ip21s = ip2-ip1;

    EXPECT_EQ(ip21s.x(), 0);
    EXPECT_EQ(ip21s.y(), iy2-iy1);
    EXPECT_GE(normf(ip21s), 0.0f);
    EXPECT_LT(abs(normf(ip21s)-abs(iy2-iy1)), eps);

    uniform_real_distribution<> rdist(1.0001, 9.9999);

    double const rx1 = rdist(rd);
    Point<double> const rp1(rx1, 0.0);

    EXPECT_LT(rp1.x()-rx1, eps);
    EXPECT_EQ(rp1.y(), 0.0);
    EXPECT_LT(abs(normf(rp1)-rx1), eps);

    double const rx2 = rdist(rd);
    Point<double> const rp2(rx2, 0.0);
    auto const rp12a = rp1+rp2;

    EXPECT_LT(rp12a.x()-(rx1+rx2), eps);
    EXPECT_EQ(rp12a.y(), 0.0);
    EXPECT_LT(abs(norm(rp12a)-(rx1+rx2)), eps);

    auto const rp12s = rp1-rp2;
    EXPECT_LT(rp12s.x()-abs(rx1-rx2), eps);
    EXPECT_EQ(rp12s.y(), 0.0);
    EXPECT_LT(abs(norm(rp12s)-abs(rx1-rx2)), eps);
}

TEST(PointTest, BothComponentsPositiveAndLargeWithSmallDifference) {
    double eps = 0.0001;
    random_device seed;

    uniform_int_distribution<short> idist(20, 99);
    uniform_int_distribution<short> idiff_dist(-9, 9);

    short const ix1 = idist(seed);
    short const iy1 = ix1 + idiff_dist(seed);
    double const inorm1 = sqrt(ix1*ix1 + iy1*iy1);
    Point<short> const ip1(ix1, iy1);

    EXPECT_EQ(ip1.x(), ix1);
    EXPECT_EQ(ip1.y(), iy1);
    EXPECT_GE(normf(ip1), 0.0f);
    EXPECT_LT(abs(normf(ip1)-inorm1), eps);

    auto const ip1n = -ip1;
    EXPECT_EQ(ip1n.x(), -ix1);
    EXPECT_EQ(ip1n.y(), -iy1);
    EXPECT_GE(norm(ip1n), 0.0);
    EXPECT_LT(abs(norm(ip1n)-inorm1), eps);

    short const ix2 = ix1 + idiff_dist(seed);
    short const iy2 = iy1 + idiff_dist(seed);
    double const inorm12a = sqrt((ix1+ix2)*(ix1+ix2) + (iy1+iy2)*(iy1+iy2));

    Point<short> const ip2(ix2, iy2);
    auto const ip12a = ip1+ip2;
    EXPECT_EQ(ip12a.x(), ix1+ix2);
    EXPECT_EQ(ip12a.y(), iy1+iy2);
    EXPECT_GE(norm(ip12a), 0.0);
    EXPECT_LT(abs(norm(ip12a)-inorm12a), eps);

    uniform_real_distribution<float> rdist(20.01, 99.99);
    uniform_real_distribution<float> rdiff_dist(-9.99, 9.99);

    float const ry1 = rdist(seed);
    float const rx1 = ry1 + rdiff_dist(seed);
    double const rnorm1 = sqrt(rx1*rx1 + ry1*ry1);
    Point<float> const rp1(rx1, ry1);

    EXPECT_LT(rp1.x()-rx1, eps);
    EXPECT_LT(rp1.y()-ry1, eps);
    EXPECT_GE(normf(rp1), 0.0f);
    EXPECT_LT(abs(normf(rp1)-rnorm1), eps);

    float const rx2 = rx1 + rdiff_dist(seed);
    float const ry2 = ry2 + rdiff_dist(seed);
    Point<float> const rp2(rx2, ry2);

    double const rnorm12a = sqrt((rx1+rx2)*(rx1+rx2) + (ry1+ry2)*(ry1+ry2));
    auto const rp12a = rp1 + rp2;

    EXPECT_LT(abs(rp12a.x()-(rx1+rx2)), eps);
    EXPECT_LT(abs(rp12a.y()-(ry1+ry2)), eps);
    EXPECT_GE(normf(rp12a), 0.0f);
    EXPECT_LT(abs(normf(rp12a)-rnorm12a), eps);

    double const rnorm21s = sqrt((rx2-rx1)*(rx2-rx1) + (ry2-ry1)*(ry2-ry1));
    auto const rp21s = rp2 - rp1;
    EXPECT_LT(abs(rp21s.x()-(rx2-rx1)), eps);
    EXPECT_LT(abs(rp21s.y()-(ry2-ry1)), eps);
    EXPECT_GE(norm(rp21s), 0.0);
    EXPECT_LT(abs(norm(rp21s)-rnorm21s), eps);
}

TEST(PointTest, XVeryLargeAndNegativeYVeryLargeAndPositive) {
    random_device seed;

    uniform_int_distribution<long>
        xdist(numeric_limits<long>::min()/2, numeric_limits<long>::min()/10);
    uniform_int_distribution<long>
        ydist(numeric_limits<long>::max()/10, numeric_limits<long>::max()/2);

    long const x1 = xdist(seed);
    long const y1 = ydist(seed);
    Point<long> const p1(x1, y1);
    EXPECT_EQ(p1.x(), x1);
    EXPECT_EQ(p1.y(), y1);

    long const x2 = xdist(seed);
    long const y2 = ydist(seed);
    Point<long> const p2(x2, y2);

    auto const p12a = p1+p2;
    EXPECT_EQ(p12a.x(), x1+x2);
    EXPECT_EQ(p12a.y(), y1+y2);

    auto const p12s = p1-p2;
    EXPECT_EQ(p12s.x(), x1-x2);
    EXPECT_EQ(p12s.y(), y1-y2);
}

TEST(PointTest, XPositiveInfinityYFinite) {
    random_device seed;

    uniform_real_distribution<long double>
        rdist(numeric_limits<long double>::min(),
              numeric_limits<long double>::max());

    long double const ry1 = rdist(seed);
    Point<long double> const rp1(numeric_limits<long double>::infinity(), ry1);
    EXPECT_EQ(rp1.x(), numeric_limits<long double>::infinity());
    EXPECT_EQ(norm(rp1), numeric_limits<double>::infinity());

    long double const rx2 = rdist(seed);
    long double const ry2 = rdist(seed);
    Point<long double> const rp2(rx2, ry2);

    auto const rp12a = rp1+rp2;
    EXPECT_EQ(rp12a.x(), numeric_limits<long double>::infinity());
    EXPECT_EQ(norm(rp12a), numeric_limits<long double>::infinity());
}

TEST(PointTest, XFiniteAndYNegativeInfinity) {
    random_device seed;

    uniform_real_distribution<float> dist(numeric_limits<float>::min(),
                                          numeric_limits<float>::max());

    float const x1 = dist(seed);
    Point<float> const p1(x1, -numeric_limits<float>::infinity());
    EXPECT_EQ(p1.x(), x1);
    EXPECT_EQ(p1.y(), -numeric_limits<float>::infinity());
    EXPECT_EQ(normf(p1), numeric_limits<float>::infinity());

    auto const p1n = -p1;
    EXPECT_EQ(p1n.x(), -x1);
    EXPECT_EQ(p1n.y(), numeric_limits<float>::infinity());
    EXPECT_EQ(norm(p1n), numeric_limits<double>::infinity());

    float const x2 = dist(seed);
    Point<float> const p2(x2, dist(seed));

    auto const p12s = p1-p2;
    EXPECT_EQ(p12s.x(), x1-x2);
    EXPECT_EQ(p12s.y(), -numeric_limits<float>::infinity());
    EXPECT_EQ(norm(p12s), numeric_limits<double>::infinity());
}

TEST(PointTest, XisNaNAndYInfinite){
    bool failed = false;
    try {
        Point<float> const p(0.0f/0.0f, numeric_limits<float>::infinity());
    }
    catch (...) {
        failed = true;
    }
    EXPECT_TRUE(failed);
}

TEST(PointTest, XFiniteAndYisNaN) {
    random_device seed;
    uniform_real_distribution<long double>
        dist(numeric_limits<long double>::min(),
             numeric_limits<long double>::max());

    bool failed = false;
    try {
        Point<long double> const p(dist(seed), nanl("1"));
    }
    catch (...) {
        failed = true;
    }
    EXPECT_TRUE(failed);
}

TEST(PointTest, XInfiniteSubInfinite) {
    random_device seed;
    uniform_real_distribution<double> dist(numeric_limits<double>::min(),
                                           numeric_limits<double>::max());

    Point<double> const p1(numeric_limits<double>::infinity(), dist(seed));
    Point<double> const p2(numeric_limits<double>::infinity(), dist(seed));

    bool failed = false;
    try {
        auto const p3 = p1 - p2;
    }
    catch (...) {
        failed = true;
    }
    EXPECT_TRUE(failed);
}
