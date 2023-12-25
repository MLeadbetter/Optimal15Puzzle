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

TEST(tree_traversal, test_simple_tree_move) {
    QuarternaryTree tree;
    tree.addString("u");
    TreeTraversalNode traversal;
    traversal.setTree(&tree);
    traversal.moveUp();
    EXPECT_EQ(3, traversal.hash(0));
}

TEST(tree_traversal, test_simple_tree_hash) {
    QuarternaryTree tree;
    tree.addString("u");
    TreeTraversalNode traversal;
    traversal.setTree(&tree);
    traversal.moveUp();
    EXPECT_EQ(5, traversal.hash(1));
}

TEST(tree_traversal, test_compound) {
    QuarternaryTree tree;
    tree.addString("uu");
    TreeTraversalNode traversal;
    traversal.setTree(&tree);
    traversal.moveUp();
    traversal.moveUp();
    EXPECT_EQ(75, traversal.hash(0));
}

TEST(tree_traversal, test_expire_node) {
    QuarternaryTree tree;
    tree.addString("ur");
    tree.addString("ud");
    TreeTraversalNode traversal;
    traversal.setTree(&tree);
    traversal.moveUp();
    traversal.moveRight();
    EXPECT_EQ(25, traversal.hash(0));
}

TEST(tree_traversal, test_equal) {
    QuarternaryTree tree;
    tree.addString("ur");
    tree.addString("ud");
    TreeTraversalNode traversal1;
    traversal1.setTree(&tree);
    traversal1.moveUp();
    traversal1.moveRight();
    TreeTraversalNode traversal2;
    traversal2.setTree(&tree);
    traversal2.moveUp();
    traversal2.moveRight();
    EXPECT_TRUE(traversal1.equals(&traversal2));
}

TEST(tree_traversal, test_not_equal) {
    QuarternaryTree tree;
    tree.addString("ur");
    tree.addString("ud");
    TreeTraversalNode traversal1;
    traversal1.setTree(&tree);
    traversal1.moveUp();
    traversal1.moveRight();
    TreeTraversalNode traversal2;
    traversal2.setTree(&tree);
    traversal2.moveUp();
    EXPECT_FALSE(traversal1.equals(&traversal2));
}

TEST(tree_traversal, test_valid) {
    QuarternaryTree tree;
    tree.addString("ud");
    tree.addString("du");
    TreeTraversalNode traversal1;
    traversal1.setTree(&tree);
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
    TreeTraversalNode traversal1;
    traversal1.setTree(&tree);
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
    TreeTraversalNode* traversal = new TreeTraversalNode();
    traversal->setTree(&tree);
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
    TreeTraversalNode* traversal = new TreeTraversalNode();
    traversal->setTree(&tree);
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
        traversal = new TreeTraversalNode();
        traversal->setTree(&tree);
        for(int j = 0; j < i+1; j++) traversal->moveUp();
        hashmap.insert(traversal, i);
    }
    EXPECT_EQ(6, hashmap.get(traversal));
}
