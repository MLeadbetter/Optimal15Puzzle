#ifndef SIMPLIFIED_SOLVER_H
#define SIMPLIFIED_SOLVER_H

#include <string>
#include <unordered_map>
#include <array>
#include <vector>

#include "simplified_grid.h"

class SimplifiedBackwardsSolver {
public:
    std::unordered_map<uint64_t, unsigned int> solveFromEnd(unsigned int movesFromEnd, std::array<int, 16> solved) {
        SimplifiedGrid grid;
        grid.set(solved);
        std::unordered_map<uint64_t, unsigned int> seen;
        seen[grid.getGrid()] = 0;
        std::unordered_map<uint64_t, unsigned int> moveFrom;
        moveFrom[grid.getGrid()] = 0;
        for(unsigned int i = 0; i < movesFromEnd; i++) {
            std::unordered_map<uint64_t, unsigned int> solutions = addMoves(moveFrom, seen);
            seen.insert(solutions.begin(), solutions.end());
            moveFrom = solutions;
            if(moveFrom.empty()) {
                std::cout << "No more solutions to find back solving" << std::endl;
                std::cout << "Solved backwards " << i+1 << " moves." << std::endl;
                break;
            }
        }
        std::cout << "Solved backwards for " << seen.size() << " positions." << std::endl;
        return seen;
    }

private:
    std::unordered_map<uint64_t, unsigned int> addMoves(std::unordered_map<uint64_t, unsigned int> &moveFrom, std::unordered_map<uint64_t, unsigned int> &seen) {
        std::unordered_map<uint64_t, unsigned int> solutions;
        for(auto &it : moveFrom)
        {
            SimplifiedGrid grid(it.first);
            int blankLoc = grid.getBlankLoc();
            unsigned int solution = it.second;
            if(grid.upValid(blankLoc))
            {
                SimplifiedGrid grid2 = grid;
                grid2.moveUp(blankLoc);
                if(!seen.contains(grid2.getGrid()))
                    solutions[grid2.getGrid()] = solution + 1;
            }
            if(grid.downValid(blankLoc))
            {
                SimplifiedGrid grid2 = grid;
                grid2.moveDown(blankLoc);
                if(!seen.contains(grid2.getGrid()))
                    solutions[grid2.getGrid()] = solution + 1;
            }
            if(grid.leftValid(blankLoc))
            {
                SimplifiedGrid grid2 = grid;
                grid2.moveLeft(blankLoc);
                if(!seen.contains(grid2.getGrid()))
                    solutions[grid2.getGrid()] = solution + 1;
            }
            if(grid.rightValid(blankLoc))
            {
                SimplifiedGrid grid2 = grid;
                grid2.moveRight(blankLoc);
                if(!seen.contains(grid2.getGrid()))
                    solutions[grid2.getGrid()] = solution + 1;
            }
        }
        return solutions;
    }
};

#endif // SIMPLIFIED_SOLVER_H
