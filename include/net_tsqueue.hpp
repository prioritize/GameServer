#pragma once
#include <deque>
#include <utility>
#include "net_common.hpp"

namespace net {
template <typename T>
class tsqueue {
 public:
    tsqueue() = default;
    tsqueue(const tsqueue<T>&) = delete;
    virtual ~tsqueue() {clear();}

    const T& front() {
        std::scoped_lock lock(muxQueue);
        return deqQueue.front();
    }

    const T& back() {
        std::scoped_lock lock(muxQueue);
        return deqQueue.back();
    }

    void push_back(T& item) {
        std::scoped_lock lock(muxQueue);
        deqQueue.emplace_back(std::move(item));
    }
    void clear() {
        std::scoped_lock lock(muxQueue);
        deqQueue.clear();
    }
    void push_front(T& item) {
        std::scoped_lock lock(muxQueue);
        deqQueue.emplace_front(std::move(item));
    }
    void push_back(T& item) {
        std::scoped_lock lock(muxQueue);
        deqQueue.emplace_back(std::move(item));
    }
    bool empty() {
        std::scoped_lock lock(muxQueue);
        return deqQueue.empty();
    }
    size_t count() {
        std::scoped_lock lock(muxQueue);
        return deqQueue.size();
    }

    T pop_front() {
        std::scoped_lock lock(muxQueue);
        auto item = std::move(deqQueue.front());
        deqQueue.pop_front();
        return item;
    }
    T pop_back() {
        std::scoped_lock lock(muxQueue);
        auto item = std::move(deqQueue.back());
        deqQueue.pop_back();
        return item;
    }

 protected:
    std::mutex muxQueue;
    std::deque<T> deqQueue;

}
};