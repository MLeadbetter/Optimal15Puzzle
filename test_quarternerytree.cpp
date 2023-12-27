#include "gtest/gtest.h"

#include "quarternarytree.h"

using namespace std;

TEST(quarternary_tree, test_depth) {
    QuarternaryTree tree;
    tree.addString("u");
    EXPECT_EQ(1, tree.getMaxDepth());
}

TEST(quarternary_tree, test_depth2) {
    QuarternaryTree tree;
    tree.addString("u");
    tree.addString("ud");
    EXPECT_EQ(2, tree.getMaxDepth());
}

TEST(quarternary_tree, test_string1) {
    QuarternaryTree tree;
    tree.addString("u");
    EXPECT_TRUE(tree.getHead()->u != 0);
    EXPECT_TRUE(tree.getHead()->d == 0);
}

TEST(quarternary_tree, test_string2) {
    QuarternaryTree tree;
    tree.addString("u");
    tree.addString("ur");
    EXPECT_TRUE(tree.getHead()->u->r != 0);
    EXPECT_TRUE(tree.getHead()->u->l == 0);
}

TEST(quarternary_tree, test_id) {
    QuarternaryTree tree;
    tree.addString("u");
    EXPECT_EQ(1, tree.getHead()->u->id);
}

TEST(quarternary_tree, test_id2) {
    QuarternaryTree tree;
    tree.addString("u");
    tree.addString("ur");
    EXPECT_EQ(2, tree.getHead()->u->r->id);
}

TEST(quarternary_tree, test_id3) {
    QuarternaryTree tree;
    tree.addString("u");
    tree.addString("ur");
    tree.addString("r");
    EXPECT_EQ(3, tree.getHead()->r->id);
}

TEST(quarternary_tree, test_superceeded_start) {
    QuarternaryTree tree;
    tree.addString("ud");
    tree.addString("du");
    EXPECT_TRUE(tree.superceeded("udlr"));
    EXPECT_FALSE(tree.superceeded("uldr"));
}

TEST(quarternary_tree, test_superceeded_middle) {
    QuarternaryTree tree;
    tree.addString("ud");
    tree.addString("du");
    EXPECT_TRUE(tree.superceeded("rudlr"));
}

TEST(quarternary_tree, test_superceeded_end) {
    QuarternaryTree tree;
    tree.addString("ud");
    tree.addString("du");
    EXPECT_TRUE(tree.superceeded("dlrud"));
}

TEST(tree_traversal, test_simple_tree_move) {
    QuarternaryTree tree;
    tree.addString("u");
    TreeTraversalNode traversal(&tree);
    traversal.moveUp();
    EXPECT_EQ(3, traversal.hash(0));
}

TEST(tree_traversal, test_simple_tree_hash) {
    QuarternaryTree tree;
    tree.addString("u");
    TreeTraversalNode traversal(&tree);
    traversal.moveUp();
    EXPECT_EQ(5, traversal.hash(1));
}

TEST(tree_traversal, test_compound) {
    QuarternaryTree tree;
    tree.addString("uu");
    TreeTraversalNode traversal(&tree);
    traversal.moveUp();
    traversal.moveUp();
    EXPECT_EQ(75, traversal.hash(0));
}

TEST(tree_traversal, test_expire_node) {
    QuarternaryTree tree;
    tree.addString("ur");
    tree.addString("ud");
    TreeTraversalNode traversal(&tree);
    traversal.moveUp();
    traversal.moveRight();
    EXPECT_EQ(25, traversal.hash(0));
}

TEST(tree_traversal, test_equal) {
    QuarternaryTree tree;
    tree.addString("ur");
    tree.addString("ud");
    TreeTraversalNode traversal1(&tree);
    traversal1.moveUp();
    traversal1.moveRight();
    TreeTraversalNode traversal2(&tree);
    traversal2.moveUp();
    traversal2.moveRight();
    EXPECT_TRUE(traversal1.equals(&traversal2));
}

TEST(tree_traversal, test_not_equal) {
    QuarternaryTree tree;
    tree.addString("ur");
    tree.addString("ud");
    TreeTraversalNode traversal1(&tree);
    traversal1.moveUp();
    traversal1.moveRight();
    TreeTraversalNode traversal2(&tree);
    traversal2.moveUp();
    EXPECT_FALSE(traversal1.equals(&traversal2));
}

TEST(tree_traversal, test_valid) {
    QuarternaryTree tree;
    tree.addString("ud");
    tree.addString("du");
    TreeTraversalNode traversal1(&tree);
    traversal1.moveUp();
    EXPECT_FALSE(traversal1.downValid());
    traversal1.moveDown();
    EXPECT_FALSE(traversal1.upValid());
    EXPECT_TRUE(traversal1.downValid());
    EXPECT_TRUE(traversal1.rightValid());
    EXPECT_TRUE(traversal1.leftValid());
}

