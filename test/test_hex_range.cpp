#include "gtest/gtest.h"
#include "tess.hpp"
#include "utility.hpp"
#include <unordered_set>

using namespace tess;
using long_hex = hex<long>;

TEST(HexRangeTest, CenterZeroRadiusZero) {
    std::unordered_set<ihex> tiles;
    hex_range(ihex::zero, 0, std::inserter(tiles, std::begin(tiles)));
    EXPECT_EQ(tiles.size(), 1);
    EXPECT_NE(tiles.find(ihex::zero), tiles.end());
    EXPECT_EQ(tiles.find(ihex::left_up), tiles.end());
}

TEST(HexRangeTest, CenterZeroRadiusPositive) {
    long const radius = 9;
    std::unordered_set<long_hex> tiles;
    hex_range(long_hex::zero, radius, std::inserter(tiles, std::begin(tiles)));
    EXPECT_EQ(tiles.size(), 271);
    EXPECT_NE(tiles.find(long_hex::zero), tiles.end());
    EXPECT_NE(tiles.find(long_hex::right_down), tiles.end());
    EXPECT_NE(tiles.find(long_hex(-5, 9)), tiles.end());
    EXPECT_EQ(tiles.find(long_hex(10, 0)), tiles.end());
}

TEST(HexRangeTest, RadiusZeroCenterPositive) {
    long_hex const center{8, 84};
    std::unordered_set<long_hex> tiles;
    hex_range(center, 0l, std::inserter(tiles, std::begin(tiles)));
    EXPECT_EQ(tiles.size(), 1);
    EXPECT_NE(tiles.find(center), tiles.end());
    EXPECT_EQ(tiles.find(long_hex::zero), tiles.end());
}

TEST(HexRangeTest, CenterNegative) {
    int const radius = 2;
    hex const center{-17, -84};

    std::unordered_set<ihex> tiles;
    hex_range(center, radius, std::inserter(tiles, std::begin(tiles)));
    EXPECT_EQ(tiles.size(), 19);
    EXPECT_NE(tiles.find(center), tiles.end());
    EXPECT_NE(tiles.find(center + hex(1, -2)), tiles.end());
    EXPECT_NE(tiles.find(center + hex(2, 0)), tiles.end());
    EXPECT_EQ(tiles.find(center + hex(-1, 3)), tiles.end());
}
