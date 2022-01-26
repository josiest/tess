#include "gtest/gtest.h"
#include "tess.hpp"
#include <random>

using namespace tess;
using namespace std;

TEST(LineTest, LengthZeroStartZero) {
    auto tiles = line(Hex<int>::zero, Hex<int>::zero);
    ASSERT_EQ(tiles.size(), 1);
    EXPECT_EQ(tiles[0], Hex<int>::zero);
}

TEST(LineTest, LengthZeroStartPositive) {
    Hex<short> start{10, 97};
    auto tiles = line(start, start);
    ASSERT_EQ(tiles.size(), 1);
    EXPECT_EQ(tiles[0], start);
}

TEST(LineTest, LengthOneStartNegativeAlongQAxis) {
    Hex<long> start{-51, -4};
    auto end = start + Hex<long>::forward_down;

    auto tiles = line(start, end);
    ASSERT_EQ(tiles.size(), 2);
    EXPECT_EQ(tiles[0], start);
    EXPECT_EQ(tiles[1], end);
}

TEST(LineTest, StartComponentsOppositeSignAndAlongRAxis) {
    int const length = 8;
    Hex<int> start{-86, 51};
    auto end = start - Hex<int>(0, length);

    auto tiles = line(start, end);
    ASSERT_EQ(tiles.size(), length+1);
    EXPECT_EQ(tiles[0], start);
    EXPECT_EQ(tiles[5], start - Hex<int>(0, 5));
    EXPECT_EQ(tiles[length], end);
}

TEST(LineTest, StartOneComponentZeroOtherPositiveAlongSAxis) {
    short const length = 16;
    Hex<short> const start{34, 0};
    auto const end = start + Hex<short>{-16, 16};

    auto tiles = line(start, end);
    ASSERT_EQ(tiles.size(), length+1);
    EXPECT_EQ(tiles[0], start);
    EXPECT_EQ(tiles[4], start + Hex<short>(-4, 4));
    EXPECT_EQ(tiles[length], end);
}

TEST(LineTest, RCrossNegativeS) {
    int const length = 26;
    Hex<int> const start{-28, 95};
    auto const end = start + Hex<int>(1, 25);

    auto tiles = line(start, end);
    ASSERT_EQ(tiles.size(), length+1);
    EXPECT_EQ(tiles[0], start);
    EXPECT_EQ(tiles[11], Hex<int>(-28, 106));
    EXPECT_EQ(tiles[length], end);
}

TEST(LineTest, NegativeQCrossNegativeS) {
    long const length = 94;
    Hex<long> const start{-30, -57};
    auto const end = start + Hex<long>(-27, 94);

    auto tiles = line(start, end);
    ASSERT_EQ(tiles.size(), length+1);
    EXPECT_EQ(tiles[0], start);
    EXPECT_EQ(tiles[53], Hex<long>(-45, -4));
    EXPECT_EQ(tiles[length], end);
}

TEST(LineTest, QCrossR) {
    int const length = 374;
    Hex<int> const start{-9520, -1552};
    auto const end = start + Hex<int>(278, 96);

    auto tiles = line(start, end);
    ASSERT_EQ(tiles.size(), length+1);
    EXPECT_EQ(tiles[0], start);
    EXPECT_EQ(tiles[337], Hex<int>(-9270, -1465));
    EXPECT_EQ(tiles[length], end);
}

TEST(LineTest, NegativeQCrossNegativeR) {
    int const length = 6128;
    Hex<int> const start{14634, 16337};
    auto const end = start + Hex<int>(-2120, -4008);

    auto tiles = line(start, end);
    ASSERT_EQ(tiles.size(), length+1);
    EXPECT_EQ(tiles[0], start);
    EXPECT_EQ(tiles[4908], Hex<int>(12936, 13127));
    EXPECT_EQ(tiles[length], end);
}

TEST(LineTest, QCrossS) {
    long const length = 40249;
    Hex<long> const start{37631, 76297};
    auto const end = start + Hex<long>(12452, -length);

    auto tiles = line(start, end);
    ASSERT_EQ(tiles.size(), length+1);
    EXPECT_EQ(tiles[0], start);
    EXPECT_EQ(tiles[8326], Hex<long>(40207, 67971));
    EXPECT_EQ(tiles[length], end);
}

TEST(LineTest, NegativeRCrossS) {
    int const length = 4;
    Hex<int> const start{-4070, -9515};
    auto const end = start + Hex<int>(-2, -2);

    auto tiles = line(start, end);
    ASSERT_EQ(tiles.size(), length+1);
    EXPECT_EQ(tiles[0], start);
    EXPECT_EQ(tiles[2], Hex<int>(-4071, -9516));
    EXPECT_EQ(tiles[length], end);
}
