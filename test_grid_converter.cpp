#include "gtest/gtest.h"

#include "grid.h"

using namespace std;

TEST(grid_converter, test_initial) {
    Grid grid;
    array<int, 16> numbers = { 1,  2,  3,  4,
                               5,  6,  7,  8,
                               9, 10, 11, 12,
                              13, 14, 15,  0 };
    grid.set(numbers);
    EXPECT_EQ(0xedcba9876543210f, grid.getTileLocations());
}

TEST(grid_converter, test_move_up_zero_location) {
    Grid grid;
    array<int, 16> numbers = { 1,  2,  3,  4,
                               5,  6,  7,  8,
                               9, 10, 11, 12,
                              13, 14, 15,  0 };
    grid.set(numbers);
    grid.moveRight(15);
    EXPECT_EQ(0xeULL, grid.getTileLocations() & 0xfULL);
}

TEST(grid_converter, test_move_up) {
    Grid grid;
    array<int, 16> numbers = { 1,  2,  3,  4,
                               5,  6,  7,  8,
                               9, 10, 11, 12,
                              13, 14, 15,  0 };
    grid.set(numbers);
    grid.moveRight(15);
    EXPECT_EQ(0xfdcba9876543210e, grid.getTileLocations());
}
