#ifndef QUARTERNARYTREE_H
#define QUARTERNARYTREE_H

#include <memory>
#include <algorithm>
#include <deque>
#include <array>
#include <cmath>

template<class T>
class CircularQueue {
public:
    CircularQueue() {
        data = new T[size]{0};
    }

    ~CircularQueue() {
        delete[] data;
    }

    void reinit() noexcept {
        delete[] data;
        data = new T[size]{0};
    }

    void addStartPopEnd(T val) noexcept {
        pos = (pos+1) % size;
        data[pos] = val;
    }

    T* getRaw() const noexcept {
        return data;
    }

    T operator [](unsigned int index) const noexcept {return data[(pos+index)%size];}
    T &operator [](unsigned int index) noexcept {return data[(pos+index)%size];}

private:
    T* data;
    unsigned int pos = 0;
    unsigned int size = 0;
};

struct QuarternaryTreeNode {
    QuarternaryTreeNode* u = 0;
    QuarternaryTreeNode* d = 0;
    QuarternaryTreeNode* l = 0;
    QuarternaryTreeNode* r = 0;
    bool isParent = false;
    unsigned int id = 0;
};

class QuarternaryTree {
    QuarternaryTreeNode head;
    unsigned long maxDepth = 0;
    unsigned int currentId = 1;

public:
    void addString(std::string s) {
        maxDepth = std::max(maxDepth, s.length());
        QuarternaryTreeNode* current = &head;
        for(const char c : s) {
            current->isParent = true;
            switch(c) {
            case 'u':
                if(current->u == 0) {
                    current->u = new QuarternaryTreeNode();
                    current->u->id = currentId;
                    currentId++;
                }
                current = current->u;
                break;
            case 'd':
                if(current->d == 0) {
                    current->d = new QuarternaryTreeNode();
                    current->d->id = currentId;
                    currentId++;
                }
                current = current->d;
                break;
            case 'r':
                if(current->r == 0) {
                    current->r = new QuarternaryTreeNode();
                    current->r->id = currentId;
                    currentId++;
                }
                current = current->r;
                break;
            case 'l':
                if(current->l == 0) {
                    current->l = new QuarternaryTreeNode();
                    current->l->id = currentId;
                    currentId++;
                }
                current = current->l;
                break;
            }
        }
    }

    bool superceeded(std::string s) {
        for(unsigned int i = 0; i < s.length()-1; i++) {
            QuarternaryTreeNode* current = &head;
            for(unsigned int j = i; j < s.length(); j++) {
                switch(s[j]) {
                case 'u':
                    if(current->u) current = current->u;
                    else goto next;
                    break;
                case 'd':
                    if(current->d) current = current->d;
                    else goto next;
                    break;
                case 'l':
                    if(current->l) current = current->l;
                    else goto next;
                    break;
                case 'r':
                    if(current->r) current = current->r;
                    else goto next;
                    break;
                }
                if(current->isParent == false) return true;
            }
        next:;
        }
        return false;
    }

    QuarternaryTreeNode* getHead() {
        return &head;
    }

    unsigned long getMaxDepth() {
        return maxDepth;
    }
};

struct TreeTraversalNode {
public:
    static constexpr unsigned int primes[] = {3,  5,  7,  11, 13,
                                              17, 19, 23, 29, 31,
                                              37, 41, 43, 47, 53,
                                              59, 61, 67, 71, 73,
                                              79, 83, 89, 97, 101,
                                              103, 107, 109, 113, 127,
                                              131, 137, 139, 149, 151,
                                              157, 163, 167, 173, 179,
                                              181, 191, 193, 197, 199,
                                              211, 223, 227, 229, 233,
                                              239, 241, 251, 257, 263,
                                              269, 271, 277, 281, 283,
                                              293, 307, 311, 313, 317,
                                              331, 337, 347, 349, 353,
                                              359, 367, 373, 379, 383,
                                              389, 397, 401, 409, 419,
                                              421, 431, 433, 439, 443,
                                              449, 457, 461, 463, 467,
                                              479, 487, 491, 499, 503,
                                              509, 521, 523, 541, 547,
                                              557, 563, 569, 571, 577,
                                              587, 593, 599, 601, 607,
                                              613, 617, 619, 631, 641,
                                              643, 647, 653, 659, 661,
                                              673, 677, 683, 691, 701,
                                              709, 719, 727, 733, 739,
                                              743, 751, 757, 761, 769,
                                              773, 787, 797, 809, 811,
                                              821, 823, 827, 829, 839,
                                              853, 857, 859, 863, 877,
                                              881, 883, 887, 907, 911,
                                              919, 929, 937, 941, 947,
                                              953, 967, 971, 977, 983,
                                              991, 997, 1009, 1013, 1019,
                                              1021, 1031, 1033, 1039, 1049,
                                              1051, 1061, 1063, 1069, 1087,
                                              1091, 1093, 1097, 1103, 1109,
                                              1117, 1123, 1129, 1151, 1153,
                                              1163, 1171, 1181, 1187, 1193,
                                              1201, 1213, 1217, 1223};

