#include "gtest/gtest.h"

#include "bucket_queue.h"

using namespace std;

TEST(bucket_queue, test_one_top) {
    MinBucketStack<unsigned int> queue;
    queue.add(1, 1);
    EXPECT_EQ(1, queue.top());
}

TEST(bucket_queue, test_min) {
    MinBucketStack<unsigned int> queue;
    queue.add(2, 2);
    queue.add(1, 1);
    queue.add(3, 3);
    EXPECT_EQ(1, queue.top());
}

TEST(bucket_queue, test_pop_list) {
    MinBucketStack<unsigned int> queue;
    queue.add(2, 2);
    queue.add(1, 1);
    queue.add(3, 3);
    queue.pop();
    EXPECT_EQ(2, queue.top());
}

TEST(bucket_queue, test_pop_list_element) {
    MinBucketStack<unsigned int> queue;
    queue.add(2, 2);
    queue.add(3, 3);
    queue.add(1, 1);
    queue.pop();
    queue.pop();
    EXPECT_EQ(3, queue.top());
}

TEST(bucket_queue, test_size) {
    MinBucketStack<unsigned int> queue;
    queue.add(2, 2);
    queue.add(3, 3);
    queue.add(1, 1);
    EXPECT_EQ(3, queue.size());
}

TEST(bucket_queue, test_size_pop) {
    MinBucketStack<unsigned int> queue;
    queue.add(2, 2);
    queue.add(3, 3);
    queue.add(1, 1);
    queue.pop();
    queue.pop();
    EXPECT_EQ(1, queue.size());
}

TEST(bucket_queue, test_empty) {
    MinBucketStack<unsigned int> queue;
    EXPECT_TRUE(queue.empty());
}

TEST(bucket_queue, test_popped_empty) {
    MinBucketStack<unsigned int> queue;
    queue.add(1, 1);
    queue.pop();
    EXPECT_TRUE(queue.empty());
}

TEST(bucket_queue, test_not_empty) {
    MinBucketStack<unsigned int> queue;
    queue.add(1, 1);
    EXPECT_FALSE(queue.empty());
}