TEST(tree_traversal, test_valid_longer) {
    QuarternaryTree tree;
    tree.addString("udd");
    tree.addString("uuu");
    TreeTraversalNode traversal1(&tree);
    traversal1.moveUp();
    EXPECT_TRUE(traversal1.downValid());
    traversal1.moveDown();
    EXPECT_TRUE(traversal1.upValid());
    EXPECT_FALSE(traversal1.downValid());
}

TEST(traversal_hash_map, test_contains) {
    QuarternaryTree tree;
    tree.addString("ur");
    tree.addString("ud");
    TreeTraversalNode* traversal = new TreeTraversalNode(&tree);
    traversal->moveUp();
    traversal->moveRight();
    TraversalHashMap hashmap(4);
    hashmap.insert(traversal, 2);
    EXPECT_TRUE(hashmap.contains(traversal));
    traversal->moveUp();
    EXPECT_FALSE(hashmap.contains(traversal));
}

TEST(traversal_hash_map, test_capacity) {
    TraversalHashMap hashmap(1);
    EXPECT_EQ(6, hashmap.capacity());
}

TEST(traversal_hash_map, test_get) {
    QuarternaryTree tree;
    tree.addString("ur");
    tree.addString("ud");
    TreeTraversalNode* traversal = new TreeTraversalNode(&tree);
    traversal->moveUp();
    traversal->moveRight();
    TraversalHashMap hashmap(4);
    hashmap.insert(traversal, 2);
    EXPECT_EQ(2, hashmap.get(traversal));
}

TEST(traversal_hash_map, test_resize) {
    QuarternaryTree tree;
    tree.addString("uuuuuuu");
    TraversalHashMap hashmap(2);
    TreeTraversalNode* traversal;
    for(int i = 0; i < 7; i++) {
        traversal = new TreeTraversalNode(&tree);
        for(int j = 0; j < i+1; j++) traversal->moveUp();
        hashmap.insert(traversal, i);
    }
    EXPECT_EQ(6, hashmap.get(traversal));
}

TEST(traversal_hash_map, test_resize2) {
    QuarternaryTree tree;
    string s;
    int len = 12;
    for(int i = 0; i < len; i++) s += "u";
    tree.addString(s);
    TraversalHashMap hashmap(2);
    TreeTraversalNode* traversal;
    for(int i = 0; i < len; i++) {
        traversal = new TreeTraversalNode(&tree);
        for(int j = 0; j < i+1; j++) traversal->moveUp();
        hashmap.insert(traversal, i);
        cout << traversal->hash(0) << " at " << i << endl;
    }
    cout << traversal->hash(0) << endl;
    EXPECT_EQ(len-1, hashmap.get(traversal));
}

/*
TEST(traversal_hash_map, test_hash_collisions) {
    QuarternaryTree tree;
    string s;
    int len = 12;
    for(int i = 0; i < len; i++) s += "u";
    tree.addString(s);
    TraversalHashMap hashmap(2);
    TreeTraversalNode* traversal;
    for(int i = 0; i < len; i++) {
        traversal = new TreeTraversalNode(&tree);
        for(int j = 0; j < i+1; j++) traversal->moveUp();
        hashmap.insert(traversal, i);
    }
    EXPECT_LE(hashmap.capacity(), len*3);
}*/

TEST(circular_queue, test_in_out) {
    CircularQueue queue(2);
    QuarternaryTreeNode* n1 = new QuarternaryTreeNode;
    n1->id = 1;
    queue.addStartPopEnd(n1);
    EXPECT_EQ(1, queue[0]->id);
}

#include <iostream>
TEST(circular_queue, test_in_out_twice) {
    CircularQueue queue(3);
    QuarternaryTreeNode* n1 = new QuarternaryTreeNode;
    n1->id = 1;
    QuarternaryTreeNode* n2 = new QuarternaryTreeNode;
    n2->id = 2;
    queue.addStartPopEnd(n1);
    queue.addStartPopEnd(n2);
    EXPECT_EQ(2, queue[0]->id);
    EXPECT_EQ(1, queue[1]->id);
}

TEST(circular_queue, test_pop_end) {
    CircularQueue queue(2);
    QuarternaryTreeNode* n1 = new QuarternaryTreeNode;
    n1->id = 1;
    QuarternaryTreeNode* n2 = new QuarternaryTreeNode;
    n2->id = 2;
    QuarternaryTreeNode* n3 = new QuarternaryTreeNode;
    n3->id = 3;
    queue.addStartPopEnd(n1);
    queue.addStartPopEnd(n2);
    queue.addStartPopEnd(n3);
    queue.addStartPopEnd(n1);
    queue.addStartPopEnd(n2);
    queue.addStartPopEnd(n3);
    cout << queue[0] << ", " << queue[1] << endl;
    EXPECT_EQ(3, queue[0]->id);
    EXPECT_EQ(2, queue[1]->id);
}
