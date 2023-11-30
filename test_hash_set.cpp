#include "gtest/gtest.h"

#include "uint64_hash_set.h"

#include <iostream>

using namespace std;

TEST(hashset, test_false_contain) {
    Uint64HashSet hashset(2);
    EXPECT_FALSE(hashset.contains(1));
}

TEST(hashset, test_true_contain) {
    Uint64HashSet hashset(2);
    hashset.insert(1);
    EXPECT_TRUE(hashset.contains(1));
}

TEST(hashset, test_false_overflow_to_next) {
    Uint64HashSet hashset(2);
    for(unsigned int i = 1; i < 5+1; i++)
        hashset.insert(i);
    EXPECT_FALSE(hashset.contains(6));
}

TEST(hashset, test_true_overflow_to_next) {
    Uint64HashSet hashset(2);
    for(unsigned int i = 1; i < 5+1; i++)
        hashset.insert(i);
    for(unsigned int i = 1; i < 5+1; i++)
        EXPECT_TRUE(hashset.contains(i));
}

TEST(hashset, test_true_overflow_to_third) {
    Uint64HashSet hashset(2);
    for(unsigned int i = 1; i < 9+1; i+=2)
        hashset.insert(i);
    for(unsigned int i = 1; i < 9+1; i+=2)
        EXPECT_TRUE(hashset.contains(i));
}

TEST(hashset, test_false_overflow_to_third) {
    Uint64HashSet hashset(2);
    for(unsigned int i = 1; i < 9+1; i+=2)
        hashset.insert(i);
    for(unsigned int i = 1; i < 9+1; i+=2)
        EXPECT_TRUE(hashset.contains(i));
}

TEST(hashset, test_resize) {
    Uint64HashSet hashset(2);
    for(unsigned int i = 1; i < 13+1; i++)
        hashset.insert(i);
    EXPECT_LT(12, hashset.capacity());
}

TEST(hashset, test_resize_contains_true) {
    Uint64HashSet hashset(2);
    for(unsigned int i = 1; i < 13+1; i++)
        hashset.insert(i);
    EXPECT_TRUE(hashset.contains(13));
}

TEST(hashset, test_resize_contains_false) {
    Uint64HashSet hashset(2);
    for(unsigned int i = 1; i < 13+1; i++)
        hashset.insert(i);
    EXPECT_FALSE(hashset.contains(14));
}

TEST(hashset, test_size) {
    Uint64HashSet hashset(2);
    for(unsigned int i = 1; i < 5+1; i++)
        hashset.insert(i);
    EXPECT_EQ(5, hashset.size());
}
