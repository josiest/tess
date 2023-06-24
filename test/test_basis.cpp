#include "gtest/gtest.h"
#include "utility.hpp"
#include "tess.hpp"

using namespace tess;
using namespace std;
 
TEST(BasisTest, SizeIsOneAndOriginZero) {
    Basis<double> const basis(int_point::zero, 1);
    EXPECT_EQ(basis.origin(), int_point::zero);
    EXPECT_EQ(basis.unit_size(), 1);
    EXPECT_EQ(basis.pixel(Hex<int>::zero), int_point::zero);
    EXPECT_LT(hex_norm(basis.hex(int_point::zero)), 0.0001);

    auto const verts = basis.vertices(Hex<int>::zero);
    EXPECT_LE(norm(verts[0]-int_point(1, 1)), 1.0);
    EXPECT_EQ(verts[1], int_point(0, 1));
    EXPECT_EQ(verts[2], int_point(-1, 1));
    EXPECT_LE(norm(verts[3]-int_point(-1, -1)), 1.0);
    EXPECT_EQ(verts[4], int_point(0, -1));
    EXPECT_LE(norm(verts[5]-int_point(1, -1)), 1.0);
}

TEST(BasisTest, SizeIsSmallAndOriginPositive) {
    double const eps = 0.001;
    basic_point<long> const origin(7, 3);
    long const unit_size = 10;

    Basis<float, long> const basis{origin, unit_size};
    EXPECT_EQ(basis.origin(), origin);
    EXPECT_EQ(basis.unit_size(), unit_size);
    EXPECT_EQ(basis.pixel(Hex<long>::zero), origin);

    auto const actual = basis.hex(basic_point<long>::zero);
    Hex<float> const expected{-0.3042, -0.2};
    EXPECT_LT(hex_norm(actual-expected), eps);

    auto const verts = basis.vertices(Hex<long>::zero);
    EXPECT_EQ(verts[0], basic_point<long>(16, 8));
    EXPECT_EQ(verts[1], basic_point<long>(7, 13));
    EXPECT_EQ(verts[2], basic_point<long>(-2, 8));
    EXPECT_EQ(verts[3], basic_point<long>(-2, -2));
    EXPECT_EQ(verts[4], basic_point<long>(7, -7));
    EXPECT_EQ(verts[5], basic_point<long>(16, -2));
}

TEST(BasisTest, SizeIsLargeAndOriginNegative) {
    double const eps = 0.0001;
    int const unit_size = 255;
    basic_point<int> const origin(-583, -570);
    Basis<long double, int> const basis{origin, unit_size, HexTop::Flat};

    EXPECT_EQ(basis.origin(), origin);
    EXPECT_EQ(basis.unit_size(), unit_size);
    EXPECT_EQ(basis.pixel(Hex<int>(10, 32)),
              basic_point<int>(3242, 15772));

    auto const actual = basis.hex(basic_point<int>(-75, -21));
    Hex<long double> const expected{1.3281l, 0.57895l};
    EXPECT_LT(hex_norm(actual-expected), eps);

    auto const verts = basis.vertices(Hex<int>::zero);
    EXPECT_EQ(verts[0], basic_point<int>(-328, -570));
    EXPECT_EQ(verts[1], basic_point<int>(-456, -349));
    EXPECT_EQ(verts[2], basic_point<int>(-711, -349));
    EXPECT_EQ(verts[3], basic_point<int>(-838, -570));
    EXPECT_EQ(verts[4], basic_point<int>(-711, -791));
    EXPECT_EQ(verts[5], basic_point<int>(-456, -791));
}

TEST(BasisTest, OriginOneComponentNegativeOtherPositive) {
    double const eps = 0.001;
    int_point const origin(-59, 61);
    int const unit_size = 21;
    Basis<float, int> const basis{origin, unit_size, HexTop::Flat};

    EXPECT_EQ(basis.origin(), origin);
    EXPECT_EQ(basis.unit_size(), unit_size);
    EXPECT_EQ(basis.pixel(Hex<int>(0, -32)), int_point(-59, -1103));

    auto const actual = basis.hex(int_point(60, 0));
    Hex<float> const expected{3.7778f, -3.566f};
    EXPECT_LT(hex_norm(actual-expected), eps);

    auto const verts = basis.vertices(Hex<int>(46, 47));
    EXPECT_EQ(verts[0], int_point(1411, 2607));
    EXPECT_EQ(verts[1], int_point(1401, 2625));
    EXPECT_EQ(verts[2], int_point(1380, 2625));
    EXPECT_EQ(verts[3], int_point(1369, 2607));
    EXPECT_EQ(verts[4], int_point(1380, 2589));
    EXPECT_EQ(verts[5], int_point(1401, 2589));
}

TEST(BasisTest, OriginOneComponentZeroOtherPositive) {
    double const eps = 0.0001;
    basic_point<long> const origin(0, 170);
    long const unit_size = 39;
    Basis<double, long> const basis{origin, unit_size, HexTop::Pointed};

    EXPECT_EQ(basis.origin(), origin);
    EXPECT_EQ(basis.unit_size(), unit_size);
    EXPECT_EQ(basis.pixel(Hex<long>(-51, 0)), basic_point<long>(-3445, 170));

    auto const actual = basis.hex(basic_point<long>(0, -4));
    Hex<double> const expected{1.48718, -2.97436};
    EXPECT_LT(hex_norm(actual-expected), eps);

    auto const verts = basis.vertices(Hex<long>(-44, -31));
    EXPECT_EQ(verts[0], basic_point<long>(-3985, -1625));
    EXPECT_EQ(verts[1], basic_point<long>(-4019, -1605));
    EXPECT_EQ(verts[2], basic_point<long>(-4053, -1625));
    EXPECT_EQ(verts[3], basic_point<long>(-4053, -1664));
    EXPECT_EQ(verts[4], basic_point<long>(-4019, -1683));
    EXPECT_EQ(verts[5], basic_point<long>(-3985, -1664));
}
