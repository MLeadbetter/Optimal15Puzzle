#include "gtest/gtest.h"

#include "solver.h"

using namespace std;

TEST(solver, test_one_move) {
    BackwardsSolver solver;
    EXPECT_EQ(3, solver.solveFromEnd(1).size());
}

TEST(solver, test_two_moves) {
    BackwardsSolver solver;
    EXPECT_EQ(7, solver.solveFromEnd(2).size());
}

TEST(solver, test_solution) {
    BackwardsSolver solver;
    std::array<int, 16> unsolved = { 1,  2,  3,  4,
                                     5,  6,  7,  8,
                                     9, 10, 11, 12,
                                    13,  0, 14, 15};
    Grid grid;
    grid.set(unsolved);
    EXPECT_EQ("ll", solver.solveFromEnd(2)[grid.getGrid()]);
}

TEST(solver, test_solution_overwrite) {
    BackwardsSolver solver;
    std::array<int, 16> unsolved = { 1,  2,  3,  4,
                                     5,  6,  7,  8,
                                     9, 10, 11, 12,
                                    13,  0, 14, 15};
    Grid grid;
    grid.set(unsolved);
    EXPECT_EQ("ll", solver.solveFromEnd(4)[grid.getGrid()]);
}
