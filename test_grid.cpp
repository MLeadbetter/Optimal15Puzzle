#include "gtest/gtest.h"

#include "grid.h"

using namespace std;

TEST(grid, test_get_set) {
    Grid grid;
    grid.set(5, 1);
    EXPECT_EQ(5, grid.valAt(1));
}

class GridTest : public ::testing::Test {
protected:
    Grid grid;
    virtual void SetUp() {
        array<int, 16> numbers = {11, 13, 12,  7,
                                   0, 15,  8,  9,
                                  10, 14,  3,  2,
                                   1,  6,  4,  5};
        grid.set(numbers);
    }
};

TEST_F(GridTest, test_find) {
    EXPECT_EQ(9, grid.find(14));
}

TEST_F(GridTest, test_find_zero) {
    EXPECT_EQ(4, grid.find(0));
}

TEST_F(GridTest, test_find_first) {
    EXPECT_EQ(0, grid.find(11));
}

TEST_F(GridTest, test_find_last) {
    EXPECT_EQ(15, grid.find(5));
}

TEST_F(GridTest, test_move_up) {
    grid.moveUp(grid.find(0));
    EXPECT_EQ(10, grid.valAt(4));
}

TEST_F(GridTest, test_move_zero_loc) {
    grid.moveUp(grid.find(0));
    EXPECT_EQ(8, grid.find(0));
}

TEST_F(GridTest, test_move_down) {
    grid.moveDown(grid.find(0));
    EXPECT_EQ(11, grid.valAt(4));
}

TEST_F(GridTest, test_move_left) {
    grid.moveLeft(grid.find(0));
    EXPECT_EQ(15, grid.valAt(4));
}

TEST_F(GridTest, test_move_right) {
    grid.moveLeft(grid.find(0));
    grid.moveRight(grid.find(0));
    EXPECT_EQ(0, grid.valAt(4));
}

TEST_F(GridTest, test_up_valid_true) {
    EXPECT_TRUE(grid.upValid(grid.find(0)));
}

TEST_F(GridTest, test_up_valid_false) {
    grid.moveUp(grid.find(0));
    grid.moveUp(grid.find(0));
    EXPECT_FALSE(grid.upValid(grid.find(0)));
}

TEST_F(GridTest, test_down_valid_true) {
    EXPECT_TRUE(grid.downValid(grid.find(0)));
}

TEST_F(GridTest, test_down_valid_false) {
    grid.moveDown(grid.find(0));
    EXPECT_FALSE(grid.downValid(grid.find(0)));
}

TEST_F(GridTest, test_left_valid_true) {
    EXPECT_TRUE(grid.leftValid(grid.find(0)));
}

TEST_F(GridTest, test_left_valid_false) {
    grid.moveLeft(grid.find(0));
    grid.moveLeft(grid.find(0));
    grid.moveLeft(grid.find(0));
    EXPECT_FALSE(grid.leftValid(grid.find(0)));
}

TEST_F(GridTest, test_right_valid_true) {
    grid.moveLeft(grid.find(0));
    EXPECT_TRUE(grid.rightValid(grid.find(0)));
}

TEST_F(GridTest, test_right_valid_false) {
    EXPECT_FALSE(grid.rightValid(grid.find(0)));
}

TEST_F(GridTest, test_manhattan) {
    EXPECT_EQ(43, grid.manhattan());
}

class GridTestSolved : public ::testing::Test {
protected:
    Grid grid;
    virtual void SetUp() {
        array<int, 16> numbers = { 1,  2,  3,  4,
                                   5,  6,  7,  8,
                                   9, 10, 11, 12,
                                  13, 14, 15,  0};
        grid.set(numbers);
    }
};

TEST_F(GridTestSolved, test_manhattan) {
    EXPECT_EQ(0, grid.manhattan());
}

TEST_F(GridTestSolved, test_up_valid) {
    EXPECT_FALSE(grid.upValid(grid.find(0)));
}

TEST_F(GridTestSolved, test_down_valid) {
    EXPECT_TRUE(grid.downValid(grid.find(0)));
}

