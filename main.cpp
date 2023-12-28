#include <iostream>

#include "solution.h"
#include "grid.h"
#include "lower_bound_tables.h"
#include "fsm.h"

using namespace std;

LowerBoundTables lbt;

/*
 * Manhatten distance
 *
 * Linear conflict (two tiles in the same row or column in the wrong order = manhatten score +2)
 *
 * Last move (last move needs to be 15 tile left or 8 tile up. So if 15 tile is not on the right +1 and if the 8 tile is not on the bottom +1) NOT COMPATIBLE WITH BIDIRECTION SEARCH
 */

struct TreeNode {
    Grid grid;
    Solution solution;
    unsigned int state;
};

unsigned int nodesExplored = 0;

void writeSolution(TreeNode node) noexcept {
    cout << "Solution: " << node.solution.getString() << endl;
    cout << "Moves: " << node.solution.length() << endl;
    cout << "Nodes explored: " << nodesExplored << endl;
}

bool checkNode(const TreeNode &node, stack<TreeNode> &nodes, const unsigned int threshold) noexcept {
    nodesExplored++;
    if(node.grid.manhattan() == 0) {
        writeSolution(node);
        return true;
    }
    if(node.solution.length() + node.grid.lower_bound() <= threshold) nodes.push(node);
    return false;
}

void ida(Grid grid) noexcept {
    nodesExplored = 0;
    FSM fsm;
    std::vector<State> fsmvec = fsm.loadFSM("fsm24");
    unsigned int threshold = grid.parity() | 2;
    bool done = false;
    TreeNode first;
    first.grid = grid;
    first.state = 0;
    while(!done) {
        cout << "Threshold: " << threshold << endl;
        std::stack<TreeNode> nodes;
        nodes.push(first);
        while(!nodes.empty()) {
            TreeNode node = nodes.top();
            nodes.pop();
            int blankLoc = node.grid.getBlankLoc();
            if(fsmvec[node.state].up != -1 && node.grid.upValid(blankLoc))
            {
                TreeNode node2(node);
                node2.grid.moveUp(blankLoc);
                node2.solution.addDirection(UP);// += 'u';
                node2.state = fsmvec[node.state].up;
                if(checkNode(node2, nodes, threshold)) {
                    done = true;
                    break;
                }
            }
            if(fsmvec[node.state].down != -1 && node.grid.downValid(blankLoc))
            {
                TreeNode node2(node);
                node2.grid.moveDown(blankLoc);
                node2.solution.addDirection(DOWN);// += 'd';
                node2.state = fsmvec[node.state].down;
                if(checkNode(node2, nodes, threshold)) {
                    done = true;
                    break;
                }
            }
            if(fsmvec[node.state].left != -1 && node.grid.leftValid(blankLoc))
            {
                TreeNode node2(node);
                node2.grid.moveLeft(blankLoc);
                node2.solution.addDirection(LEFT);// += 'l';
                node2.state = fsmvec[node.state].left;
                if(checkNode(node2, nodes, threshold)) {
                    done = true;
                    break;
                }
            }
            if(fsmvec[node.state].right != -1 && node.grid.rightValid(blankLoc))
            {
                TreeNode node2(node);
                node2.grid.moveRight(blankLoc);
                node2.solution.addDirection(RIGHT);// += 'r';
                node2.state = fsmvec[node.state].right;
                if(checkNode(node2, nodes, threshold)) {
                    done = true;
                    break;
                }
            }
        }
        threshold += 2;
    }

}

void solve_sample1() {
    Grid grid;
    array<int, 16> numbers = {11, 13, 12,  7,
                               0, 15,  8,  9,
                              10, 14,  3,  2,
                               1,  6,  4,  5};
    grid.set(numbers);
    ida(grid);
}

void solve_sample2() {
    Grid grid;
    array<int, 16> numbers = { 0, 13,  8,  6,
                               9, 11,  2,  1,
                               3,  4, 10,  7,
                              15,  5, 14, 12};
    grid.set(numbers);
    ida(grid);
}

