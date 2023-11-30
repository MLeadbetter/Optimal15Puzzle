#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>
#include <string>

enum Direction {UP, DOWN, LEFT, RIGHT};

class Solution {
public:
    void addDirection(Direction direction) noexcept {
        unsigned char idir = static_cast<unsigned char>(direction);
        if((_length & 3) == 0) {
            solution.push_back(0);
        }
        solution[solution.size()-1] |= idir << ((_length&3) << 1);
        _length++;
    }

    std::string getString() const noexcept {
        std::string result;
        for(unsigned char i = 0; i < _length; i++) {
            Direction idir = static_cast<Direction>((solution[i>>2] >> ((i & 3) << 1)) & 3);
            switch(idir) {
            case UP:
                result += 'u';
                break;
            case DOWN:
                result += 'd';
                break;
            case LEFT:
                result += 'l';
                break;
            case RIGHT:
                result += 'r';
                break;
            }
        }
        return result;
    }

    unsigned int length() const noexcept {
        return _length;
    }

    unsigned int size() const noexcept {
        return _length;
    }

    unsigned int bytes() const noexcept {
        return solution.size();
    }

    std::vector<unsigned char> solution;
    unsigned char _length = 0;
};

#endif // SOLUTION_H
