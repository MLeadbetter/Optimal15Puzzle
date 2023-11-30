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
#include "grid_converter.h"

extern LowerBoundTables lbt;

class Grid {
public:
    Grid() {}
    Grid(uint64_t grid) : grid(grid) {
        calculateManhattan();
        tileLocations = gridToTileLocations(grid);
    }

    int lower_bound() noexcept {
        return std::max(manhattan(), lbt.lower_bound(tileLocations));
    }

    unsigned int manhattan() const noexcept {
        return manhattanCost;
    }

    int getBlankLoc() const noexcept {
        return tileLocations & 0xfULL;
    }

    void set(std::array<int, 16> numbers) noexcept {
        for(int i = 0; i < 16; i++) {
                set(numbers[i], i);
        }
        tileLocations = gridToTileLocations(grid);
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

    unsigned int valAt(int loc) const noexcept {
        loc *= 4;
        return ((0xfULL << loc) & grid) >> loc;
    }

    void moveUp(unsigned int blankLoc) noexcept {
        manhattanCost += (valAt(blankLoc+4)-1) / 4 > blankLoc / 4 ? 1 : -1;
        move(blankLoc, blankLoc+4);
    }

    void moveDown(unsigned int blankLoc) noexcept {
        manhattanCost += (valAt(blankLoc-4)-1) / 4 < blankLoc / 4 ? 1 : -1;
        move(blankLoc, blankLoc-4);
    }

    void moveLeft(unsigned int blankLoc) noexcept {
        manhattanCost += (valAt(blankLoc+1)-1) % 4 > blankLoc % 4 ? 1 : -1;
        move(blankLoc, blankLoc+1);
    }

    void moveRight(unsigned int blankLoc) noexcept {
        manhattanCost += (valAt(blankLoc-1)-1) % 4 < blankLoc % 4 ? 1 : -1;
        move(blankLoc, blankLoc-1);
    }

    bool upValid(unsigned int blankLoc) const noexcept {
        return (blankLoc >> 2) < 3;
    }

    bool downValid(unsigned int blankLoc) const noexcept {
        return blankLoc >> 2;
    }

    bool leftValid(unsigned int blankLoc) const noexcept {
        return (blankLoc & 3) < 3;
    }

    bool rightValid(unsigned int blankLoc) const noexcept {
        return blankLoc & 3;
    }

    uint64_t getGrid() const noexcept {
        return grid;
    }

    uint64_t getTileLocations() const noexcept {
        return tileLocations;
    }

    int find(int tile) const noexcept {
        return (tileLocations >> tile) & 0xfULL;
    }

    void debugPrint() const noexcept {
        debugPrint(grid);
    }

private:
    uint64_t grid = 0;
    uint64_t tileLocations = 0;
    unsigned int manhattanCost = 0;

    constexpr static std::array<uint64_t, 16> xorlu = {0x0000000000000000ULL, 0x1111111111111111ULL, 0x2222222222222222ULL, 0x3333333333333333ULL,
                                                       0x4444444444444444ULL, 0x5555555555555555ULL, 0x6666666666666666ULL, 0x7777777777777777ULL,
                                                       0x8888888888888888ULL, 0x9999999999999999ULL, 0xaaaaaaaaaaaaaaaaULL, 0xbbbbbbbbbbbbbbbbULL,
                                                       0xccccccccccccccccULL, 0xddddddddddddddddULL, 0xeeeeeeeeeeeeeeeeULL, 0xffffffffffffffffULL};

    void move(unsigned int blankLoc, unsigned int loc) noexcept {
        tileLocations &= 0xfffffffffffffff0ULL;
        tileLocations |= static_cast<uint64_t>(loc);
        loc *= 4;
        uint64_t tile = ((grid >> loc) & 0xfULL);
        grid |= tile << blankLoc*4;
        grid &= ~(0xfULL << loc);
        tileLocations &= ~(0xfULL << tile*4);
        tileLocations |= static_cast<uint64_t>(blankLoc) << tile*4;
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