    TreeTraversalNode() noexcept {}

    TreeTraversalNode(QuarternaryTree* tree) noexcept {
        setTree(tree);
    }

    void setTree(QuarternaryTree* tree) noexcept {
        state.resize(tree->getMaxDepth(), 0);
        this->tree = tree;
    }

    void moveUp() noexcept {
        for(unsigned int i = 0; i < state.size(); i++) {
            if(state[i] != 0) state[i] = state[i]->u;
        }
        state.push_front(tree->getHead()->u);
        state.pop_back();
    }

    void moveDown() noexcept {
        for(unsigned int i = 0; i < state.size(); i++) {
            if(state[i] != 0) state[i] = state[i]->d;
        }
        state.push_front(tree->getHead()->d);
        state.pop_back();
    }

    void moveLeft() noexcept {
        for(unsigned int i = 0; i < state.size(); i++) {
            if(state[i] != 0) state[i] = state[i]->l;
        }
        state.push_front(tree->getHead()->l);
        state.pop_back();
    }

    void moveRight() noexcept {
        for(unsigned int i = 0; i < state.size(); i++) {
            if(state[i] != 0) state[i] = state[i]->r;
        }
        state.push_front(tree->getHead()->r);
        state.pop_back();
    }

    bool upValid() const noexcept {
        for(QuarternaryTreeNode* const s : state) {
            if(s != 0 && s->u != 0 && !s->u->isParent) return false;
        }
        return true;
    }

    bool downValid() const noexcept {
        for(QuarternaryTreeNode* const s : state) {
            if(s != 0 && s->d != 0 && !s->d->isParent) return false;
        }
        return true;
    }

    bool leftValid() const noexcept {
        for(QuarternaryTreeNode* const s : state) {
            if(s != 0 && s->l != 0 && !s->l->isParent) return false;
        }
        return true;
    }

    bool rightValid() const noexcept {
        for(QuarternaryTreeNode* const s : state) {
            if(s != 0 && s->r != 0 && !s->r->isParent) return false;
        }
        return true;
    }

    int ipow(uint64_t base, uint64_t exp) const noexcept {
        int result = 1;
        while(true) {
            if (exp & 1)
                result *= base;
            exp >>= 1;
            if (!exp)
                break;
            base *= base;
        }

        return result;
    }

    uint64_t hash(unsigned int p) const noexcept {
        uint64_t h = 1;
        for(unsigned int i = 0; i < state.size(); i++) {
            if(state[i] != 0)
                h *= ipow(primes[i+p], state[i]->id);
        }
        return h;
    }

    bool equals(const TreeTraversalNode *b) const noexcept {
        if(b == 0) return false;
        for(unsigned int i = 0; i < state.size(); i++) {
            if(state[i] != b->state[i]) return false;
        }
        return true;
    }

    TreeTraversalNode* clone() const noexcept {
        TreeTraversalNode* c = new TreeTraversalNode;
        c->id = id;
        c->state = state;
        c->tree = tree;
        return c;
    }

    QuarternaryTree *getTree() const noexcept {
        return tree;
    }

    int id;

private:
    std::deque<QuarternaryTreeNode*> state;
    QuarternaryTree *tree;
};

struct HashNode {
    bool populated = false;
    uint64_t hash = 0;
    TreeTraversalNode *key;
    uint64_t value;
};

