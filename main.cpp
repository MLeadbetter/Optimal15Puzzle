#include <iostream>
#include <fstream>
#include <array>
#include <cmath>
#include <queue>
#include <memory>
#include <unordered_set>
#include <bitset>
#include <deque>
#include <thread>
#include <chrono>

#include "bucket_queue.h"
#include "solver.h"
#include "uint64_hash_set.h"
#include "solution.h"
#include "grid.h"
#include "lower_bound_tables.h"

using namespace std;

LowerBoundTables lbt;

struct Node {
    Grid grid;
    Solution solution;
};

struct SolveInfo {
    MinBucketStack<Node> queue;
    Uint64HashSet seen; // TODO: Not valid! This NEEDS to be a map.
    std::unordered_map<uint64_t, std::string> endSolutions;
    unsigned int upperBound = numeric_limits<int>::max();
    std::string bestSolution;
};

unsigned int nodesExplored = 0;

bool moveNode(SolveInfo &info, Node node) {
    nodesExplored++;
    uint64_t grid = node.grid.getGrid();
    if(info.seen.contains(grid)) return false;
    unsigned int cost = node.grid.lower_bound();
    //if(!info.endSolutions.contains(grid)) cost = max(cost, 22+1);
    if(cost == 0) {
        cout << node.solution.getString() << endl;
        cout << "Explored: " << nodesExplored;
        exit(0);
        return true;
    }
    cost += node.solution.length();
    if(info.endSolutions.contains(grid)) {
        string endSolution = info.endSolutions[grid];
        unsigned int solutionLength = node.solution.length() + endSolution.length();
        if(info.upperBound > solutionLength) {
            cout << "Improved upper bound to: " << solutionLength << endl;
            info.upperBound = solutionLength;
            info.bestSolution = node.solution.getString() + endSolution;
            cout << info.bestSolution << endl;
            cout << cost << ", " << info.upperBound << ", " << node.grid.lower_bound() << ", " << node.solution.length() <<  endl;
            if(cost >= info.upperBound) {
                cout << node.solution.getString() << endl;
                cout << "Explored: " << nodesExplored << endl;
                exit(0);
                return true;
            }
        }
    } else if(cost < info.upperBound) {
        info.queue.add(cost, node);
        info.seen.insert(node.grid.getGrid());
    }
    return false;
}

void bucketSolve(Grid grid) {
    static constexpr unsigned int BACK_MOVES = 22;
    Node node;
    node.grid = grid;
    unsigned int solutionMoves = node.solution.length() + node.grid.lower_bound();
    unsigned int longestExploration = node.solution.length();
    SolveInfo info;
    BackwardsSolver solver;
    info.endSolutions = solver.solveFromEnd(BACK_MOVES);
    info.queue.add(solutionMoves, node);
    bool done = false;
    while(!done && !info.queue.empty()) {
        node = info.queue.top();
        unsigned int estimatedMoves = node.solution.length() + node.grid.lower_bound();
        if(estimatedMoves >= info.upperBound) {
            cout << "Lower bound met upper bound." << endl;
            break;
        }
        if(estimatedMoves > solutionMoves) {
            cout << "Checking for " << estimatedMoves << " move length solutions." << endl;
            solutionMoves = estimatedMoves;
        }
        if(node.solution.length() > longestExploration) {
            cout << "Furthest depth explored so far: " << node.solution.length() << endl;
            longestExploration = node.solution.length();
        }
        info.queue.pop();
        if(info.endSolutions.contains(grid.getGrid()) && info.endSolutions[grid.getGrid()].length() + node.solution.length() >= static_cast<unsigned int>(info.upperBound))
            continue;
        if(!info.endSolutions.contains(grid.getGrid()) && static_cast<unsigned int>(info.upperBound) <= node.solution.length() + BACK_MOVES)
            continue;
        int blankLoc = node.grid.getBlankLoc();

        if(node.grid.upValid(blankLoc))
        {
            Node node2(node);
            node2.grid.moveUp(blankLoc);
            node2.solution.addDirection(UP);// += 'u';
            if(moveNode(info, node2)) break;
        }
        if(node.grid.downValid(blankLoc))
        {
            Node node2(node);
            node2.grid.moveDown(blankLoc);
            node2.solution.addDirection(DOWN);// += 'd';
            if(moveNode(info, node2)) break;
        }
        if(node.grid.leftValid(blankLoc))
        {
            Node node2(node);
            node2.grid.moveLeft(blankLoc);
            node2.solution.addDirection(LEFT);// += 'l';
            if(moveNode(info, node2)) break;
        }
        if(node.grid.rightValid(blankLoc))
        {
            Node node2(node);
            node2.grid.moveRight(blankLoc);
            node2.solution.addDirection(RIGHT);// += 'r';
            if(moveNode(info, node2)) break;
        }
    }
    cout << "Best: " << info.bestSolution << endl;
    cout << "In " << info.bestSolution.length() << " moves." << endl;
    cout << "Explored: " << nodesExplored << endl;
    exit(0);
}

