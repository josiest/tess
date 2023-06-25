#include "gtest/gtest.h"
#include "utility.hpp"
#include "tess.hpp"

using namespace tess;
using long_point = point<long>;
using long_hex = hex<long>;
 
TEST(BasisTest, SizeIsOneAndOriginZero) {
    Basis<double> const basis(ipoint::zero, 1);
    EXPECT_EQ(basis.origin(), ipoint::zero);
    EXPECT_EQ(basis.unit_size(), 1);
    EXPECT_EQ(basis.nearest_pixel(ihex::zero), ipoint::zero);
    EXPECT_LT(hex_norm(basis.nearest_hex(ipoint::zero)), 0.0001);

    auto const verts = basis.vertices(ihex::zero);
    EXPECT_LE(norm(verts[0]-point(1, 1)), 1.0);
    EXPECT_EQ(verts[1], point(0, 1));
    EXPECT_EQ(verts[2], point(-1, 1));
    EXPECT_LE(norm(verts[3]-point(-1, -1)), 1.0);
    EXPECT_EQ(verts[4], point(0, -1));
    EXPECT_LE(norm(verts[5]-point(1, -1)), 1.0);
}

TEST(BasisTest, SizeIsSmallAndOriginPositive) {
    double const eps = 0.001;
    long_point const origin(7, 3);
    long const unit_size = 10;

    Basis<float, long> const basis{origin, unit_size};
    EXPECT_EQ(basis.origin(), origin);
    EXPECT_EQ(basis.unit_size(), unit_size);
    EXPECT_EQ(basis.nearest_pixel(long_hex::zero), origin);

    auto const actual = basis.nearest_hex(long_point::zero);
    fhex const expected_val{-0.3042, -0.2};
    EXPECT_LT(hex_norm(actual-expected_val), eps);

    auto const verts = basis.vertices(long_hex::zero);
    EXPECT_EQ(verts[0], long_point(16, 8));
    EXPECT_EQ(verts[1], long_point(7, 13));
    EXPECT_EQ(verts[2], long_point(-2, 8));
    EXPECT_EQ(verts[3], long_point(-2, -2));
    EXPECT_EQ(verts[4], long_point(7, -7));
    EXPECT_EQ(verts[5], long_point(16, -2));
}

TEST(BasisTest, SizeIsLargeAndOriginNegative) {
    double const eps = 0.0001;
    int const unit_size = 255;
    point const origin(-583, -570);
    Basis<long double, int> const basis{origin, unit_size, HexTop::Flat};

    EXPECT_EQ(basis.origin(), origin);
    EXPECT_EQ(basis.unit_size(), unit_size);
    EXPECT_EQ(basis.nearest_pixel(hex(10, 32)),
              point(3242, 15772));

    auto const actual = basis.nearest_hex(point(-75, -21));
    hex<long double> const expected_val{1.3281l, 0.57895l};
    EXPECT_LT(hex_norm(actual-expected_val), eps);

    auto const verts = basis.vertices(ihex::zero);
    EXPECT_EQ(verts[0], point(-328, -570));
    EXPECT_EQ(verts[1], point(-456, -349));
    EXPECT_EQ(verts[2], point(-711, -349));
    EXPECT_EQ(verts[3], point(-838, -570));
    EXPECT_EQ(verts[4], point(-711, -791));
    EXPECT_EQ(verts[5], point(-456, -791));
}

TEST(BasisTest, OriginOneComponentNegativeOtherPositive) {
    double const eps = 0.001;
    point const origin(-59, 61);
    int const unit_size = 21;
    Basis<float, int> const basis{origin, unit_size, HexTop::Flat};

    EXPECT_EQ(basis.origin(), origin);
    EXPECT_EQ(basis.unit_size(), unit_size);
    EXPECT_EQ(basis.nearest_pixel(hex(0, -32)), point(-59, -1103));

    auto const actual = basis.nearest_hex(point(60, 0));
    fhex const expected_val{3.7778f, -3.566f};
    EXPECT_LT(hex_norm(actual-expected_val), eps);

    auto const verts = basis.vertices(hex(46, 47));
    EXPECT_EQ(verts[0], point(1411, 2607));
    EXPECT_EQ(verts[1], point(1401, 2625));
    EXPECT_EQ(verts[2], point(1380, 2625));
    EXPECT_EQ(verts[3], point(1369, 2607));
    EXPECT_EQ(verts[4], point(1380, 2589));
    EXPECT_EQ(verts[5], point(1401, 2589));
}

TEST(BasisTest, OriginOneComponentZeroOtherPositive) {
    double const eps = 0.0001;
    long_point const origin(0, 170);
    long const unit_size = 39;
    Basis<double, long> const basis{origin, unit_size, HexTop::Pointed};

    EXPECT_EQ(basis.origin(), origin);
    EXPECT_EQ(basis.unit_size(), unit_size);
    EXPECT_EQ(basis.nearest_pixel(long_hex(-51, 0)), long_point(-3445, 170));

    auto const actual = basis.nearest_hex(long_point(0, -4));
    dhex const expected_val{1.48718, -2.97436};
    EXPECT_LT(hex_norm(actual-expected_val), eps);

    auto const verts = basis.vertices(long_hex(-44, -31));
    EXPECT_EQ(verts[0], long_point(-3985, -1625));
    EXPECT_EQ(verts[1], long_point(-4019, -1605));
    EXPECT_EQ(verts[2], long_point(-4053, -1625));
    EXPECT_EQ(verts[3], long_point(-4053, -1664));
    EXPECT_EQ(verts[4], long_point(-4019, -1683));
    EXPECT_EQ(verts[5], long_point(-3985, -1664));
}
