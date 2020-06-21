#include "gtest/gtest.h"
#include "hax.hpp"

using namespace hax;
using namespace std;

TEST(HexRangeTest, CenterZeroRadiusZero) {
    auto const tiles = hex_range(Hex<int>::zero, 0);
    EXPECT_EQ(tiles.size(), 1);
    EXPECT_NE(tiles.find(Hex<int>::zero), tiles.end());
    EXPECT_EQ(tiles.find(Hex<int>::left_up), tiles.end());
}

TEST(HexRangeTest, CenterZeroRadiusPositive) {
    long const radius = 9;
    auto const tiles = hex_range(Hex<long>::zero, radius);
    EXPECT_EQ(tiles.size(), 271);
    EXPECT_NE(tiles.find(Hex<long>::zero), tiles.end());
    EXPECT_NE(tiles.find(Hex<long>::right_down), tiles.end());
    EXPECT_NE(tiles.find(Hex<long>(-5, 9)), tiles.end());
    EXPECT_EQ(tiles.find(Hex<long>(10, 0)), tiles.end());
}

TEST(HexRangeTest, RadiusZeroCenterPositive) {
    Hex<long> const center{8, 84};
    auto const tiles = hex_range(center, 0l);
    EXPECT_EQ(tiles.size(), 1);
    EXPECT_NE(tiles.find(center), tiles.end());
    EXPECT_EQ(tiles.find(Hex<long>::zero), tiles.end());
}

TEST(HexRangeTest, CenterNegative) {
    short const radius = 2;
    Hex<short> const center{-17, -84};

    auto const tiles = hex_range(center, radius);
    EXPECT_EQ(tiles.size(), 19);
    EXPECT_NE(tiles.find(center), tiles.end());
    EXPECT_NE(tiles.find(center + Hex<short>(1, -2)), tiles.end());
    EXPECT_NE(tiles.find(center + Hex<short>(2, 0)), tiles.end());
    EXPECT_EQ(tiles.find(center + Hex<short>(-1, 3)), tiles.end());
}