void solve_sample1() {
    Grid grid;
    array<int, 16> numbers = {11, 13, 12,  7,
                               0, 15,  8,  9,
                              10, 14,  3,  2,
                               1,  6,  4,  5};
    grid.set(numbers);
    bucketSolve(grid);
}

void solve_sample2() {
    Grid grid;
    array<int, 16> numbers = { 0, 13,  8,  6,
                               9, 11,  2,  1,
                               3,  4, 10,  7,
                              15,  5, 14, 12};
    grid.set(numbers);
    bucketSolve(grid);
}

void solve_sample3() {
    Grid grid;
    array<int, 16> numbers = {14,  4,  0,  6,
                               1,  5, 13, 11,
                               7,  3, 12, 15,
                              10,  2,  9,  8};
    grid.set(numbers);
    bucketSolve(grid);
}

void solve_sample4() {
    Grid grid;
    array<int, 16> numbers = { 3,  4, 14, 13,
                              10, 11,  5,  7,
                               8,  0,  6,  1,
                              15,  2, 12,  9};
    grid.set(numbers);
    bucketSolve(grid);
}

void solve_sample5() {
    Grid grid;
    array<int, 16> numbers = { 0,  2, 12, 11,
                               6,  1, 10, 15,
                               5, 13,  8,  9,
                               7,  3,  4, 14};
    grid.set(numbers);
    bucketSolve(grid);
}

void solve_worst1() {
    Grid grid;
    array<int, 16> numbers = { 0, 15, 14, 13,
                              12, 11, 10,  9,
                               8,  7,  6,  5,
                               4,  3,  2,  1};
    grid.set(numbers);
    bucketSolve(grid);
}

void solve_worst2() {
    Grid grid;
    array<int, 16> numbers = { 0, 14, 15, 13,
                               8, 11, 10,  5,
                              12,  7,  6,  9,
                               4,  2,  3,  1};
    grid.set(numbers);
    bucketSolve(grid);
}

void solve_last_layers() {
    Grid grid;
    array<int, 16> numbers = { 1,  2,  3,  4,
                               5,  6,  7,  8,
                              10,  0, 14, 13,
                              12, 11,  9, 15};
    grid.set(numbers);
    bucketSolve(grid);
}

void solve_last_layers2() {
    Grid grid;
    array<int, 16> numbers = { 1,  2,  3,  4,
                               5,  6,  7,  8,
                              11, 15, 13,  0,
                              14, 10, 12,  9};
    grid.set(numbers);
    bucketSolve(grid);
}

void solve_last_layers3() {
    Grid grid;
    array<int, 16> numbers = { 1,  2,  3,  4,
                               5,  6,  7,  8,
                               9, 13,  0, 15,
                              10, 14, 11, 12};
    grid.set(numbers);
    bucketSolve(grid);
}

void solve_last_layers4() {
    Grid grid;
    array<int, 16> numbers = { 1,  2,  3,  4,
                               5,  6,  7,  8,
                              14, 10,  0, 13,
                              11, 12,  9, 15};
    grid.set(numbers);
    bucketSolve(grid);
}

void solve_last_layers5() {
    Grid grid;
    array<int, 16> numbers = { 1,  2,  3,  4,
                               5,  6,  7,  8,
                              15, 11,  0, 10,
                              14, 13, 12,  9};
    grid.set(numbers);
    bucketSolve(grid);
}

void numberphile() {
    Grid grid;
    array<int, 16> numbers = { 1,  2,  3,  4,
                              12, 13, 14,  5,
                              11,  0, 15,  6,
                              10,  9,  8,  7};
    grid.set(numbers);
    bucketSolve(grid);
}

int main() {
    solve_worst2();
    //solve_last_layers5();
    //solve_sample1();
    //numberphile();

    exit(0);
}
