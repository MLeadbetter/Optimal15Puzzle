#ifndef QUARTERNARYTREE_H
#define QUARTERNARYTREE_H

#include <memory>
#include <algorithm>
#include <array>
#include <stack>
#include <cmath>
#include <bit>

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

    virtual ~QuarternaryTree() {
        std::stack<QuarternaryTreeNode*> nodes;
        nodes.push(&head);
        while(!nodes.empty()) {
            QuarternaryTreeNode* current = nodes.top();
            nodes.pop();
            if(current->u) nodes.push(current->u);
            if(current->d) nodes.push(current->d);
            if(current->l) nodes.push(current->l);
            if(current->r) nodes.push(current->r);
            if(current != &head) delete current;
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
#include <iostream>
class CircularQueue {
public:
    CircularQueue(unsigned int size) : _size(size) {
        data = new QuarternaryTreeNode*[size]{0};
    }

    /*CircularQueue(const CircularQueue &c) :_size(c._size) {
        data = new QuarternaryTreeNode*[_size]{0};
        for(unsigned int i = 0; i < _size; i++) {
            data[i] = c.data[i];
            pos = c.pos;
        }
    }

    CircularQueue& operator=(const CircularQueue &c) {
        //data = new QuarternaryTreeNode*[_size]{0};
        for(unsigned int i = 0; i < _size; i++) {
            data[i] = c.data[i];
            pos = c.pos;
        }
        return *this;
    }*/

    ~CircularQueue() {
        delete[] data;
    }

    void reinit() noexcept {
        delete[] data;
        data = new QuarternaryTreeNode*[_size]{0};
    }

    void setData(QuarternaryTreeNode** newData, unsigned int pos) noexcept {
        for(unsigned int i = 0; i < _size; i++) {
            data[i] = newData[i];
        }
        this->pos = pos;
    }

    unsigned int getPos() const noexcept {
        return pos;
    }

    void addStartPopEnd(QuarternaryTreeNode* val) noexcept {
        pos = (pos+_size-1) % _size;
        data[pos] = val;
    }

    QuarternaryTreeNode** getRaw() const noexcept {
        return data;
    }

    unsigned int size() const noexcept {
        return _size;
    }

    QuarternaryTreeNode* operator [](unsigned int index) const noexcept {return data[(pos+index)%_size];}
    QuarternaryTreeNode* &operator [](unsigned int index) noexcept {return data[(pos+index)%_size];}

private:
    QuarternaryTreeNode** data;
    unsigned int pos = 1;
    const unsigned int _size;
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

    static constexpr uint64_t primes2[]= {8400258977915390269ULL, 2651797441646065907ULL, 6485979681404405279ULL, 3399748860715184603ULL, 1275563148975948439ULL, 1275297488069249299ULL, 7860230888069455397ULL, 2993206844973573517ULL, 8382784540840949143ULL, 9267892822377177097ULL, 1168209292105888603ULL, 8183573410020441497ULL, 6185599539222423587ULL, 2356655709923141171ULL, 4487197072724967677ULL, 4781190765361296571ULL, 5279433610146807607ULL, 7897838741589597709ULL, 7076813857959433433ULL, 5326752869356981147ULL, 3586956073107489619ULL, 8872249270157779393ULL, 1942918662158584499ULL, 4067068890185269721ULL, 5671876708415451383ULL, 4812559778928314887ULL, 9654134999216276887ULL, 1208356955578222649ULL, 3629259048085269617ULL, 1146018678250844813ULL, 6616289894318405759ULL, 4251238325797481287ULL, 4563097568433622051ULL, 6487136592424400251ULL, 7206629461388892481ULL, 8149425579402935257ULL, 1708841473982518021ULL, 4477103760837165673ULL, 3955041634548512009ULL, 1814718795727399171ULL, 6736286652801120641ULL, 3440918648485120867ULL, 1178085811014202003ULL, 6409794909270965881ULL, 5674571425963490587ULL, 8503549932689185783ULL, 2870108303125622249ULL, 8166447337332708163ULL, 1808490791262827449ULL, 4768938432786421727ULL, 9943839582558052337ULL, 3820487317648082491ULL, 8813266707171359267ULL, 4061779618751475031ULL, 8693795364898594583ULL, 4758572949676498637ULL, 5533099753543305547ULL, 7249349253118374527ULL, 1507138902681555973ULL, 9631022480814455113ULL, 1273233659649375079ULL, 9368360816590980359ULL, 5043139390005874403ULL, 7984230445317223777ULL, 9600814179808709393ULL, 9347284875993649231ULL, 8815927017081012749ULL, 3294115622239180709ULL, 2746299196805350861ULL, 8125774468285036553ULL, 9739805701535896123ULL, 2082351095429950349ULL, 5155563119192308541ULL, 6930991917973614461ULL, 2250356340797193371ULL, 1593901110788350451ULL, 3262135988014002833ULL, 5030019825744950089ULL, 1437754360263987547ULL, 2305891990685721101ULL, 6865090266781889297ULL, 7069688648212460033ULL, 6072878647569060037ULL, 1303002385798111919ULL, 3411077213703622847ULL, 3812250525966924337ULL, 2724195237945537683ULL, 8136481884682153237ULL, 8359926488000406629ULL, 2544725808509826607ULL, 2720526517323445837ULL, 9382348298438955989ULL, 2174727428678912309ULL, 2176470240814777451ULL, 4284731490491457073ULL, 3161189890048474859ULL, 9770060200401016459ULL, 3657079457763299441ULL, 8112040393832986021ULL, 2579989519811387371ULL, 7707551629243020949ULL, 4718928810188440223ULL, 9195944040795170497ULL, 2275684215253345769ULL, 3709467574578267431ULL};
    //TreeTraversalNode() noexcept {}

    TreeTraversalNode(QuarternaryTree* tree) : tree(tree), state(tree->getMaxDepth()) {}

    void moveUp() noexcept {
        for(unsigned int i = 0; i < state.size(); i++) {
            if(state[i] != 0) state[i] = state[i]->u;
        }
        state.addStartPopEnd(tree->getHead()->u);
    }

    void moveDown() noexcept {
        for(unsigned int i = 0; i < state.size(); i++) {
            if(state[i] != 0) state[i] = state[i]->d;
        }
        state.addStartPopEnd(tree->getHead()->d);
    }

    void moveLeft() noexcept {
        for(unsigned int i = 0; i < state.size(); i++) {
            if(state[i] != 0) state[i] = state[i]->l;
        }
        state.addStartPopEnd(tree->getHead()->l);
    }

    void moveRight() noexcept {
        for(unsigned int i = 0; i < state.size(); i++) {
            if(state[i] != 0) state[i] = state[i]->r;
        }
        state.addStartPopEnd(tree->getHead()->r);
    }

    bool upValid() const noexcept {
        for(unsigned int i = 0; i < state.size(); i++) {
            if(state[i] != 0 && state[i]->u != 0 && !state[i]->u->isParent) return false;
        }
        return true;
    }

    bool downValid() const noexcept {
        for(unsigned int i = 0; i < state.size(); i++) {
            if(state[i] != 0 && state[i]->d != 0 && !state[i]->d->isParent) return false;
        }
        return true;
    }

    bool leftValid() const noexcept {
        for(unsigned int i = 0; i < state.size(); i++) {
            if(state[i] != 0 && state[i]->l != 0 && !state[i]->l->isParent) return false;
        }
        return true;
    }

    bool rightValid() const noexcept {
        for(unsigned int i = 0; i < state.size(); i++) {
            if(state[i] != 0 && state[i]->r != 0 && !state[i]->r->isParent) return false;
        }
        return true;
    }

    uint64_t ipow(uint64_t base, uint64_t exp) const noexcept {
        uint64_t result = 1;
        while(true) {
            if (exp & 1) result *= base;
            exp >>= 1;
            if (!exp) return result;
            base *= base;
        }
    }

    uint64_t hash(unsigned int p) const noexcept {
        uint64_t h = 1;
        for(unsigned int i = 0; i < state.size(); i++) {
            if(state[i] != 0)
                h *= ipow(primes[i+p], state[i]->id);
        }
        return h >> 1;
    }

    bool equals(const TreeTraversalNode* const b) const noexcept {
        if(b == 0) return false;
        for(unsigned int i = 0; i < state.size(); i++) {
            if(state[i] != b->state[i]) return false;
        }
        return true;
    }

    TreeTraversalNode* clone() const noexcept {
        TreeTraversalNode* c = new TreeTraversalNode(tree);
        c->id = id;
        c->state.setData(state.getRaw(), state.getPos());
        return c;
    }

    QuarternaryTree *getTree() const noexcept {
        return tree;
    }

    int id;

private:
    QuarternaryTree *tree;
    CircularQueue state;
};

struct HashNode {
    bool populated = false;
    uint64_t hash = 0;
    const TreeTraversalNode* key;
    uint64_t value;
};
#include <iostream>
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

    bool equal(const HashNode &node, const TreeTraversalNode* const value, const uint64_t hash) const noexcept {
        return node.populated && node.hash == hash && value->equals(node.key);
    }

    bool contains(const TreeTraversalNode* const value) const noexcept {
        const uint64_t h1 = hash1(value);
        uint64_t h = h1 & mask;
        if(equal(table1[h], value, h1)) return true;
        h = hash2(value) & mask;
        if(equal(table2[h], value, h1)) return true;
        h = hash3(value) & mask;
        if(equal(table3[h], value, h1)) return true;
        return false;
    }

    uint64_t operator [](const TreeTraversalNode* const key) const {return get(key);}
    //uint64_t &operator [](uint64_t key) {return registers[i];}

    uint64_t get(const TreeTraversalNode* const key) const noexcept {
        const uint64_t h1 = hash1(key);
        uint64_t h = h1 & mask;
        if(equal(table1[h], key, h1)) return table1[h].value;
        h = hash2(key) & mask;
        if(equal(table2[h], key, h1)) return table2[h].value;
        h = hash3(key) & mask;
        if(equal(table3[h], key, h1)) return table3[h].value;
        return 0;
    }

    void insert(const TreeTraversalNode* const key, const uint64_t value) noexcept {
        HashNode node;
        node.key = key;
        node.value = value;
        node.populated = true;
        std::pair<bool, HashNode> result = insert(node, 0);
        if(result.first) {
            doubleTable();
            insert(result.second, 0);
        }
        elements++;
    }

    uint64_t size() { return elements; }
    uint64_t capacity() { return _capacity; }

private:
    void doubleTable() noexcept {
        std::cout << "Doubling" << std::endl;
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
        for(unsigned int i = 0; i < size/2; i++) {
            if(tempTable1[i].populated) insert(tempTable1[i], 0);
            if(tempTable2[i].populated) insert(tempTable2[i], 0);
            if(tempTable3[i].populated) insert(tempTable3[i], 0);
        }
        delete[] tempTable1;
        delete[] tempTable2;
        delete[] tempTable3;
        std::cout << "Finished Doubling " << elements << ", " << _capacity << std::endl;
    }

    // Returns 0 on success, and 1 on failure
    std::pair<bool, HashNode> insert(HashNode value, const int attempts) noexcept {
        const uint64_t h1 = hash1(value.key);
        value.hash = h1;
        uint64_t h = h1;
        uint64_t loc = h & mask;
        if(!table1[loc].populated) {
            table1[loc] = value;
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
                        return std::make_pair(true, value);
                    return insert(value, attempts+1);
                }
            }
        }
        return std::make_pair(false, value);
    }

    uint64_t hash1(const TreeTraversalNode* const value) const noexcept {
        return value->hash(0);
    }

    uint64_t hash2(const TreeTraversalNode* const value) const noexcept {
        return value->hash(20);
    }

    uint64_t hash3(const TreeTraversalNode* const value) const noexcept {
        return value->hash(40);
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
