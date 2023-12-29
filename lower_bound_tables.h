#ifndef LOWER_BOUND_TABLES_H
#define LOWER_BOUND_TABLES_H

#include <unordered_map>
#include <cstdint>
#include <vector>
#include <array>
#include <algorithm>
#include <fstream>

#include "simplified_solver.h"
#include "grid_converter.h"
#include "uint64_hash_map.h"

#define NUM_TABLES 9

class LowerBoundTables {
public:

    unsigned int lower_bound(uint64_t tileLocations) {
        unsigned int lb = 0;
        for(unsigned int i = 0; i < NUM_TABLES; i++) {
            lb = std::max(lb, tables[i].get(tileLocations & masks[i]));
        }
        return lb;
    }

    void generateTables() {
        for(auto &table : tables) {
            table = Uint64HashMap<unsigned int>(8);
        }
        populateTable(0, {1,  2,  3,  4});
        populateTable(1, {5,  6,  7,  8});
        populateTable(2, {9,  10, 11, 12});
        populateTable(3, {1,  5,  9,  13});
        populateTable(4, {2,  6,  10, 14});
        populateTable(5, {3,  7,  11, 15});
        populateTable(6, {11, 12, 13, 14, 15});
        populateTable(7, {4,  8,  11, 12, 15});
        populateTable(8, {4,  8,  12, 13, 14, 15});
    }

    void saveTables() {
        for(unsigned int i = 0; i < NUM_TABLES; i++) {
            std::ofstream ofs;
            ofs.open(std::string("table") + std::to_string(i), std::ios::binary);
            uint64_t size = tables[i].size();
            ofs.write(reinterpret_cast<char*>(&size), sizeof(size));
            for(KeyValue<unsigned int> d : tables[i]) {
                ofs.write(reinterpret_cast<char*>(&d.key), sizeof(d.key));
                ofs.write(reinterpret_cast<char*>(&d.value), sizeof(d.value));
            }
            ofs.close();
        }
    }

    void loadTables() {
        masks[0] = createMask({1,  2,  3,  4});
        masks[1] = createMask({5,  6,  7,  8});
        masks[2] = createMask({9,  10, 11, 12});
        masks[3] = createMask({1,  5,  9,  13});
        masks[4] = createMask({2,  6,  10, 14});
        masks[5] = createMask({3,  7,  11, 15});
        masks[6] = createMask({11, 12, 13, 14, 15});
        masks[7] = createMask({4,  8,  11, 12, 15});
        masks[8] = createMask({4,  8,  12, 13, 14, 15});
        for(unsigned int i = 0; i < 9; i++) {
            std::ifstream ifs;
            ifs.open(std::string("table") + std::to_string(i), std::ios::binary);
            uint64_t size;
            ifs.read(reinterpret_cast<char*>(&size), sizeof(size));

            tables[i] = Uint64HashMap<unsigned int>(64 - std::countl_zero(size / 3));
            while(!ifs.eof()) {
                uint64_t first;
                ifs.read(reinterpret_cast<char*>(&first), sizeof(first));
                unsigned int second;
                ifs.read(reinterpret_cast<char*>(&second), sizeof(second));
                tables[i].insert(first, second);
            }
            ifs.close();
        }
    }

private:
    std::array<Uint64HashMap<unsigned int>, NUM_TABLES> tables;
    std::array<uint64_t, NUM_TABLES> masks;

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

    void populateTable(unsigned int index, const std::vector<unsigned int> &values) {
        masks[index] = createMask(values);
        std::unordered_map<uint64_t, unsigned int> initialSolve = bulkSolve(values);
        for(std::pair<uint64_t, unsigned int> solution : initialSolve) {
            uint64_t key = gridToTileLocations(solution.first) & masks[index];
            if(!tables[index].contains(key)) tables[index].insert(key, solution.second);
        }
    }

    uint64_t createMask(std::vector<unsigned int> values) {
        uint64_t mask = 0xfULL;
        for(const unsigned int &value : values) {
            mask |= 0xfULL << value*4;
        }
        return mask;
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

#undef NUM_TABLES

#endif // LOWER_BOUND_TABLES_H
