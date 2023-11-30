#ifndef BUCKET_QUEUE_H
#define BUCKET_QUEUE_H

#include <map>
#include <memory>
#include <stack>

template <class T>
class MinBucketStack {
public:

    void add(int cost, T object) noexcept {
        if(!buckets.contains(cost)) {
            buckets[cost] = std::make_shared<std::stack<T>>();
        }
        buckets[cost]->push(object);
        _size++;
    }

    T top() const noexcept {
        return buckets.begin()->second->top();
    }

    void pop() noexcept {
        buckets.begin()->second->pop();
        if(buckets.begin()->second->empty()) buckets.erase(buckets.begin());
        _size--;
    }

    unsigned int size() const noexcept {
        return _size;
    }

    bool empty() const noexcept {
        return _size == 0;
    }

private:
    std::map<int, std::shared_ptr<std::stack<T>>> buckets;
    unsigned int _size = 0;
};

#endif // BUCKET_QUEUE_H
