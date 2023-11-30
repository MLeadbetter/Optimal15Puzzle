#ifndef LOWER_BOUND_TABLES_H
#define LOWER_BOUND_TABLES_H

#include <unordered_map>
#include <cstdint>
#include <vector>
#include <array>

#include "simplified_solver.h"

class LowerBoundTables {
public:

    LowerBoundTables() {
        generateTables();
    }

    void generateTables() {
        std::vector<unsigned int> values({1, 2, 3, 4});
        h[0] = bulkSolve(values);
        first_unused_h[0] = find_first_unused(values);

        values = {5, 6, 7, 8};
        h[1] = bulkSolve(values);
        first_unused_h[1] = find_first_unused(values);

        values = {9, 10, 11, 12};
        h[2] = bulkSolve(values);
        first_unused_h[2] = find_first_unused(values);



        values = {1, 5, 9, 13};
        v[0] = bulkSolve(values);
        first_unused_v[0] = find_first_unused(values);

        values = {2, 6, 10, 14};
        v[1] = bulkSolve(values);
        first_unused_v[1] = find_first_unused(values);

        values = {3, 7, 11, 15};
        v[2] = bulkSolve(values);
        first_unused_v[2] = find_first_unused(values);


        values = {11, 12, 13, 14, 15};
        //values = {4, 8, 11};
        e[0] = bulkSolve(values);
        first_unused_e[0] = find_first_unused(values);

        values = {4, 8, 11, 12, 15};
        //values = {4, 8, 11};
        e[1] = bulkSolve(values);
        first_unused_e[1] = find_first_unused(values);

        values = {4, 8, 11, 13, 14, 15};
        //values = {4, 8, 11};
        e[2] = bulkSolve(values);
        first_unused_e[2] = find_first_unused(values);
    }

    unsigned int get_first_unused_h(unsigned int index) const noexcept {
        return first_unused_h[index];
    }

    unsigned int get_first_unused_v(unsigned int index) const noexcept {
        return first_unused_v[index];
    }

    unsigned int get_first_unused_e(unsigned int index) const noexcept {
        return first_unused_e[index];
    }

    int get_moves_h(uint64_t grid, unsigned int index) noexcept {
        return h[index][grid];
    }

    int get_moves_v(uint64_t grid, unsigned int index) noexcept {
        return v[index][grid];
    }

    int get_moves_e(uint64_t grid, unsigned int index) noexcept {
        return e[index][grid];
    }

private:
    std::array<std::unordered_map<uint64_t, unsigned int>, 3> h;
    std::array<std::unordered_map<uint64_t, unsigned int>, 3> v;
    std::array<std::unordered_map<uint64_t, unsigned int>, 3> e;

    std::array<unsigned int, 3> first_unused_h;
    std::array<unsigned int, 3> first_unused_v;
    std::array<unsigned int, 3> first_unused_e;

    unsigned int find_first_unused(const std::vector<unsigned int> &values) {
        for(unsigned int i = 1; i < 16; i++) {
            bool used = false;
            for(unsigned int j = 0; j < values.size(); j++) {
                if(i == values[j]) used = true;
            }
            if(!used) return i;
        }
        return -1;
    }

    std::unordered_map<uint64_t, unsigned int> bulkSolve(const std::vector<unsigned int> &values) {
        SimplifiedBackwardsSolver solver;
        std::array<int, 16> solved = { 1,  2,  3,  4,
                                       5,  6,  7,  8,
                                       9,  10, 11, 12,
                                       13, 14, 15, 0 };
        int first_unused = find_first_unused(values);
        solved = { first_unused, first_unused, first_unused, first_unused,
                   first_unused, first_unused, first_unused, first_unused,
                   first_unused, first_unused, first_unused, first_unused,
                   first_unused, first_unused, first_unused, 0 };
        for(auto value : values) {
            solved[value-1] = value;
        }
        return solver.solveFromEnd(100, solved);
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
        std::cout << std::endl;
    }
};

#endif // LOWER_BOUND_TABLES_H
