#include "gtest/gtest.h"
#include "tess.hpp"
#include "utility.hpp"
#include <vector>

using namespace tess;
using long_hex = hex<long>;

TEST(LineTest, LengthZeroStartZero) {
    std::vector<ihex> tiles;
    line(ihex::zero, ihex::zero, std::back_inserter(tiles));
    ASSERT_EQ(tiles.size(), 1);
    EXPECT_EQ(tiles[0], ihex::zero);
}

TEST(LineTest, LengthZeroStartPositive) {
    hex start{10, 97};
    std::vector<ihex> tiles;
    line(start, start, std::back_inserter(tiles));
    ASSERT_EQ(tiles.size(), 1);
    EXPECT_EQ(tiles[0], start);
}

TEST(LineTest, LengthOneStartNegativeAlongQAxis) {
    long_hex start{-51, -4};
    auto end = start + long_hex::forward_down;

    std::vector<long_hex> tiles;
    line(start, end, std::back_inserter(tiles));
    ASSERT_EQ(tiles.size(), 2);
    EXPECT_EQ(tiles[0], start);
    EXPECT_EQ(tiles[1], end);
}

TEST(LineTest, StartComponentsOppositeSignAndAlongRAxis) {
    int const length = 8;
    hex start{-86, 51};
    auto end = start - hex(0, length);

    std::vector<ihex> tiles;
    line(start, end, std::back_inserter(tiles));
    ASSERT_EQ(tiles.size(), length+1);
    EXPECT_EQ(tiles[0], start);
    EXPECT_EQ(tiles[5], start - hex(0, 5));
    EXPECT_EQ(tiles[length], end);
}

TEST(LineTest, StartOneComponentZeroOtherPositiveAlongSAxis) {
    short const length = 16;
    hex const start{34, 0};
    auto const end = start + hex{-16, 16};

    std::vector<ihex> tiles;
    line(start, end, std::back_inserter(tiles));
    ASSERT_EQ(tiles.size(), length+1);
    EXPECT_EQ(tiles[0], start);
    EXPECT_EQ(tiles[4], start + hex(-4, 4));
    EXPECT_EQ(tiles[length], end);
}

TEST(LineTest, RCrossNegativeS) {
    int const length = 26;
    hex const start{-28, 95};
    auto const end = start + hex(1, 25);

    std::vector<ihex> tiles;
    line(start, end, std::back_inserter(tiles));
    ASSERT_EQ(tiles.size(), length+1);
    EXPECT_EQ(tiles[0], start);
    EXPECT_EQ(tiles[11], hex(-28, 106));
    EXPECT_EQ(tiles[length], end);
}

TEST(LineTest, NegativeQCrossNegativeS) {
    long const length = 94;
    long_hex const start{-30, -57};
    auto const end = start + long_hex(-27, 94);

    std::vector<long_hex> tiles;
    line(start, end, std::back_inserter(tiles));
    ASSERT_EQ(tiles.size(), length+1);
    EXPECT_EQ(tiles[0], start);
    EXPECT_EQ(tiles[53], long_hex(-45, -4));
    EXPECT_EQ(tiles[length], end);
}

TEST(LineTest, QCrossR) {
    int const length = 374;
    hex const start{-9520, -1552};
    auto const end = start + hex(278, 96);

    std::vector<ihex> tiles;
    line(start, end, std::back_inserter(tiles));
    ASSERT_EQ(tiles.size(), length+1);
    EXPECT_EQ(tiles[0], start);
    EXPECT_EQ(tiles[337], hex(-9270, -1465));
    EXPECT_EQ(tiles[length], end);
}

TEST(LineTest, NegativeQCrossNegativeR) {
    int const length = 6128;
    hex const start{14634, 16337};
    auto const end = start + hex(-2120, -4008);

    std::vector<ihex> tiles;
    line(start, end, std::back_inserter(tiles));
    ASSERT_EQ(tiles.size(), length+1);
    EXPECT_EQ(tiles[0], start);
    EXPECT_EQ(tiles[4908], hex(12936, 13127));
    EXPECT_EQ(tiles[length], end);
}

TEST(LineTest, QCrossS) {
    long const length = 40249;
    long_hex const start{37631, 76297};
    auto const end = start + long_hex(12452, -length);

    std::vector<long_hex> tiles;
    line(start, end, std::back_inserter(tiles));
    ASSERT_EQ(tiles.size(), length+1);
    EXPECT_EQ(tiles[0], start);
    EXPECT_EQ(tiles[8326], long_hex(40207, 67971));
    EXPECT_EQ(tiles[length], end);
}

TEST(LineTest, NegativeRCrossS) {
    int const length = 4;
    hex const start{-4070, -9515};
    auto const end = start + hex(-2, -2);

    std::vector<ihex> tiles;
    line(start, end, std::back_inserter(tiles));
    ASSERT_EQ(tiles.size(), length+1);
    EXPECT_EQ(tiles[0], start);
    EXPECT_EQ(tiles[2], hex(-4071, -9516));
    EXPECT_EQ(tiles[length], end);
}
