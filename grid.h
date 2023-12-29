#ifndef GRID_H
#define GRID_H

#include <cstdint>
#include <array>
#include <limits>
#include <bit>
#include <bitset>
#include <cmath>
#include <immintrin.h>

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

    int lower_bound() const noexcept {
        return std::max(manhattan(), lbt.lower_bound(tileLocations));
    }


    /* a^b
     * 0 0 0
     * 0 1 1
     * 1 0 1
     * 1 1 0
     *
     * ~(a^b)
     * 0 0 1
     * 0 1 0
     * 1 0 0
     * 1 1 1
     *
     * (~a)^b
     * 0 0 1
     * 0 1 0
     * 1 0 0
     * 1 1 1
     */
    unsigned int cornerHeuristic() const noexcept {
        uint64_t correct = tileLocations ^ (~0x0123456789abcdef);
        uint64_t incorrectCorners = (tileLocations & 0x0f00f00000000f00ULL) ^ 0x0100400000000d00ULL;
        incorrectCorners |= incorrectCorners >> 1;
        incorrectCorners |= incorrectCorners >> 2;
        incorrectCorners |= incorrectCorners << 1;
        incorrectCorners |= incorrectCorners << 2;
        incorrectCorners &= 0x0f00f00000000f00ULL;
        uint64_t correctAdjacent = ~((tileLocations & 0x00ff0f00ff0000f0ULL) ^ 0x00230500890000e0ULL);
    }

    unsigned int manhattan() const noexcept {
        //return calculateManhattan4();
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
        return (tileLocations >> tile*4) & 0xfULL;
    }

    int linearConflict() const noexcept {
        int conflicts = 0;
        uint64_t correctness = ~(tileLocations ^ 0xedcba9876543210fULL);
        uint64_t rowCorrectness = correctness & 0x3333333333333333ULL;
        rowCorrectness &= (rowCorrectness | 0x1111111111111111ULL) >> 1;
        rowCorrectness &= (rowCorrectness | 0x2222222222222222ULL) << 1;
        uint64_t colCorrectness = (correctness & 0xccccccccccccccccULL) >> 2;
        colCorrectness &= (colCorrectness | 0x1111111111111111ULL) >> 1;
        colCorrectness &= (colCorrectness | 0x2222222222222222ULL) << 1;

        uint64_t rowSizes = tileLocations & (rowCorrectness << 2);
        uint64_t rowConflicts = twoBitGt(rowSizes, rowSizes >> 4);
        rowConflicts |= twoBitGt(rowSizes, rowSizes >> 8) << 1;
        rowConflicts |= twoBitGt(rowSizes, rowSizes >> 12) << 2;

        /*
         * Possible values:
         * 1234 - 0, 0
         * 1243 - 1, 1
         * 1423 - 2, 1
         * 4123 - 3, 1
         * 1432 - 2, 1
         *
         * perm - ans, gt
         * 1234 - 0, 0
         * 1243 - 1, 1
         * 1324 - 1, 1
         * 1342 - 1, 2
         * 1423 - 1, 2
         * 1432 - 2, 3
         * 2134 - 1, 1
         * 2143 - 2, 2
         * 2314 - 1, 2
         * 2341 - 1, 3
         * 2413 - 2, 4
         * 2431 - 2, 5
         * 3124 - 1, 2
         * 3142 - 2, 3
         * 3214 - 2,
         * 3241 - 2
         * 3412 - 2
         * 3421 - 2
         * 4123 - 1
         * 4132 - 2
         * 4213 - 2
         * 4231 - 2
         * 4312 - 2
         * 4321 - 3
         */

        conflicts += std::popcount(rowConflicts & 0x000000000000ffffULL);

        return conflicts;
    }

    void debugPrint() const noexcept {
        debugPrint(grid);
    }

    unsigned int parity() const noexcept {
        // Return 0 if solution has an even number of moves. 1 if odd
        return (tileLocations ^ (tileLocations >> 2)) & 1;
    }

