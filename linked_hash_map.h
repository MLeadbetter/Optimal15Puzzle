#ifndef LINKED_HASH_MAP_H
#define LINKED_HASH_MAP_H

#include <cstdint>
#include <bit>
#include <algorithm>
#include <iostream>

#include "grid.h"

/*namespace {
struct MapNode {
    MapNode* prev = 0;
    MapNode* next = 0;
    Grid data;
    uint64_t value;
};
}

class LinkedHashMap {
public:
    LinkedHashMap(unsigned int m=20) : m(m), elements(0) {
        updateMask();
        uint64_t size = 1ULL << m;
        _capacity = size * 3;
        table1 = new MapNode[size];
        table2 = new MapNode[size];
        table3 = new MapNode[size];
    }

    ~LinkedHashMap() {
        delete[] table1;
        delete[] table2;
        delete[] table3;
    }

    void remove(uint64_t key) {
        MapNode* toRemove = table1[hash1(key)];
        if(toRemove->key == key) {
            _remove(toRemove);
            return;
        }
        toRemove = table2[hash2(key)];
        if(table2[hash2(key)]==key) {
            _remove(toRemove);
            return;
        }
        toRemove = table3[hash3(key)];
        if(table3[hash3(key)]==key) {
            _remove(toRemove);
            return;
        }
    }

    bool contains(uint64_t key) const noexcept {
        return table1[hash1(key)]==key || table2[hash2(key)]==key || table3[hash3(key)]==key;
    }

    uint64_t operator [](uint64_t key) const {return get(key);}

    uint64_t get(uint64_t key) const noexcept {
        uint64_t h = hash1(key);
        if(table1[h]==key) return table1[h+1];
        h = hash2(key);
        if(table2[h]==key) return table2[h+1];
        h = hash3(key);
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
        uint64_t size = 1ULL << m;
        updateMask();
        _capacity = size | (size << 1); // Times 3
        MapNode* tempTable1 = table1;
        MapNode* tempTable2 = table2;
        MapNode* tempTable3 = table3;
        table1 = new MapNode[size];
        table2 = new MapNode[size];
        table3 = new MapNode[size];
        for(unsigned int i = 0; i < size/2; i++) {
            if(tempTable1[i].data.getGrid()) insert(tempTable1[i].data.getGrid(), tempTable1[i+1].data.getGrid(), 0);
            if(tempTable2[i].data.getGrid()) insert(tempTable2[i].data.getGrid(), tempTable2[i+1].data.getGrid(), 0);
            if(tempTable3[i].data.getGrid()) insert(tempTable3[i].data.getGrid(), tempTable3[i+1].data.getGrid(), 0);
        }
        delete[] tempTable1;
        delete[] tempTable2;
        delete[] tempTable3;
    }

    // Returns 0 on success, and 1 on failure
    bool insert(Grid grid, uint64_t value, int attempts) noexcept {
        MapNode newNode;
        newNode.data = grid;
        newNode.next = head;
        uint64_t loc = hash1(grid.getGrid());
        if(table1[loc].data.getGrid() == 0) {
            table1[loc].data = grid;
            table1[loc+1].value = value;
        } else {
            std::swap(table1[loc].data, grid);
            std::swap(table1[loc+1].value, value);
            loc = hash2(grid.getGrid());
            if(table2[loc].data.getGrid() == 0) {
                table2[loc].data = grid;
                table2[loc+1].value = value;
            } else {
                std::swap(table2[loc], key);
                std::swap(table2[loc+1], value);
                loc = hash3(key);
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

    void _remove(MapNode<T>* toRemove) {
        toRemove->key = 0;
        if(toRemove->prev) toRemove->prev->next = toRemove->next;
        if(toRemove->next) toRemove->next->prev = toRemove->prev;
        elements--;
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
    MapNode *table1;
    MapNode *table2;
    MapNode *table3;
    uint64_t elements = 0;
    uint64_t _capacity;
    MapNode *head;
};
*/

#endif // LINKED_HASH_MAP_H
