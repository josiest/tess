#include "gtest/gtest.h"
#include "hax.hpp"
#include <random>
#include <limits>

using namespace hax;
using namespace std;

TEST(PointTest, GetXBothComponentsZero) {
    Point const p(0, 0);
    EXPECT_EQ(p.x(), 0);
}

TEST(PointTest, GetXAsZeroWithYPositiveInt) {
    Point const p1(0, 1);
    Point const p2(0, 2);
    Point const p3(0, 11);
    Point const p4(0, 7821);

    EXPECT_EQ(p1.x(), 0);
    EXPECT_EQ(p2.x(), 0);
    EXPECT_EQ(p3.x(), 0);
    EXPECT_EQ(p4.x(), 0);
}

TEST(PointTest, GetXAsZeroWithYPositiveFloat) {
    Point const p5(0, 0.0001);
    Point const p6(0, 0.7632);
    Point const p7(0, 13.282);

    EXPECT_EQ(p5.x(), 0);
    EXPECT_EQ(p6.x(), 0);
    EXPECT_EQ(p7.x(), 0);
}

TEST(PointTest, GetXAsZeroWithYNegativeInt) {
    Point const p1(0, -1);
    Point const p2(0, -3);
    Point const p3(0, -8);
    Point const p4(0, -28811);

    EXPECT_EQ(p1.x(), 0);
    EXPECT_EQ(p2.x(), 0);
    EXPECT_EQ(p3.x(), 0);
    EXPECT_EQ(p4.x(), 0);
}

TEST(PointTest, GetXAsZeroWithYNegativeFloat) {
    Point const p5(0, -0.00002);
    Point const p6(0, -0.582);
    Point const p7(0, -281.23);

    EXPECT_EQ(p5.x(), 0);
    EXPECT_EQ(p6.x(), 0);
    EXPECT_EQ(p7.x(), 0);
}

TEST(PointTest, GetXAsPositiveInt) {
    Point const p1(1, 0);
    Point const p2(2, -1);
    Point const p3(13, 781);
    Point const p4(82739, -13);

    EXPECT_EQ(p1.x(), 1);
    EXPECT_EQ(p2.x(), 2);
    EXPECT_EQ(p3.x(), 13);
    EXPECT_EQ(p4.x(), 82739);
}

TEST(PointTest, GetXAsPositiveFloat) {
    Point const p1(0.00002, 830.0);
    Point const p2(0.123, 0.0);
    Point const p3(1.0, -13.3);
    Point const p4(1332.31313, -2991.23);

    EXPECT_LT(p1.x()-0.00002, 0.000001);
    EXPECT_LT(p2.x()-0.123, 0.0001);
    EXPECT_LT(p3.x()-1.0, 0.01);
    EXPECT_LT(p4.x()-1332.31313, 0.001);
}

TEST(PointTest, GetXAsNegativeInt) {
    Point const p1(-1, 13);
    Point const p2(-2, -2003);
    Point const p3(-82, -293);
    Point const p4(-12920, 32);

    EXPECT_EQ(p1.x(), -1);
    EXPECT_EQ(p2.x(), -2);
    EXPECT_EQ(p3.x(), -82);
    EXPECT_EQ(p4.x(), -12920);
}

TEST(PointTest, GetYBothComponentsZero) {
    Point const p(0, 0);
    EXPECT_EQ(p.y(), 0);
}

TEST(PointTest, GetYAsZeroWithXPositiveInt) {
    random_device rd;

    Point const p1(1, 0);
    EXPECT_EQ(p1.y(), 0);

    uniform_int_distribution<> dist2(2, 100);
    Point const p2(dist2(rd), 0);
    EXPECT_EQ(p2.y(), 0);

    uniform_int_distribution<> dist3(101, numeric_limits<int>::max()-1);
    Point const p3(dist3(rd), 0);
    EXPECT_EQ(p3.y(), 0);

    Point const p4(numeric_limits<int>::max(), 0);
    EXPECT_EQ(p4.y(), 0);
}

TEST(PointTest, GetYAsZeroWithXPositiveFloat) {
    random_device rd;

    Point const p1(1.0, 0);
    EXPECT_EQ(p1.y(), 0);

    uniform_real_distribution<float> dist2(0.000001, 0.00001);
    Point const p2(dist2(rd), 0);
    EXPECT_EQ(p2.y(), 0);

    uniform_real_distribution<float> dist3(0.001, 0.999);
    Point const p3(dist3(rd), 0);
    EXPECT_EQ(p3.y(), 0);

    uniform_real_distribution<float> dist4(1.001, 99.99);
    Point const p4(dist4(rd), 0);
    EXPECT_EQ(p4.y(), 0);

    uniform_real_distribution<float>
        dist5(100.0, numeric_limits<float>::max()-1);

    Point const p5(dist5(rd), 0);
    EXPECT_EQ(p5.y(), 0);

    Point const p6(numeric_limits<float>::max(), 0);
    EXPECT_EQ(p6.y(), 0);
}

