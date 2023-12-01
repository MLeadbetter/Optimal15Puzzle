#ifndef UINT64_HASH_MAP_H
#define UINT64_HASH_MAP_H

#include <cstdint>
#include <bit>
#include <algorithm>
#include <iostream>

class Uint64HashMap {
public:
    Uint64HashMap(unsigned int m=20) : m(m), elements(0) {
        updateMask();
        uint64_t size = 1ULL << (m+1);
        _capacity = size * 3;
        table1 = new uint64_t[size]{0};
        table2 = new uint64_t[size]{0};
        table3 = new uint64_t[size]{0};
    }

    ~Uint64HashMap() {
        delete[] table1;
        delete[] table2;
        delete[] table3;
    }

    bool contains(uint64_t key) const noexcept {
        return table1[hash1(key)<<1]==key || table2[hash2(key)<<1]==key || table3[hash3(key)<<1]==key;
    }

    unsigned long operator [](uint64_t key) const {return get(key);}
    //unsigned long &operator [](uint64_t key) {return registers[i];}

    uint64_t get(uint64_t key) const noexcept {
        uint64_t h = hash1(key)<<1;
        if(table1[h]==key) return table1[h+1];
        h = hash2(key)<<1;
        if(table2[h]==key) return table2[h+1];
        h = hash3(key)<<1;
        if(table3[h]==key) return table3[h+1];
        return 0;
    }

    void insert(uint64_t key, uint64_t value) noexcept {
        uint64_t failed = insert(key, value, 0);
        if(failed) {
            doubleTable();
            insert(key, value);
        }
        elements++;
    }

    uint64_t size() { return elements; }
    uint64_t capacity() { return _capacity; }


private:
    void doubleTable() noexcept {
        m++;
        uint64_t size = 1ULL << (m+1);
        updateMask();
        _capacity = size | (size << 1); // Times 3
        uint64_t* tempTable1 = table1;
        uint64_t* tempTable2 = table2;
        uint64_t* tempTable3 = table3;
        table1 = new uint64_t[size]{0};
        table2 = new uint64_t[size]{0};
        table3 = new uint64_t[size]{0};
        for(unsigned int i = 0; i < size/2; i+=2) {
            if(tempTable1[i]) insert(tempTable1[i], tempTable1[i+1], 0);
            if(tempTable2[i]) insert(tempTable2[i], tempTable2[i+1], 0);
            if(tempTable3[i]) insert(tempTable3[i], tempTable3[i+1], 0);
        }
        delete[] tempTable1;
        delete[] tempTable2;
        delete[] tempTable3;
    }

    // Returns 0 on success, and 1 on failure
    bool insert(uint64_t key, uint64_t value, int attempts) noexcept {
        uint64_t loc = hash1(key)<<1;
        if(table1[loc] == 0) {
            table1[loc] = key;
            table1[loc+1] = value;
        } else {
            std::swap(table1[loc], key);
            std::swap(table1[loc+1], value);
            loc = hash2(key)<<1;
            if(table2[loc] == 0) {
                table2[loc] = key;
                table2[loc+1] = value;
            } else {
                std::swap(table2[loc], key);
                std::swap(table2[loc+1], value);
                loc = hash3(key)<<1;
                if(table3[loc] == 0) {
                    table3[loc] = key;
                    table3[loc+1] = value;
                } else {
                    std::swap(table3[loc], key);
                    std::swap(table3[loc+1], value);
                    if(attempts > 10)
                        return 1;
                    return insert(key, value, attempts+1);
                }
            }
        }
        return 0;
    }

    uint64_t hash1(uint64_t value) const noexcept {
        return std::rotr(value * 0xe7fe4ef13633798bULL, 32) & mask;
    }

    uint64_t invHash1(uint64_t value) const noexcept {
        return std::rotr(value, 32) * 0x8110742b64bb6623ULL;
    }

    uint64_t hash2(uint64_t value) const noexcept {
        return std::rotr(value * 0xa61387f4703332a9ULL, 32) & mask;
    }

    uint64_t invHash2(uint64_t value) const noexcept {
        return std::rotr(value, 32) * 0x54f95cabfa2d9199ULL;
    }

    uint64_t hash3(uint64_t value) const noexcept {
        return std::rotr(value * 0x45115d01b0c77e79ULL, 32) & mask;
    }

    uint64_t invHash3(uint64_t value) const noexcept {
        return std::rotr(value, 32) * 0x83aba487dac28bc9ULL;
    }

    void updateMask() noexcept {
        mask = ~(0xffffffffffffffffULL << m);
    }

    uint64_t mask;
    unsigned int m;
    uint64_t *table1;
    uint64_t *table2;
    uint64_t *table3;
    uint64_t elements = 0;
    uint64_t _capacity;
};

#endif // UINT64_HASH_MAP_H