private:
    uint64_t grid = 0;
    uint64_t tileLocations = 0;
    unsigned int manhattanCost = 0;

    uint64_t twoBitGt(uint64_t a, uint64_t b) const noexcept {
        /*
         * c = a > b
         *
         * a  b  c  a & ~b
         * 00 00 0  00
         * 00 01 0  00
         * 00 10 0  00
         * 00 11 0  00
         *
         * 01 00 1  01
         * 01 01 0  00
         * 01 10 0  01   <-- wrong
         * 01 11 0  00
         *
         * 10 00 1  10
         * 10 01 1  10
         * 10 10 0  00
         * 10 11 0  00
         *
         * 11 00 1  11
         * 11 01 1  10
         * 11 10 1  01
         * 11 11 0  00
         *
         * (~a & 2)
         *
         */
        uint64_t most = a & ~b;
        most = (most | (most >> 1)) & 0x5555555555555555ULL;
        uint64_t oneTwos = (a ^ 0x5555555555555555ULL) | (b ^ 0xaaaaaaaaaaaaaaaaULL);
        oneTwos |= (oneTwos & 0x5555555555555555ULL) << 1;
        oneTwos |= (oneTwos & 0xaaaaaaaaaaaaaaaaULL) >> 1;
        return (most & oneTwos) | (0x5555555555555555ULL & ~oneTwos);
    }

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

    __m128i convertToSimd(uint64_t input) const noexcept {
        uint64_t q0 = (input & 0x0f0f0f0f0f0f0f0fULL);
        uint64_t q1 = (input & 0xf0f0f0f0f0f0f0f0ULL) >> 4;

        return _mm_set_epi64x(q1, q0);
    }

    uint64_t convertToInt(__m128i input) const noexcept {
        return _mm_extract_epi64(input, 0) | _mm_extract_epi64(input, 1) << 4;
    }

    __m128i nibbleAbsSub(uint64_t a, uint64_t b) const noexcept {
        __m128i nibblesA = convertToSimd(a);
        __m128i nibblesB = convertToSimd(b);
        return _mm_add_epi8(_mm_subs_epu8(nibblesA, nibblesB), _mm_subs_epu8(nibblesB, nibblesA));
    }

    unsigned int calculateManhattan2() const noexcept {
        // ~36 instructions
        constexpr uint64_t column = 0x2103210321032100ULL;
        constexpr uint64_t row = 0x3332222111100000ULL;
        __m128i columnCost = nibbleAbsSub(tileLocations & 0x3333333333333330ULL, column);
        __m128i rowCost = nibbleAbsSub((tileLocations>>2) & 0x3333333333333330ULL, row);
        __m128i distances = _mm_add_epi8(columnCost, rowCost);
        uint64_t distance1 = _mm_extract_epi64(distances, 0);
        uint64_t distance2 = _mm_extract_epi64(distances, 1);
        distance1 += distance2;
        distance1 += distance1 >> 32;
        distance1 += distance1 >> 16;
        distance1 += distance1 >> 8;
        return distance1 & 0x00000000000000ffULL;
    }

    unsigned int calculateManhattan4() const noexcept {
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
        return total;
    }

    uint64_t twoBitAbsSub(uint64_t a, uint64_t b) const noexcept {
        /*
         * For c = abs(a-b):
         *
         * a  b  c
         * 00 00 00
         * 00 01 01
         * 00 10 10
         * 00 11 11
         *
         * 01 00 01
         * 01 01 00
         * 01 10 01
         * 01 11 10
         *
         * 10 00 10
         * 10 01 01
         * 10 10 00
         * 10 11 01
         *
         * 11 00 11
         * 11 01 10
         * 11 10 01
         * 11 11 00
         *
         * xor manages most of these except two lines
         * 01 10 01
         * 10 01 01
         *
         * Let's detect those mask them out and use xor for the rest
         */

        uint64_t most = a ^ b;
        uint64_t oneTwos = (a ^ 0x5555555555555555ULL) | (b ^ 0xaaaaaaaaaaaaaaaaULL);
        oneTwos |= (oneTwos & 0x5555555555555555ULL) << 1;
        oneTwos |= (oneTwos & 0xaaaaaaaaaaaaaaaaULL) >> 1;
        uint64_t twoOnes = (a ^ 0xaaaaaaaaaaaaaaaaULL) | (b ^ 0x5555555555555555ULL);
        twoOnes |= (twoOnes & 0x5555555555555555ULL) << 1;
        twoOnes |= (twoOnes & 0xaaaaaaaaaaaaaaaaULL) >> 1;
        uint64_t mask = oneTwos & twoOnes;
        return (most & mask) | (0x5555555555555555ULL & ~mask);
    }

    unsigned int calculateManhattan3() const noexcept {
        // ~38 instructions
        constexpr uint64_t column = 0x2103210321032103ULL;
        constexpr uint64_t row =    0x3332222111100003ULL;
        constexpr uint64_t all = column | (row << 2);
        uint64_t cost = twoBitAbsSub(tileLocations, all) & 0xfffffffffffffff0ULL;
        cost = (cost & 0x3333333333333333ULL) + ((cost>>2) & 0x3333333333333333ULL);
        cost = (cost & 0x0f0f0f0f0f0f0f0fULL) + ((cost>>4) & 0x0f0f0f0f0f0f0f0fULL);
        cost += cost >> 32;
        cost += cost >> 16;
        cost += cost >> 8;
        return cost & 0x00000000000000ffULL;
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

    std::string binary(uint64_t x) const noexcept {
        std::string output = "";
        for(unsigned int i = 0; i < 64; i++) {
            if(i % 4 == 0)
                output += " ";
            if((x >> (63-i)) & 1) {
                output += '1';
            } else {
                output += '0';
            }
        }
        return output;
    }
};

#endif // GRID_H
