#ifndef SOLVER_H
#define SOLVER_H

#include <cstring>
#include <string>
#include <unordered_map>
#include <unordered_set>

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

    std::unordered_set<std::string> findRedundant(unsigned int movesFromEnd) {
        std::unordered_set<std::string> redundant;
        for(unsigned int i = 0; i < 16; i++) {
            std::cout << "0 at " << i << std::endl;
            std::array<int, 16> solved = { 1,  2,  3,  4,
                                           5,  6,  7,  8,
                                           9, 10, 11, 12,
                                          13, 14, 15,  0};
            std::swap(solved[i], solved[15]);
            findRedundant(movesFromEnd, solved, redundant);
        }
        return redundant;
    }

private:
    void findRedundant(unsigned int movesFromEnd, std::array<int, 16> solved, std::unordered_set<std::string> &redundant) {
        Grid grid;
        grid.set(solved);
        std::unordered_map<uint64_t, std::string> seen;
        seen[grid.getGrid()] = std::string("");
        std::unordered_map<uint64_t, std::string> moveFrom;
        moveFrom[grid.getGrid()] = std::string("");
        unsigned int prev = 0;
        for(unsigned int i = 0; i < movesFromEnd; i++) {
            std::unordered_map<uint64_t, std::string> solutions = recordRedundant(moveFrom, seen, redundant);
            seen.insert(solutions.begin(), solutions.end());
            moveFrom = solutions;
            std::cout << i+1 << ": " << redundant.size()-prev << " no-nos" << std::endl;
            prev = redundant.size();
        }
        std::cout << "Solved backwards for " << seen.size() << " positions." << std::endl;
    }

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

    std::unordered_map<uint64_t, std::string> recordRedundant(std::unordered_map<uint64_t, std::string> &moveFrom, std::unordered_map<uint64_t, std::string> &seen, std::unordered_set<std::string> &redundant) {
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
                    solutions[grid2.getGrid()] = solution + 'u';
                else if(!superceeded(solution + 'u', redundant)) redundant.insert(solution + 'u');
            }
            if(grid.downValid(blankLoc))
            {
                Grid grid2 = grid;
                grid2.moveDown(blankLoc);
                if(!seen.contains(grid2.getGrid()))
                    solutions[grid2.getGrid()] = solution + 'd';
                else if(!superceeded(solution + 'd', redundant)) redundant.insert(solution + 'd');
            }
            if(grid.leftValid(blankLoc))
            {
                Grid grid2 = grid;
                grid2.moveLeft(blankLoc);
                if(!seen.contains(grid2.getGrid()))
                    solutions[grid2.getGrid()] = solution + 'l';
                else if(!superceeded(solution + 'l', redundant)) redundant.insert(solution + 'l');
            }
            if(grid.rightValid(blankLoc))
            {
                Grid grid2 = grid;
                grid2.moveRight(blankLoc);
                if(!seen.contains(grid2.getGrid()))
                    solutions[grid2.getGrid()] = solution + 'r';
                else if(!superceeded(solution + 'r', redundant)) redundant.insert(solution + 'r');
            }
        }
        return solutions;
    }

    bool superceeded(std::string candidate, std::unordered_set<std::string> &redundant) {
        for(const std::string &comp : redundant) {
            if(candidate.find(comp) != std::string::npos)
                return true;
        }
        return false;
    }
};

#endif // SOLVER_H
