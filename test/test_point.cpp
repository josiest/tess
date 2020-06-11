#include "gtest/gtest.h"
#include "hax.hpp"

using namespace hax;

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

//int main(int argc, char * argv[]) {
//    ::testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
//}
