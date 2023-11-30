#ifndef GRID_H
#define GRID_H

#include <cstdint>
#include <array>
#include <limits>
#include <bit>
#include <bitset>
#include <cmath>

#include <iostream>

#include "lower_bound_tables.h"

extern LowerBoundTables lbt;

class Grid {
public:
    Grid() {}
    Grid(uint64_t grid) : grid(grid) {
        calculateManhattan();
    }

    int lower_bound() noexcept {
        if(lb != -1) return lb;
        unsigned int unused = lbt.get_first_unused_h(0);
        uint64_t simpleGridH[3];
        simpleGridH[0] = xorlu[unused];
        unused = lbt.get_first_unused_h(1);
        simpleGridH[1] = xorlu[unused];
        unused = lbt.get_first_unused_h(2);
        simpleGridH[2] = xorlu[unused];

        unused = lbt.get_first_unused_v(0);
        uint64_t simpleGridV[3];
        simpleGridV[0] = xorlu[unused];
        unused = lbt.get_first_unused_v(1);
        simpleGridV[1] = xorlu[unused];
        unused = lbt.get_first_unused_v(2);
        simpleGridV[2] = xorlu[unused];

        unused = lbt.get_first_unused_e(0);
        uint64_t simpleGridE[3];
        simpleGridE[0] = xorlu[unused];
        unused = lbt.get_first_unused_e(1);
        simpleGridE[1] = xorlu[unused];
        unused = lbt.get_first_unused_e(2);
        simpleGridE[2] = xorlu[unused];

        for(unsigned int i = 0; i < 16; i++) {
            uint64_t tile = (grid >> (i*4)) & 0xf;
            uint64_t posMask = 0xfULL << i*4;
            switch(tile) {
            case 0:
                simpleGridH[0] &= ~posMask;
                simpleGridH[1] &= ~posMask;
                simpleGridH[2] &= ~posMask;
                simpleGridV[0] &= ~posMask;
                simpleGridV[1] &= ~posMask;
                simpleGridV[2] &= ~posMask;
                simpleGridE[0] &= ~posMask;
                simpleGridE[1] &= ~posMask;
                simpleGridE[2] &= ~posMask;
                break;
            case 1:
                simpleGridH[0] &= ~posMask;
                simpleGridH[0] ^= posMask & grid;
                simpleGridV[0] &= ~posMask;
                simpleGridV[0] ^= posMask & grid;
                break;
            case 2:
                simpleGridH[0] &= ~posMask;
                simpleGridH[0] ^= posMask & grid;
                simpleGridV[1] &= ~posMask;
                simpleGridV[1] ^= posMask & grid;
                break;
            case 3:
                simpleGridH[0] &= ~posMask;
                simpleGridH[0] ^= posMask & grid;
                simpleGridV[2] &= ~posMask;
                simpleGridV[2] ^= posMask & grid;
                break;
            case 4:
                simpleGridH[0] &= ~posMask;
                simpleGridH[0] ^= posMask & grid;
                simpleGridE[1] &= ~posMask;
                simpleGridE[1] ^= posMask & grid;
                simpleGridE[2] &= ~posMask;
                simpleGridE[2] ^= posMask & grid;
                break;
            case 5:
                simpleGridH[1] &= ~posMask;
                simpleGridH[1] ^= posMask & grid;
                simpleGridV[0] &= ~posMask;
                simpleGridV[0] ^= posMask & grid;
                break;
            case 6:
                simpleGridH[1] &= ~posMask;
                simpleGridH[1] ^= posMask & grid;
                simpleGridV[1] &= ~posMask;
                simpleGridV[1] ^= posMask & grid;
                break;
            case 7:
                simpleGridH[1] &= ~posMask;
                simpleGridH[1] ^= posMask & grid;
                simpleGridV[2] &= ~posMask;
                simpleGridV[2] ^= posMask & grid;
                break;
            case 8:
                simpleGridH[1] &= ~posMask;
                simpleGridH[1] ^= posMask & grid;
                simpleGridE[1] &= ~posMask;
                simpleGridE[1] ^= posMask & grid;
                simpleGridE[2] &= ~posMask;
                simpleGridE[2] ^= posMask & grid;
                break;
            case 9:
                simpleGridH[2] &= ~posMask;
                simpleGridH[2] ^= posMask & grid;
                simpleGridV[0] &= ~posMask;
                simpleGridV[0] ^= posMask & grid;
                break;
            case 10:
                simpleGridH[2] &= ~posMask;
                simpleGridH[2] ^= posMask & grid;
                simpleGridV[1] &= ~posMask;
                simpleGridV[1] ^= posMask & grid;
                break;
            case 11:
                simpleGridH[2] &= ~posMask;
                simpleGridH[2] ^= posMask & grid;
                simpleGridV[2] &= ~posMask;
                simpleGridV[2] ^= posMask & grid;
                simpleGridE[0] &= ~posMask;
                simpleGridE[0] ^= posMask & grid;
                simpleGridE[1] &= ~posMask;
                simpleGridE[1] ^= posMask & grid;
                simpleGridE[2] &= ~posMask;
                simpleGridE[2] ^= posMask & grid;
                break;
            case 12:
                simpleGridH[2] &= ~posMask;
                simpleGridH[2] ^= posMask & grid;
                simpleGridE[0] &= ~posMask;
                simpleGridE[0] ^= posMask & grid;
                simpleGridE[1] &= ~posMask;
                simpleGridE[1] ^= posMask & grid;
                simpleGridE[2] &= ~posMask;
                simpleGridE[2] ^= posMask & grid;
                break;
            case 13:
                simpleGridV[0] &= ~posMask;
                simpleGridV[0] ^= posMask & grid;
                simpleGridE[0] &= ~posMask;
                simpleGridE[0] ^= posMask & grid;
                simpleGridE[2] &= ~posMask;
                simpleGridE[2] ^= posMask & grid;
                break;
            case 14:
                simpleGridV[1] &= ~posMask;
                simpleGridV[1] ^= posMask & grid;
                simpleGridE[0] &= ~posMask;
                simpleGridE[0] ^= posMask & grid;
                simpleGridE[2] &= ~posMask;
                simpleGridE[2] ^= posMask & grid;
                break;
            case 15:
                simpleGridV[2] &= ~posMask;
                simpleGridV[2] ^= posMask & grid;
                simpleGridE[0] &= ~posMask;
                simpleGridE[0] ^= posMask & grid;
                simpleGridE[1] &= ~posMask;
                simpleGridE[1] ^= posMask & grid;
                simpleGridE[2] &= ~posMask;
                simpleGridE[2] ^= posMask & grid;
                break;
            }
        }

        int lowerBound = manhattan();
        for(unsigned int i = 0; i < 3; i++) {
            int moves = lbt.get_moves_h(simpleGridH[i], i);
            lowerBound = std::max(lowerBound, moves);
            moves = lbt.get_moves_v(simpleGridV[i], i);
            lowerBound = std::max(lowerBound, moves);
            moves = lbt.get_moves_e(simpleGridE[i], i);
            lowerBound = std::max(lowerBound, moves);
        }

        lb = lowerBound;
        return lowerBound;
    }

