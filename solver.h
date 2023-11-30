#ifndef SOLVER_H
#define SOLVER_H

#include <string>
#include <unordered_map>

#include "grid.h"

class BackwardsSolver {
public:
    std::unordered_map<uint64_t, std::string> solveFromEnd(unsigned int movesFromEnd) {
        std::array<int, 16> solved = { 1,  2,  3,  4,
                                       5,  6,  7,  8,
                                       9, 10, 11, 12,
                                      13, 14, 15,  0};
        Grid grid;
        grid.set(solved);
        std::unordered_map<uint64_t, std::string> seen;
        seen[grid.getGrid()] = std::string("");
        std::unordered_map<uint64_t, std::string> moveFrom;
        moveFrom[grid.getGrid()] = std::string("");
        for(unsigned int i = 0; i < movesFromEnd; i++) {
            std::unordered_map<uint64_t, std::string> solutions = addMoves(moveFrom, seen);
            seen.insert(solutions.begin(), solutions.end());
            moveFrom = solutions;
            std::cout << "Solved backwards " << i+1 << " moves." << std::endl;
        }
        std::cout << "Solved backwards for " << seen.size() << " positions." << std::endl;
        return seen;
    }

private:
    std::unordered_map<uint64_t, std::string> addMoves(std::unordered_map<uint64_t, std::string> &moveFrom, std::unordered_map<uint64_t, std::string> &seen) {
        std::unordered_map<uint64_t, std::string> solutions;
        for(auto &it : moveFrom)
        {
            Grid grid(it.first);
            int blankLoc = grid.getBlankLoc();
            std::string solution = it.second;
            if(grid.upValid(blankLoc))
            {
                Grid grid2 = grid;
                grid2.moveUp(blankLoc);
                if(!seen.contains(grid2.getGrid()))
                    solutions[grid2.getGrid()] = 'd' + solution;
            }
            if(grid.downValid(blankLoc))
            {
                Grid grid2 = grid;
                grid2.moveDown(blankLoc);
                if(!seen.contains(grid2.getGrid()))
                    solutions[grid2.getGrid()] = 'u' + solution;
            }
            if(grid.leftValid(blankLoc))
            {
                Grid grid2 = grid;
                grid2.moveLeft(blankLoc);
                if(!seen.contains(grid2.getGrid()))
                    solutions[grid2.getGrid()] = 'r' + solution;
            }
            if(grid.rightValid(blankLoc))
            {
                Grid grid2 = grid;
                grid2.moveRight(blankLoc);
                if(!seen.contains(grid2.getGrid()))
                    solutions[grid2.getGrid()] = 'l' + solution;
            }
        }
        return solutions;
    }
};

#endif // SOLVER_H
