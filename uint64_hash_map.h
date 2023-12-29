#ifndef UINT64_HASH_MAP_H
#define UINT64_HASH_MAP_H

#include <cstdint>
#include <bit>
#include <algorithm>

#include <iostream>

template<class T>
struct KeyValue {
    uint64_t key = 0;
    T value;
};

template<class T>
struct Uint64HashMapIterator  {
    Uint64HashMapIterator(unsigned int m, KeyValue<T> *table1, KeyValue<T> *table2, KeyValue<T> *table3, bool end=false) : m(m), table1(table1), table2(table2), table3(table3) {
        if(end) {
            pos = (1ULL<<m);
            table = 2;
        } else {
            pos = 0;
            table = 0;
        }
    }

    KeyValue<T> operator*() const {
        switch(table) {
        case 0:
            return table1[pos];
            break;
        case 1:
            return table2[pos];
            break;
        default:
            return table3[pos];
            break;
        }
    }

    KeyValue<T>* operator->() {
        switch(table) {
        case 0:
            return &table1[pos];
            break;
        case 1:
            return &table2[pos];
            break;
        default:
            return &table3[pos];
            break;
        }
    }

    // Prefix increment
    Uint64HashMapIterator<T>& operator++() {
        pos++;
        KeyValue<T>* ptable = getPtable();

        do {
            if(pos == (1ULL<<m) && table < 2) {
                table++;
                ptable = getPtable();
                pos = 0;
            }
            while(pos < (1ULL<<m) && ptable[pos].key == 0) pos++;
        } while(pos == (1ULL<<m) && table < 2);

        return *this;
    }

    // Postfix increment
    Uint64HashMapIterator<T> operator++(int) { Uint64HashMapIterator<T> tmp = *this; ++(*this); return tmp; }

    friend bool operator== (const Uint64HashMapIterator<T>& a, const Uint64HashMapIterator<T>& b) { return a.pos == b.pos && a.table == b.table; };
    friend bool operator!= (const Uint64HashMapIterator<T>& a, const Uint64HashMapIterator<T>& b) { return a.pos != b.pos || a.table != b.table; };

private:

    KeyValue<T>* getPtable() {
        switch(table) {
        case 0:
            return table1;
            break;
        case 1:
            return table2;
            break;
        default:
            return table3;
            break;
        }
    }

    uint64_t pos;
    unsigned int m;
    KeyValue<T> *table1;
    KeyValue<T> *table2;
    KeyValue<T> *table3;
    unsigned int table;
};

template<class T>
class Uint64HashMap {
public:
    Uint64HashMap(unsigned int m=2) : m(m), elements(0) {
        updateMask();
        uint64_t size = 1ULL << this->m;
        _capacity = size * 3;
        table1 = new KeyValue<T>[size];
        table2 = new KeyValue<T>[size];
        table3 = new KeyValue<T>[size];
    }

    ~Uint64HashMap() noexcept {
        delete[] table1;
        delete[] table2;
        delete[] table3;
    }

    Uint64HashMap<T>& operator=(const Uint64HashMap<T>& other) noexcept {
        this->table1 = new Uint64HashMap<T>[1ULL << other.m];
        this->table2 = new Uint64HashMap<T>[1ULL << other.m];
        this->table3 = new Uint64HashMap<T>[1ULL << other.m];
        uint64_t size = 1ULL << other.m;
        for(unsigned int i = 0; i < size; i++) {
            table1[i] = other.table1[i];
            table3[i] = other.table2[i];
            table2[i] = other.table3[i];
        }
        this->m = other.m;
        this->mask = other.mask;
        this->_capacity = other._capacity;
        return *this;
    }

    Uint64HashMap(Uint64HashMap<T> &&other) {
        other.table1 = 0;
        other.table2 = 0;
        other.table3 = 0;
    }

    Uint64HashMap<T>& operator=(Uint64HashMap<T> &&other) noexcept {
        if (this != &other) {
            delete[] table1;
            delete[] table2;
            delete[] table3;
            table1 = other.table1;
            table2 = other.table2;
            table3 = other.table3;
            other.table1 = 0;
            other.table2 = 0;
            other.table3 = 0;
            m = other.m;
            _capacity = other._capacity;
            elements = other.elements;
        }
        return *this;
    }