    int manhattan() const noexcept {
        return manhattanCost;
    }

    int getBlankLoc() const noexcept {
        return blankLoc;
    }

    void set(std::array<int, 16> numbers) noexcept {
        for(int i = 0; i < 16; i++) {
                set(numbers[i], i);
        }
        calculateManhattan();
    }

    void set(uint64_t val, int loc) noexcept {
        loc *= 4;
        grid &= ~(0xfULL << loc);
        grid |= val << loc;
    }

    void set(uint64_t val, int x, int y) noexcept {
        int loc = (x + 4*y)*4;
        grid &= ~(0xfULL << loc);
        grid |= val << loc;
    }

    int valAt(int loc) const noexcept {
        loc *= 4;
        return ((0xfULL << loc) & grid) >> loc;
    }

    void moveUp(int blankLoc) noexcept {
        manhattanCost += (valAt(blankLoc+4)-1) / 4 > blankLoc / 4 ? 1 : -1;
        move(blankLoc, blankLoc+4);
        this->blankLoc += 4;
    }

    void moveDown(int blankLoc) noexcept {
        manhattanCost += (valAt(blankLoc-4)-1) / 4 < blankLoc / 4 ? 1 : -1;
        move(blankLoc, blankLoc-4);
        this->blankLoc -= 4;
    }

    void moveLeft(int blankLoc) noexcept {
        manhattanCost += (valAt(blankLoc+1)-1) % 4 > blankLoc % 4 ? 1 : -1;
        move(blankLoc, blankLoc+1);
        this->blankLoc += 1;
    }

