#include "gtest/gtest.h"

#include "linked_hash_map.h"

#include <iostream>

using namespace std;

/*TEST(hashmap, test_false_contain) {
    LinkedHashMap<uint64_t> hashmap(2);
    EXPECT_FALSE(hashmap.contains(1));
}

TEST(hashmap, test_true_contain) {
    LinkedHashMap<uint64_t> hashmap(2);
    hashmap.insert(1, 1);
    EXPECT_TRUE(hashmap.contains(1));
}

TEST(hashmap, test_value_retrieval) {
    LinkedHashMap<uint64_t> hashmap(2);
    hashmap.insert(1, 5);
    EXPECT_EQ(5, hashmap[1]);
}

TEST(hashmap, test_false_overflow_to_next) {
    LinkedHashMap<uint64_t> hashmap(2);
    for(unsigned int i = 1; i < 5+1; i++)
        hashmap.insert(i, 1);
    EXPECT_FALSE(hashmap.contains(6));
}

TEST(hashmap, test_true_overflow_to_next) {
    LinkedHashMap<uint64_t> hashmap(2);
    for(unsigned int i = 1; i < 5+1; i++)
        hashmap.insert(i, 1);
    for(unsigned int i = 1; i < 5+1; i++)
        EXPECT_TRUE(hashmap.contains(i));
}

TEST(hashmap, test_true_overflow_to_third) {
    LinkedHashMap<uint64_t> hashmap(2);
    for(unsigned int i = 1; i < 9+1; i+=2)
        hashmap.insert(i, 1);
    for(unsigned int i = 1; i < 9+1; i+=2)
        EXPECT_TRUE(hashmap.contains(i));
}

TEST(hashmap, test_false_overflow_to_third) {
    LinkedHashMap<uint64_t> hashmap(2);
    for(unsigned int i = 1; i < 9+1; i+=2)
        hashmap.insert(i, 1);
    for(unsigned int i = 1; i < 9+1; i+=2)
        EXPECT_TRUE(hashmap.contains(i));
}

TEST(hashmap, test_resize) {
    LinkedHashMap<uint64_t> hashmap(2);
    for(unsigned int i = 1; i < 13+1; i++)
        hashmap.insert(i, 1);
    EXPECT_LT(12, hashmap.capacity());
}

TEST(hashmap, test_resize_contains_true) {
    LinkedHashMap<uint64_t> hashmap(2);
    for(unsigned int i = 1; i < 13+1; i++)
        hashmap.insert(i, 1);
    EXPECT_TRUE(hashmap.contains(13));
}

TEST(hashmap, test_resize_contains_false) {
    LinkedHashMap<uint64_t> hashmap(2);
    for(unsigned int i = 1; i < 13+1; i++)
        hashmap.insert(i, 1);
    EXPECT_FALSE(hashmap.contains(14));
}

TEST(hashmap, test_size) {
    LinkedHashMap<uint64_t> hashmap(2);
    for(unsigned int i = 1; i < 5+1; i++)
        hashmap.insert(i, 1);
    EXPECT_EQ(5, hashmap.size());
}

TEST(hashmap, test_resize_get) {
    LinkedHashMap<uint64_t> hashmap(2);
    for(unsigned int i = 1; i < 13+1; i++)
        hashmap.insert(i, i);
    EXPECT_EQ(1, hashmap[1]);
    EXPECT_EQ(13, hashmap[13]);
}*/