TEST_F(GridTestSolved, test_left_valid) {
    EXPECT_FALSE(grid.leftValid(grid.find(0)));
}

TEST_F(GridTestSolved, test_right_valid) {
    EXPECT_TRUE(grid.rightValid(grid.find(0)));
}

class GridTestTopRight : public ::testing::Test {
protected:
    Grid grid;
    virtual void SetUp() {
        array<int, 16> numbers = { 1,  2,  3,  0,
                                   5,  6,  7,  8,
                                   9, 10, 11, 12,
                                  13, 14, 15,  4};
        grid.set(numbers);
    }
};

TEST_F(GridTestTopRight, test_up_valid) {
    EXPECT_TRUE(grid.upValid(grid.find(0)));
}

TEST_F(GridTestTopRight, test_down_valid) {
    EXPECT_FALSE(grid.downValid(grid.find(0)));
}

TEST_F(GridTestTopRight, test_left_valid) {
    EXPECT_FALSE(grid.leftValid(grid.find(0)));
}

TEST_F(GridTestTopRight, test_right_valid) {
    EXPECT_TRUE(grid.rightValid(grid.find(0)));
}

TEST_F(GridTestTopRight, test_manhattan_up) {
    grid.moveUp(grid.find(0));
    EXPECT_EQ(4, grid.manhattan());
}

TEST_F(GridTestTopRight, test_manhattan_right) {
    grid.moveRight(grid.find(0));
    EXPECT_EQ(4, grid.manhattan());
}

class GridTestTopLeft : public ::testing::Test {
protected:
    Grid grid;
    virtual void SetUp() {
        array<int, 16> numbers = { 0,  2,  3,  4,
                                   5,  6,  7,  8,
                                   9, 10, 11, 12,
                                  13, 14, 15,  1};
        grid.set(numbers);
    }
};

TEST_F(GridTestTopLeft, test_up_valid) {
    EXPECT_TRUE(grid.upValid(grid.find(0)));
}

TEST_F(GridTestTopLeft, test_down_valid) {
    EXPECT_FALSE(grid.downValid(grid.find(0)));
}

TEST_F(GridTestTopLeft, test_left_valid) {
    EXPECT_TRUE(grid.leftValid(grid.find(0)));
}

TEST_F(GridTestTopLeft, test_right_valid) {
    EXPECT_FALSE(grid.rightValid(grid.find(0)));
}

class GridTestBottomLeft : public ::testing::Test {
protected:
    Grid grid;
    virtual void SetUp() {
        array<int, 16> numbers = { 1,  2,  3,  4,
                                   5,  6,  7,  8,
                                   9, 10, 11, 12,
                                   0, 14, 15, 13};
        grid.set(numbers);
    }
};

TEST_F(GridTestBottomLeft, test_up_valid) {
    EXPECT_FALSE(grid.upValid(grid.find(0)));
}

TEST_F(GridTestBottomLeft, test_down_valid) {
    EXPECT_TRUE(grid.downValid(grid.find(0)));
}

TEST_F(GridTestBottomLeft, test_left_valid) {
    EXPECT_TRUE(grid.leftValid(grid.find(0)));
}

TEST_F(GridTestBottomLeft, test_right_valid) {
    EXPECT_FALSE(grid.rightValid(grid.find(0)));
}

class GridTestSample2 : public ::testing::Test {
protected:
    Grid grid;
    virtual void SetUp() {
        array<int, 16> numbers = {10, 11,  3,  7,
                                   1,  4, 13,  8,
                                   6,  2, 12,  9,
                                   0, 14, 15,  5};
        grid.set(numbers);
    }
};

TEST_F(GridTestSample2, test_manhattan) {
    EXPECT_EQ(29, grid.manhattan());
}

TEST_F(GridTestSample2, test_manhattan_move_down) {
    grid.moveDown(grid.find(0));
    EXPECT_EQ(30, grid.manhattan());
}

TEST_F(GridTestSample2, test_manhattan_move_left) {
    grid.moveLeft(grid.find(0));
    EXPECT_EQ(30, grid.manhattan());
}
