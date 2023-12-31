#ifndef FSM_H
#define FSM_H

#include <unordered_set>
#include <unordered_map>
#include <string>
#include <stack>
#include <vector>
#include <fstream>

#include "quarternarytree.h"

struct State {
    int up = -1;
    int down = -1;
    int left = -1;
    int right = -1;
};


class FSM {
public:
    void saveFSM(std::string fileName, std::vector<State> fsm) {
        std::ofstream ofs;
        ofs.open(fileName, std::ios::app | std::ios::binary);
        for(State &s : fsm) {
            ofs.write(reinterpret_cast<char*>(&s.up), sizeof(s.up));
            ofs.write(reinterpret_cast<char*>(&s.down), sizeof(s.down));
            ofs.write(reinterpret_cast<char*>(&s.left), sizeof(s.left));
            ofs.write(reinterpret_cast<char*>(&s.right), sizeof(s.right));
        }
        ofs.close();
    }

    std::vector<State> loadFSM(std::string fileName) {
        std::vector<State> fsm;
        std::ifstream ifs;
        ifs.open(fileName, std::ios::binary);
        while(!ifs.eof()) {
            State s;
            ifs.read(reinterpret_cast<char*>(&s.up), sizeof(s.up));
            ifs.read(reinterpret_cast<char*>(&s.down), sizeof(s.down));
            ifs.read(reinterpret_cast<char*>(&s.left), sizeof(s.left));
            ifs.read(reinterpret_cast<char*>(&s.right), sizeof(s.right));
            fsm.push_back(s);
        }
        ifs.close();
        return fsm;
    }

    std::vector<State> generateFSM(std::unordered_set<std::string> &redundant) noexcept {
        // Create 4-ary tree from strings
        // A slow way of using this would be to create a new traversal at the bottom of the tree every move
        // Then stopping any move from any of the traversals to an end node
        // Each traversal expires when a move to a node that does not exist in the tree happens

        // To generate a nice, speedy FSM we need to create a node for all possible multi-traversals of this tree
        // An upper bound on the size of this is 3^d, which ends up being surprisingly reasonable. Using 600mb of ram
        // at depth 17. Should go a fair way higher though.

        // We now need to do a DFS from all starting 0 positions to four times the length of the longest string.
        // Maintaining a hash table of seen states will cut most of this.
        //      At each step check if we've seen it.
        //          If so, connect the current state to that one.
        //          If not, create the state, add it to the hash table and connect the previous state to it.

        // Doing this from all posible starting 0 positions isn't necessary but may remove some impossible states from the state machine.
        QuarternaryTree tree;
        for(auto & s : redundant) {
            tree.addString(s);
        }

        std::vector<State> fsm;
        unsigned int traversalId = 0;
        TraversalHashMap hashMap;
        std::stack<TreeTraversalNode*> nodes;
        TreeTraversalNode* traversal = new TreeTraversalNode(&tree);
        traversal->id = traversalId;
        hashMap.insert(traversal, traversal->id);
        traversalId++;
        nodes.push(traversal);
        fsm.resize(1);
        while(!nodes.empty()) {
            TreeTraversalNode* toExpand = nodes.top();
            nodes.pop();
            if(fsm[toExpand->id].up == -1 && toExpand->upValid()) {
                TreeTraversalNode* newNode = toExpand->clone();
                newNode->moveUp();
                if(hashMap.contains(newNode)) {
                    fsm[toExpand->id].up = hashMap.get(newNode);
                    delete newNode;
                } else {
                    newNode->id = traversalId;
                    traversalId++;
                    hashMap.insert(newNode, newNode->id);
                    fsm.push_back(State());
                    fsm[toExpand->id].up = newNode->id;
                    nodes.push(newNode);
                }
            }

            if(fsm[toExpand->id].down == -1 && toExpand->downValid()) {
                TreeTraversalNode* newNode = toExpand->clone();
                newNode->moveDown();
                if(hashMap.contains(newNode)) {
                    fsm[toExpand->id].down = hashMap.get(newNode);
                    delete newNode;
                } else {
                    newNode->id = traversalId;
                    traversalId++;
                    hashMap.insert(newNode, newNode->id);
                    fsm.push_back(State());
                    fsm[toExpand->id].down = newNode->id;
                    nodes.push(newNode);
                }
            }

            if(fsm[toExpand->id].left == -1 && toExpand->leftValid()) {
                TreeTraversalNode* newNode = toExpand->clone();
                newNode->moveLeft();
                if(hashMap.contains(newNode)) {
                    fsm[toExpand->id].left = hashMap.get(newNode);
                    delete newNode;
                } else {
                    newNode->id = traversalId;
                    traversalId++;
                    hashMap.insert(newNode, newNode->id);
                    fsm.push_back(State());
                    fsm[toExpand->id].left = newNode->id;
                    nodes.push(newNode);
                }
            }

            if(fsm[toExpand->id].right == -1 && toExpand->rightValid()) {
                TreeTraversalNode* newNode = toExpand->clone();
                newNode->moveRight();
                if(hashMap.contains(newNode)) {
                    fsm[toExpand->id].right = hashMap.get(newNode);
                    delete newNode;
                } else {
                    newNode->id = traversalId;
                    traversalId++;
                    hashMap.insert(newNode, newNode->id);
                    fsm.push_back(State());
                    fsm[toExpand->id].right = newNode->id;
                    nodes.push(newNode);
                }
            }
        }
        return fsm;
    }
};

#endif // FSM_H