TEST(PointTest, GetYAsZeroWithXNegativeInt) {
    random_device rd;

    Point const p1(-1, 0);
    EXPECT_EQ(p1.y(), 0);

    uniform_int_distribution<> dist2(-2, 99);
    Point const p2(dist2(rd), 0);
    EXPECT_EQ(p2.y(), 0);

    uniform_int_distribution<> dist3(-100, numeric_limits<int>::min()+1);
    Point const p3(dist3(rd), 0);
    EXPECT_EQ(p3.y(), 0);

    Point const p4(numeric_limits<int>::min(), 0);
    EXPECT_EQ(p4.y(), 0);
}

TEST(PointTest, GetYAsPositiveInt) {
    random_device rd;
    uniform_int_distribution<> xdist(numeric_limits<int>::min(),
                                     numeric_limits<int>::max());

    Point const p1(xdist(rd), 1);
    EXPECT_EQ(p1.y(), 1);

    uniform_int_distribution<> ydist2(2, 99);
    int const y2 = ydist2(rd);
    Point const p2(xdist(rd), y2);
    EXPECT_EQ(p2.y(), y2);

    uniform_int_distribution<> ydist3(100, numeric_limits<int>::max()-1);
    int const y3 = ydist3(rd);
    Point const p3(xdist(rd), y3);
    EXPECT_EQ(p3.y(), y3);

    // Point const p4(xdist(rd), numeric_limits<int>::max());
    // EXPECT_EQ(p3.y(), numeric_limits<int>::max());
}

TEST(PointTest, GetYAsPositiveFloat) {
    random_device rd;
    uniform_real_distribution<float> xdist(numeric_limits<float>::min(),
                                           numeric_limits<float>::max());

    uniform_real_distribution<float> ydist1(0.000001, 0.00001);
    float const y1 = ydist1(rd);
    Point const p1(xdist(rd), y1);
    EXPECT_LT(p1.y()-y1, 0.00001);

    uniform_real_distribution<float> ydist2(0.001, 0.999);
    float const y2 = ydist2(rd);
    Point const p2(xdist(rd), y2);
    EXPECT_LT(p2.y()-y2, 0.001);

    uniform_real_distribution<float> ydist3(1.001, 99.99);
    float const y3 = ydist3(rd);
    Point const p3(xdist(rd), y3);
    EXPECT_LT(p3.y()-y3, 0.01);
}

TEST(PointTest, GetYAsNegativeInt) {
    random_device rd;
    uniform_int_distribution<> xdist(numeric_limits<int>::min(),
                                     numeric_limits<int>::max());

    Point const p1(xdist(rd), -1);
    EXPECT_EQ(p1.y(), -1);

    uniform_int_distribution<> ydist2(-99, -2);
    int const y2 = ydist2(rd);
    Point const p2(xdist(rd), y2);
    EXPECT_EQ(p2.y(), y2);

    uniform_int_distribution<> ydist3(numeric_limits<int>::min()+1, -100);
    int const y3 = ydist3(rd);
    Point const p3(xdist(rd), y3);
    EXPECT_EQ(p3.y(), y3);

    Point const p4(xdist(rd), numeric_limits<int>::min());
    EXPECT_EQ(p4.y(), numeric_limits<int>::min());
}

TEST(PointTest, GetYasNegativeFloat) {
    random_device rd;
    uniform_real_distribution<float> xdist(numeric_limits<float>::min(),
                                           numeric_limits<float>::max());

    uniform_real_distribution<float> ydist1(-0.00001, -0.000001);
    float const y1 = ydist1(rd);
    Point const p1(xdist(rd), y1);
    EXPECT_LT(p1.y()-y1, 0.00001);

    uniform_real_distribution<float> ydist2(-0.999, -0.001);
    float const y2 = ydist2(rd);
    Point const p2(xdist(rd), y2);
    EXPECT_LT(p2.y()-y2, 0.001);

    uniform_real_distribution<float> ydist3(-99.99, -1.001);
    float const y3 = ydist3(rd);
    Point const p3(xdist(rd), y3);
    EXPECT_LT(p3.y()-y3, 0.01);
}
