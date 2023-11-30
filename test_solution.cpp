#include "gtest/gtest.h"

#include "solution.h"

#include <iostream>

using namespace std;

TEST(solution, test_UP) {
    Solution solution;
    solution.addDirection(UP);
    EXPECT_EQ("u", solution.getString());
}

TEST(solution, test_DOWN) {
    Solution solution;
    solution.addDirection(DOWN);
    EXPECT_EQ("d", solution.getString());
}

TEST(solution, test_LEFT) {
    Solution solution;
    solution.addDirection(LEFT);
    EXPECT_EQ("l", solution.getString());
}

TEST(solution, test_RIGHT) {
    Solution solution;
    solution.addDirection(RIGHT);
    EXPECT_EQ("r", solution.getString());
}

TEST(solution, test_length) {
    Solution solution;
    solution.addDirection(UP);
    solution.addDirection(LEFT);
    solution.addDirection(RIGHT);
    solution.addDirection(DOWN);
    EXPECT_EQ(4, solution.size());
}

TEST(solution, test_ulrd) {
    Solution solution;
    solution.addDirection(UP);
    solution.addDirection(LEFT);
    solution.addDirection(RIGHT);
    solution.addDirection(DOWN);
    EXPECT_EQ("ulrd", solution.getString());
}

TEST(solution, test_ulrdr_bytes) {
    Solution solution;
    solution.addDirection(UP);
    solution.addDirection(LEFT);
    solution.addDirection(RIGHT);
    solution.addDirection(DOWN);
    solution.addDirection(RIGHT);
    EXPECT_EQ(2, solution.bytes());
}

TEST(solution, test_ulrdr) {
    Solution solution;
    solution.addDirection(UP);
    solution.addDirection(LEFT);
    solution.addDirection(RIGHT);
    solution.addDirection(DOWN);
    solution.addDirection(RIGHT);
    EXPECT_EQ("ulrdr", solution.getString());
}
