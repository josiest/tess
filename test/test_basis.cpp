#include "gtest/gtest.h"
#include "hax.hpp"

using namespace hax;
using namespace std;
 
TEST(BasisTest, SizeIsOneAndOriginZero) {
    Basis<double> const basis(Point<int>::zero, 1);
    EXPECT_EQ(basis.origin(), Point<int>::zero);
    EXPECT_EQ(basis.unit_size(), 1);
    EXPECT_EQ(basis.pixel(Hex<int>::zero), Point<int>::zero);
    EXPECT_LT(hex_norm(basis.hex(Point<int>::zero)), 0.0001);

    auto const verts = basis.vertices(Hex<int>::zero);
    EXPECT_LE(norm(verts[0]-Point<int>(1, 1)), 1.0);
    EXPECT_EQ(verts[1], Point<int>(0, 1));
    EXPECT_EQ(verts[2], Point<int>(-1, 1));
    EXPECT_LE(norm(verts[3]-Point<int>(-1, -1)), 1.0);
    EXPECT_EQ(verts[4], Point<int>(0, -1));
    EXPECT_LE(norm(verts[5]-Point<int>(1, -1)), 1.0);
}

TEST(BasisTest, SizeIsSmallAndOriginPositive) {
    double const eps = 0.001;
    Point<long> const origin(7, 3);
    long const unit_size = 10;

    Basis<float, long> const basis{origin, unit_size};
    EXPECT_EQ(basis.origin(), origin);
    EXPECT_EQ(basis.unit_size(), unit_size);
    EXPECT_EQ(basis.pixel(Hex<long>::zero), origin);

    auto const actual = basis.hex(Point<long>::zero);
    Hex<float> const expected{-0.3042, -0.2};
    EXPECT_LT(hex_norm(actual-expected), eps);

    auto const verts = basis.vertices(Hex<long>::zero);
    EXPECT_EQ(verts[0], Point<long>(16, 8));
    EXPECT_EQ(verts[1], Point<long>(7, 13));
    EXPECT_EQ(verts[2], Point<long>(-2, 8));
    EXPECT_EQ(verts[3], Point<long>(-2, -2));
    EXPECT_EQ(verts[4], Point<long>(7, -7));
    EXPECT_EQ(verts[5], Point<long>(16, -2));
}

TEST(BasisTest, SizeIsLargeAndOriginNegative) {
    double const eps = 0.0001;
    short const unit_size = 255;
    Point<short> const origin(-583, -570);
    Basis<long double, short> const basis{origin, unit_size, HexTop::Flat};

    EXPECT_EQ(basis.origin(), origin);
    EXPECT_EQ(basis.unit_size(), unit_size);
    EXPECT_EQ(basis.pixel(Hex<short>(10, 32)), Point<short>(3242, 15772));

    auto const actual = basis.hex(Point<short>(-75, -21));
    Hex<long double> const expected{1.3281l, 0.57895l};
    EXPECT_LT(hex_norm(actual-expected), eps);

    auto const verts = basis.vertices(Hex<short>::zero);
    EXPECT_EQ(verts[0], Point<short>(-328, -570));
    EXPECT_EQ(verts[1], Point<short>(-456, -349));
    EXPECT_EQ(verts[2], Point<short>(-711, -349));
    EXPECT_EQ(verts[3], Point<short>(-838, -570));
    EXPECT_EQ(verts[4], Point<short>(-711, -791));
    EXPECT_EQ(verts[5], Point<short>(-456, -791));
}

TEST(BasisTest, OriginOneComponentNegativeOtherPositive) {
    double const eps = 0.001;
    Point<int> const origin(-59, 61);
    int const unit_size = 21;
    Basis<float, int> const basis{origin, unit_size, HexTop::Flat};

    EXPECT_EQ(basis.origin(), origin);
    EXPECT_EQ(basis.unit_size(), unit_size);
    EXPECT_EQ(basis.pixel(Hex<int>(0, -32)), Point<int>(-59, -1103));

    auto const actual = basis.hex(Point<int>(60, 0));
    Hex<float> const expected{3.7778f, -3.566f};
    EXPECT_LT(hex_norm(actual-expected), eps);

    auto const verts = basis.vertices(Hex<int>(46, 47));
    EXPECT_EQ(verts[0], Point<int>(1411, 2607));
    EXPECT_EQ(verts[1], Point<int>(1401, 2625));
    EXPECT_EQ(verts[2], Point<int>(1380, 2625));
    EXPECT_EQ(verts[3], Point<int>(1369, 2607));
    EXPECT_EQ(verts[4], Point<int>(1380, 2589));
    EXPECT_EQ(verts[5], Point<int>(1401, 2589));
}

TEST(BasisTest, OriginOneComponentZeroOtherPositive) {
    double const eps = 0.0001;
    Point<long> const origin(0, 170);
    long const unit_size = 39;
    Basis<double, long> const basis{origin, unit_size, HexTop::Pointed};

    EXPECT_EQ(basis.origin(), origin);
    EXPECT_EQ(basis.unit_size(), unit_size);
    EXPECT_EQ(basis.pixel(Hex<long>(-51, 0)), Point<long>(-3445, 170));

    auto const actual = basis.hex(Point<long>(0, -4));
    Hex<double> const expected{1.48718, -2.97436};
    EXPECT_LT(hex_norm(actual-expected), eps);

    auto const verts = basis.vertices(Hex<long>(-44, -31));
    EXPECT_EQ(verts[0], Point<long>(-3985, -1625));
    EXPECT_EQ(verts[1], Point<long>(-4019, -1605));
    EXPECT_EQ(verts[2], Point<long>(-4053, -1625));
    EXPECT_EQ(verts[3], Point<long>(-4053, -1664));
    EXPECT_EQ(verts[4], Point<long>(-4019, -1683));
    EXPECT_EQ(verts[5], Point<long>(-3985, -1664));
}
