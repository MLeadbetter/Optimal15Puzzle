#ifndef SIMPLIFIED_GRID_H
#define SIMPLIFIED_GRID_H

#include <cstdint>
#include <array>
#include <vector>
#include <limits>
#include <bit>
#include <bitset>
#include <cmath>

#include <iostream>

class SimplifiedGrid {
public:
    SimplifiedGrid() {}
    SimplifiedGrid(uint64_t grid) : grid(grid) {
        blankLoc = find(0);
    }

    int getBlankLoc() const noexcept {
        return blankLoc;
    }

    void set(const std::array<int, 16> &numbers) noexcept {
        for(int i = 0; i < 16; i++) {
                set(numbers[i], i);
        }
        blankLoc = find(0);
    }

    void set(uint64_t val, int loc) noexcept {
        loc *= 4;
        grid &= ~(0xfULL << loc);
        grid |= val << loc;
    }

    void moveUp(int blankLoc) noexcept {
        move(blankLoc, blankLoc+4);
        this->blankLoc += 4;
    }

    void moveDown(int blankLoc) noexcept {
        move(blankLoc, blankLoc-4);
        this->blankLoc -= 4;
    }

    void moveLeft(int blankLoc) noexcept {
        move(blankLoc, blankLoc+1);
        this->blankLoc += 1;
    }

    void moveRight(int blankLoc) noexcept {
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
        constexpr static std::array<uint64_t, 16> xorlu = {0x0000000000000000ULL, 0x1111111111111111ULL, 0x2222222222222222ULL, 0x3333333333333333ULL,
                                                           0x4444444444444444ULL, 0x5555555555555555ULL, 0x6666666666666666ULL, 0x7777777777777777ULL,
                                                           0x8888888888888888ULL, 0x9999999999999999ULL, 0xaaaaaaaaaaaaaaaaULL, 0xbbbbbbbbbbbbbbbbULL,
                                                           0xccccccccccccccccULL, 0xddddddddddddddddULL, 0xeeeeeeeeeeeeeeeeULL, 0xffffffffffffffffULL};
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
    int blankLoc = 0;

    void move(int loc) noexcept {
        move(find(0), loc);
    }

    void move(int blankLoc, int loc) noexcept {
        blankLoc *= 4;
        loc *= 4;
        grid |= ((grid >> loc) & 0xfULL) << blankLoc;
        grid &= ~(0xfULL << loc);
    }

    int valAt(int loc) const noexcept {
        loc *= 4;
        return ((0xfULL << loc) & grid) >> loc;
    }

    void debugPrint(uint64_t x) const noexcept {
        for(int i = 15; i >= 0; i--)
            std::cout << std::bitset<4>(x >> (i*4)) << " ";
        std::cout << std::endl;

        for(int i = 0; i < 16; i++) {
            int val = valAt(i);
            if(val < 10) std::cout << " ";
            std::cout << val << " ";
            if(i%4==3)
                std::cout << std::endl;
        }
        std::cout << std::endl;
    }
};

#endif // SIMPLIFIED_GRID_H
