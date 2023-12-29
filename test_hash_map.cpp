#include "gtest/gtest.h"

#include "uint64_hash_map.h"

#include <iostream>

using namespace std;

TEST(hashmap, test_false_contain) {
    Uint64HashMap<unsigned int> hashmap(2);
    EXPECT_FALSE(hashmap.contains(1));
}

TEST(hashmap, test_true_contain) {
    Uint64HashMap<unsigned int> hashmap(2);
    hashmap.insert(1, 1);
    EXPECT_TRUE(hashmap.contains(1));
}

TEST(hashmap, test_value_retrieval) {
    Uint64HashMap<unsigned int> hashmap(2);
    hashmap.insert(1, 5);
    EXPECT_EQ(5, hashmap[1]);
}

TEST(hashmap, test_false_overflow_to_next) {
    Uint64HashMap<unsigned int> hashmap(2);
    for(unsigned int i = 1; i < 5+1; i++)
        hashmap.insert(i, 1);
    EXPECT_FALSE(hashmap.contains(6));
}

TEST(hashmap, test_true_overflow_to_next) {
    Uint64HashMap<unsigned int> hashmap(2);
    for(unsigned int i = 1; i < 5+1; i++)
        hashmap.insert(i, 1);
    for(unsigned int i = 1; i < 5+1; i++)
        EXPECT_TRUE(hashmap.contains(i));
}

TEST(hashmap, test_true_overflow_to_third) {
    Uint64HashMap<unsigned int> hashmap(2);
    for(unsigned int i = 1; i < 9+1; i+=2)
        hashmap.insert(i, 1);
    for(unsigned int i = 1; i < 9+1; i+=2)
        EXPECT_TRUE(hashmap.contains(i));
}

TEST(hashmap, test_false_overflow_to_third) {
    Uint64HashMap<unsigned int> hashmap(2);
    for(unsigned int i = 1; i < 9+1; i+=2)
        hashmap.insert(i, 1);
    for(unsigned int i = 1; i < 9+1; i+=2)
        EXPECT_TRUE(hashmap.contains(i));
}

TEST(hashmap, test_resize) {
    Uint64HashMap<unsigned int> hashmap(2);
    for(unsigned int i = 1; i < 13+1; i++)
        hashmap.insert(i, 1);
    EXPECT_LT(12, hashmap.capacity());
}

TEST(hashmap, test_resize_contains_true) {
    Uint64HashMap<unsigned int> hashmap(2);
    for(unsigned int i = 1; i < 13+1; i++)
        hashmap.insert(i, 1);
    EXPECT_TRUE(hashmap.contains(13));
}

TEST(hashmap, test_resize_contains_false) {
    Uint64HashMap<unsigned int> hashmap(2);
    for(unsigned int i = 1; i < 13+1; i++)
        hashmap.insert(i, 1);
    EXPECT_FALSE(hashmap.contains(14));
}

TEST(hashmap, test_size) {
    Uint64HashMap<unsigned int> hashmap(2);
    for(unsigned int i = 1; i < 5+1; i++)
        hashmap.insert(i, 1);
    EXPECT_EQ(5, hashmap.size());
}

TEST(hashmap, test_resize_get) {
    Uint64HashMap<unsigned int> hashmap(2);
    for(unsigned int i = 1; i < 13+1; i++)
        hashmap.insert(i, i);
    EXPECT_EQ(1, hashmap[1]);
    EXPECT_EQ(13, hashmap[13]);
}

TEST(hashmap, test_iterate) {
    Uint64HashMap<unsigned int> hashmap(3);
    for(unsigned int i = 1; i < 13+1; i++)
        hashmap.insert(i, i);
    unsigned int count = 0;
    for(auto a : hashmap) {
        std::cout << a.key << std::endl;
        count++;
        EXPECT_NE(0, a.key);
    }
    EXPECT_EQ(13, count);
}