    bool contains(uint64_t key) const noexcept {
        uint64_t k1 = table1[hash1(key)].key;
        uint64_t k2 = table2[hash2(key)].key;
        uint64_t k3 = table3[hash3(key)].key;
        return k1==key || k2==key || k3==key;
    }

    T get(uint64_t key) const noexcept {
        uint64_t h = hash1(key);
        if(table1[h].key==key) return table1[h].value;
        h = hash2(key);
        if(table2[h].key==key) return table2[h].value;
        h = hash3(key);
        if(table3[h].key==key) return table3[h].value;
        return 0;
    }

    void insert(uint64_t key, T value) noexcept {
        KeyValue<T> kv;
        kv.key = key;
        kv.value = value;
        insert(kv);
    }

    uint64_t size() { return elements; }
    uint64_t capacity() { return _capacity; }

    uint64_t operator [](uint64_t key) const {return get(key);}
    //uint64_t &operator [](uint64_t key) {return registers[i];}

    Uint64HashMapIterator<T> begin() { return Uint64HashMapIterator<T>(m, table1, table2, table3); }
    Uint64HashMapIterator<T> end()   { return Uint64HashMapIterator<T>(m, table1, table2, table3, true); }


private:
    void doubleTable() noexcept {
        m++;
        uint64_t size = 1ULL << m;
        updateMask();
        _capacity = size * 3;
        KeyValue<T>* tempTable1 = table1;
        KeyValue<T>* tempTable2 = table2;
        KeyValue<T>* tempTable3 = table3;
        table1 = new KeyValue<T>[size];
        table2 = new KeyValue<T>[size];
        table3 = new KeyValue<T>[size];
        for(unsigned int i = 0; i < size/2; i++) {
            if(tempTable1[i].key) insert(tempTable1[i], 0);
            if(tempTable2[i].key) insert(tempTable2[i], 0);
            if(tempTable3[i].key) insert(tempTable3[i], 0);
        }
        delete[] tempTable1;
        delete[] tempTable2;
        delete[] tempTable3;
    }

    void insert(KeyValue<T> kv) noexcept {
        KeyValue<T> failed = insert(kv, 0);
        if(failed.key) {
            doubleTable();
            insert(failed);
        }
        elements++;
    }

    // Returns 0 on success, and an uninserted value on failure
    KeyValue<T> insert(KeyValue<T> value, int attempts) noexcept {
        uint64_t loc = hash1(value.key);
        if(table1[loc].key == 0) {
            table1[loc] = value;
        } else {
            std::swap(table1[loc], value);
            loc = hash2(value.key);
            if(table2[loc].key == 0) {
                table2[loc] = value;
            } else {
                std::swap(table2[loc], value);
                loc = hash3(value.key);
                if(table3[loc].key == 0) {
                    table3[loc] = value;
                } else {
                    std::swap(table3[loc], value);
                    if(attempts > 10)
                        return value;
                    return insert(value, attempts+1);
                }
            }
        }
        return KeyValue<T>();
    }

    uint64_t hash1(uint64_t value) const noexcept {
        return std::rotr(std::rotr(value * 0xe7fe4ef13633798bULL, 32) * 0xe7a393e13193a9bULL, 32) & mask;
    }

    uint64_t hash2(uint64_t value) const noexcept {
        return std::rotr(std::rotr(value * 0xa61387f4703332a9ULL, 32) * 0x97ec2d6906f6cbd5, 32) & mask;
    }

    uint64_t hash3(uint64_t value) const noexcept {
        return std::rotr(std::rotr(value * 0x45115d01b0c77e79ULL, 32) * 0x679e88f423e977b9, 32) & mask;
    }

    void updateMask() noexcept {
        mask = ~(0xffffffffffffffffULL << m);
    }

    uint64_t mask;
    unsigned int m;
    KeyValue<T> *table1;
    KeyValue<T> *table2;
    KeyValue<T> *table3;
    uint64_t elements = 0;
    uint64_t _capacity;
};

#endif // UINT64_HASH_MAP_H