    void moveRight(int blankLoc) noexcept {
        manhattanCost += (valAt(blankLoc-1)-1) % 4 < blankLoc % 4 ? 1 : -1;
        move(blankLoc, blankLoc-1);
        this->blankLoc -= 1;
    }

    bool upValid(int blankLoc) const noexcept {
        return (blankLoc >> 2) < 3;
    }

    bool downValid(int blankLoc) const noexcept {
        return blankLoc >> 2;
    }

    bool leftValid(int blankLoc) const noexcept {
        return (blankLoc & 3) < 3;
    }

    bool rightValid(int blankLoc) const noexcept {
        return blankLoc & 3;
    }

    uint64_t getGrid() const noexcept {
        return grid;
    }

    int find(int tile) const noexcept {
        // If we xor every position with the value we're looking for, only the position with the value we're looking for will be 0.
        uint64_t zeroedPos = grid ^ xorlu[tile];
        // Next we are going to fill every position that isn't fully zeros with ones. We need to use masks to prevent overflow into other positions.
        constexpr static uint64_t mask1 = 0x7777777777777777ULL; // 01110111...
        zeroedPos |= ((zeroedPos & mask1) << 1);
        constexpr static uint64_t mask2 = 0x3333333333333333ULL; // 00110011...
        zeroedPos |= ((zeroedPos & mask2) << 2);
        constexpr static uint64_t mask3 = 0xeeeeeeeeeeeeeeeeULL; // 11101110...
        zeroedPos |= ((zeroedPos & mask3) >> 1);
        constexpr static uint64_t mask4 = 0xccccccccccccccccULL; // 11001100...
        zeroedPos |= ((zeroedPos & mask4) >> 2);

        // Now all bits should be 1, except at the position we're looking for, where everything will be 0. We can now find the position by counting the digits until the first 0 and dividing by 4.
        return std::countr_one(zeroedPos) >> 2;
    }

    void debugPrint() const noexcept {
        debugPrint(grid);
    }

private:
    uint64_t grid = 0;
    unsigned int manhattanCost = 0;
    int blankLoc = 0;
    int lb = -1;

    constexpr static std::array<uint64_t, 16> xorlu = {0x0000000000000000ULL, 0x1111111111111111ULL, 0x2222222222222222ULL, 0x3333333333333333ULL,
                                                       0x4444444444444444ULL, 0x5555555555555555ULL, 0x6666666666666666ULL, 0x7777777777777777ULL,
                                                       0x8888888888888888ULL, 0x9999999999999999ULL, 0xaaaaaaaaaaaaaaaaULL, 0xbbbbbbbbbbbbbbbbULL,
                                                       0xccccccccccccccccULL, 0xddddddddddddddddULL, 0xeeeeeeeeeeeeeeeeULL, 0xffffffffffffffffULL};

    void move(int blankLoc, int loc) noexcept {
        lb = -1;
        blankLoc *= 4;
        loc *= 4;
        grid |= ((grid >> loc) & 0xfULL) << blankLoc;
        grid &= ~(0xfULL << loc);
    }

    void calculateManhattan() noexcept {
        int total = 0;
        uint64_t tempGrid = grid;
        int tile = (tempGrid & 0xfULL) - 1;
        int i = 0;
        if(tile != -1)
                total += std::abs((tile & 3) - (i & 3)) + std::abs((tile >> 2) - (i >> 2));
        for(i = 1; i < 16; i++) {
                tempGrid >>= 4;
                tile = (tempGrid & 0xfULL) - 1;
                if(tile != -1)
                total += std::abs((tile & 3) - (i & 3)) + std::abs((tile >> 2) - (i >> 2));
        }
        manhattanCost = total;
        blankLoc = find(0);
    }

    void debugPrint(uint64_t x) const noexcept {
        for(int i = 15; i >= 0; i--)
            std::cout << std::bitset<4>(x >> (i*4)) << " ";
        std::cout << std::endl;

        for(int i = 0; i < 16; i++) {
            int loc = i*4;
            int val = ((0xfULL << loc) & x) >> loc;;
            if(val < 10) std::cout << " ";
            std::cout << val << " ";
            if(i%4==3)
                std::cout << std::endl;
        }
        std::cout << manhattan() << std::endl;
        std::cout << std::endl;
    }
};

#endif // GRID_H
