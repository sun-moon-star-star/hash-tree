/**
 * @file hashtree.h
 * @author rongming-lu (823952051@qq.com)
 * @brief hashtree
 * @version 0.1
 * @date 2021-03-12
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef __HASHTREE_H__
#define __HASHTREE_H__

#include <cstdint>
#include <memory>

template <typename T>
struct hashtree {
private:
    struct node {
        node(uint64_t hashcode, std::shared_ptr<T> data)
            : hashcode(hashcode),
              data(data),
              children(nullptr),
              brother(nullptr) {}
        node(uint64_t hashcode) : node(hashcode, nullptr) {}
        node() : node(0llu, nullptr) {}

        // 1 means is leaf, means no children, redundant elements link after
        // 2 means children is array
        uint8_t type;

        uint8_t slot;    // hashcode % father base unless root
        uint32_t count;  // count(childrens + it)

        uint64_t hashcode;

        std::shared_ptr<T> data;

        std::shared_ptr<node> children;
        std::shared_ptr<node> brother;
    } __attribute__((packed));

public:
    hashtree(uint32_t level1) : _level1(level1), _size(0llu), _head(nullptr) {}

    hashtree(const hashtree&);

    hashtree(hashtree&&);

    hashtree& operator=(const hashtree&);

    hashtree& operator=(hashtree&&);

    // Erases all elements from the container. After this call, size() returns
    // zero.
    void clear() noexcept;

    // Exchanges the contents of the container with those of other.
    void swap(hashtree&);

    // The number of elements in the container.
    uint64_t size() const noexcept;

    // Returns the number of elements with key that compares equivalent to the
    // specified argument.
    uint64_t count(uint64_t hashcode) const noexcept;

    // Inserts element into the container, if the container doesn't already
    // contain an element with an equivalent key.
    int insert(uint64_t hashcode, std::shared_ptr<T> data);
    int insert(uint64_t hashcode, const T& data);
    int insert(uint64_t hashcode, T&& data);

    // Removes specified elements from the container. Returns the data address
    // if the element exist.
    std::shared_ptr<T> erase(uint64_t hashcode);

private:
    static const uint8_t _prime[7];
    uint32_t _level1;  // 质数

    uint64_t _size;
    std::shared_ptr<node> _head;
};  // struct hashtree

template <typename T>
const uint8_t hashtree<T>::_prime[7] = {23, 19, 17, 13, 11, 7, 5};

template <typename T>
hashtree<T>::hashtree(const hashtree<T>& other) {
    _size = other._size;
    _level1 = other._level1;
    _head = other._head;
}

template <typename T>
hashtree<T>::hashtree(hashtree<T>&& other) {
    _size = other._size;
    _level1 = other._level1;
    _head = other._head;

    other._size = 0llu;
    other._level1 = 0llu;
    other._head = nullptr;
}

template <typename T>
hashtree<T>& hashtree<T>::operator=(const hashtree<T>& other) {
    _size = other._size;
    _level1 = other._level1;
    _head = other._head;
}

template <typename T>
hashtree<T>& hashtree<T>::operator=(hashtree<T>&& other) {
    _size = other._size;
    _level1 = other._level1;
    _head = other._head;

    other._size = 0llu;
    other._level1 = 0llu;
    other._head = nullptr;
}

template <typename T>
void hashtree<T>::clear() noexcept {
    _size = 0llu;
    _head = nullptr;
}

template <typename T>
void hashtree<T>::swap(hashtree<T>& other) {
    hashtree<T> tmp = other;
    x = other;
    other = tmp;
}

template <typename T>
uint64_t hashtree<T>::size() const noexcept {
    return _size;
}

template <typename T>
uint64_t hashtree<T>::count(uint64_t hashcode) const noexcept {}

template <typename T>
int hashtree<T>::insert(uint64_t hashcode, std::shared_ptr<T> data) {
    uint64_t slot = hashcode % _prime[0];
    if (_head[slot] == nullptr) {
        _head[slot] = std::make_shared<node>(hashcode, data);
        return;
    }
}

template <typename T>
int hashtree<T>::insert(uint64_t hashcode, const T& data) {
    if (_head == nullptr) {
        _head = std::make_shared<node>(hashcode, data);
        return;
    }
}

template <typename T>
int hashtree<T>::insert(uint64_t hashcode, T&& data) {
    if (_head == nullptr) {
        _head = std::make_shared<node>(hashcode, data);

        return;
    }
}

template <typename T>
std::shared_ptr<T> hashtree<T>::erase(uint64_t hashcode) {}

#endif  //__HASHTREE_H__