void solve_sample3() {
    Grid grid;
    array<int, 16> numbers = {14,  4,  0,  6,
                               1,  5, 13, 11,
                               7,  3, 12, 15,
                              10,  2,  9,  8};
    grid.set(numbers);
    ida(grid);
}

void solve_sample4() {
    Grid grid;
    array<int, 16> numbers = { 3,  4, 14, 13,
                              10, 11,  5,  7,
                               8,  0,  6,  1,
                              15,  2, 12,  9};
    grid.set(numbers);
    ida(grid);
}

void solve_sample5() {
    Grid grid;
    array<int, 16> numbers = { 0,  2, 12, 11,
                               6,  1, 10, 15,
                               5, 13,  8,  9,
                               7,  3,  4, 14};
    grid.set(numbers);
    ida(grid);
}

void solve_worst1() {
    Grid grid;
    array<int, 16> numbers = { 0, 15, 14, 13,
                              12, 11, 10,  9,
                               8,  7,  6,  5,
                               4,  3,  2,  1};
    grid.set(numbers);
    ida(grid);
}

void solve_worst2() {
    Grid grid;
    array<int, 16> numbers = { 0, 14, 15, 13,
                               8, 11, 10,  5,
                              12,  7,  6,  9,
                               4,  2,  3,  1};
    grid.set(numbers);
    ida(grid);
}

void solve_last_layers() {
    Grid grid;
    array<int, 16> numbers = { 1,  2,  3,  4,
                               5,  6,  7,  8,
                              10,  0, 14, 13,
                              12, 11,  9, 15};
    grid.set(numbers);
    ida(grid);
}

void solve_last_layers2() {
    Grid grid;
    array<int, 16> numbers = { 1,  2,  3,  4,
                               5,  6,  7,  8,
                              11, 15, 13,  0,
                              14, 10, 12,  9};
    grid.set(numbers);
    ida(grid);
}

void solve_last_layers3() {
    Grid grid;
    array<int, 16> numbers = { 1,  2,  3,  4,
                               5,  6,  7,  8,
                               9, 13,  0, 15,
                              10, 14, 11, 12};
    grid.set(numbers);
    ida(grid);
}

void solve_last_layers4() {
    Grid grid;
    array<int, 16> numbers = { 1,  2,  3,  4,
                               5,  6,  7,  8,
                              14, 10,  0, 13,
                              11, 12,  9, 15};
    grid.set(numbers);
    ida(grid);
}

void solve_last_layers5() {
    Grid grid;
    array<int, 16> numbers = { 1,  2,  3,  4,
                               5,  6,  7,  8,
                              15, 11,  0, 10,
                              14, 13, 12,  9};
    grid.set(numbers);
    ida(grid);
}

void numberphile() {
    Grid grid;
    array<int, 16> numbers = { 1,  2,  3,  4,
                              12, 13, 14,  5,
                              11,  0, 15,  6,
                              10,  9,  8,  7};
    grid.set(numbers);
    ida(grid);
}

#include <chrono>


int main() {
    /*BackwardsSolver solver;
    std::unordered_set<std::string> redundant = solver.findRedundant(10);
    FSM fsm;
    std::vector<State> fsmvec = fsm.generateFSM(redundant);
    cout << fsmvec.size() << endl;*/
    /*Grid grid;
    array<int, 16> numbers = { 1,  2,  3,  4,
                              5,  6,  7,  8,
                              9, 10, 11, 12,
                              13, 14, 15,  0};
    auto start_time = std::chrono::high_resolution_clock::now();

    volatile int a = 0;
    for(unsigned int i = 0; i < 100000000; i++) {
        grid.set(numbers);
        grid.set(numbers);
        grid.set(numbers);
        grid.set(numbers);
        a += i;
    }
    cout << a << endl;

    auto end_time = std::chrono::high_resolution_clock::now();

    // Calculate the duration in milliseconds
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    // Output the result
    std::cout << "Time taken: " << duration.count() << " milliseconds" << std::endl;


    return 0;*/

    //solve_worst2();
    //solve_last_layers5();
    solve_sample1();
    //solve_sample5();
    //numberphile();

    exit(0);
}