class TraversalHashMap {
public:
    TraversalHashMap(unsigned int m=20) : m(m), elements(0) {
        updateMask();
        uint64_t size = 1ULL << m;
        _capacity = size * 3;
        table1 = new HashNode[size];
        table2 = new HashNode[size];
        table3 = new HashNode[size];
    }

    ~TraversalHashMap() {
        uint64_t size = 1ULL << m;
        for(unsigned int i = 0; i < size; i++) {
            if(table1[i].populated) delete table1[i].key;
            if(table2[i].populated) delete table2[i].key;
            if(table3[i].populated) delete table3[i].key;
        }
        delete[] table1;
        delete[] table2;
        delete[] table3;
    }

    bool equal(HashNode &node, TreeTraversalNode *value, uint64_t hash) const noexcept {
        return node.populated && node.hash == hash && value->equals(node.key);
    }

    bool contains(TreeTraversalNode *value) const noexcept {
        uint64_t h = hash1(value) & mask;
        if(equal(table1[h], value, h)) return true;
        h = hash2(value) & mask;
        if(equal(table2[h], value, h)) return true;
        h = hash3(value) & mask;
        if(equal(table3[h], value, h)) return true;
        return false;
    }

    uint64_t operator [](TreeTraversalNode *key) const {return get(key);}
    //uint64_t &operator [](uint64_t key) {return registers[i];}

    uint64_t get(TreeTraversalNode *key) const noexcept {
        uint64_t h = hash1(key) & mask;
        if(equal(table1[h], key, h)) return table1[h].value;
        h = hash2(key) & mask;
        if(equal(table2[h], key, h)) return table2[h].value;
        h = hash3(key) & mask;
        if(equal(table3[h], key, h)) return table3[h].value;
        return 0;
    }

    void insert(TreeTraversalNode* key, uint64_t value) noexcept {
        HashNode node;
        node.key = key;
        node.value = value;
        node.populated = true;
        uint64_t failed = insert(node, 0);
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
        _capacity = size * 3;
        HashNode* tempTable1 = table1;
        HashNode* tempTable2 = table2;
        HashNode* tempTable3 = table3;
        table1 = new HashNode[size];
        table2 = new HashNode[size];
        table3 = new HashNode[size];
        for(unsigned int i = 0; i < size/2; i+=2) {
            if(tempTable1[i].populated) insert(tempTable1[i], 0);
            if(tempTable2[i].populated) insert(tempTable2[i], 0);
            if(tempTable3[i].populated) insert(tempTable3[i], 0);
        }
        delete[] tempTable1;
        delete[] tempTable2;
        delete[] tempTable3;
    }

    // Returns 0 on success, and 1 on failure
    bool insert(HashNode value, int attempts) noexcept {
        uint64_t h = hash1(value.key);
        uint64_t loc = h & mask;
        if(!table1[loc].populated) {
            table1[loc] = value;
            table1[loc].hash = loc;
        } else {
            std::swap(table1[loc], value);
            h = hash2(value.key);
            loc = h & mask;
            if(!table2[loc].populated) {
                table2[loc] = value;
            } else {
                std::swap(table2[loc], value);
                h = hash3(value.key);
                loc = h & mask;
                if(!table3[loc].populated) {
                    table3[loc] = value;
                } else {
                    std::swap(table3[loc], value);
                    if(attempts > 10)
                        return 1;
                    return insert(value, attempts+1);
                }
            }
        }
        return 0;
    }

    uint64_t hash1(const TreeTraversalNode *value) const noexcept {
        return value->hash(0);
    }

    uint64_t hash2(const TreeTraversalNode *value) const noexcept {
        return value->hash(5);
    }

    uint64_t hash3(const TreeTraversalNode *value) const noexcept {
        return value->hash(10);
    }

    void updateMask() noexcept {
        mask = ~(0xffffffffffffffffULL << m);
    }

    uint64_t mask;
    unsigned int m;
    HashNode *table1;
    HashNode *table2;
    HashNode *table3;
    uint64_t elements = 0;
    uint64_t _capacity;
};

#endif // QUARTERNARYTREE_H
