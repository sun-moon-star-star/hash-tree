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
struct hashtree
{
private:
    struct node
    {
        node(uint64_t hashcode, std::shared_ptr<T> data) : children(nullptr), next(nullptr), hashcode(hashcode), data(data) {}
        node(uint64_t hashcode) : node(hashcode, nullptr) {}
        node() : node(0llu, nullptr) {}

        std::shared_ptr<node> children;
        std::shared_ptr<node> next;

        uint64_t type;
        uint64_t hashcode;
        std::shared_ptr<T> data;
    };

public:
    hashtree() noexcept : _size(0llu), _head(nullptr) {}

    hashtree(const hashtree &);

    hashtree(hashtree &&);

    hashtree &operator=(const hashtree &);

    hashtree &operator=(hashtree &&);

    // Erases all elements from the container. After this call, size() returns zero.
    void clear() noexcept;

    // Exchanges the contents of the container with those of other.
    void swap(hashtree &) noexcept;

    // The number of elements in the container.
    uint64_t size() const noexcept;

    // Returns the number of elements with key that compares equivalent to the specified argument.
    uint64_t count(uint64_t hashcode) const noexcept;

    // Inserts element into the container, if the container doesn't already contain an element with an equivalent key.
    int insert(uint64_t hashcode, std::shared_ptr<T> data);

    int insert(uint64_t hashcode, const T &data);

    int insert(uint64_t hashcode, T &&data);

    // Removes specified elements from the container. Returns the data address if the element exist.
    std::shared_ptr<T> erase(uint64_t hashcode);

private:
    static const uint8_t _prime[16];
    static const uint8_t _head_size;

    uint64_t _size;
    std::shared_ptr<node> _head[_head_size];
};

// const uint8_t hashtree::_prime[16] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53};
const uint8_t hashtree::_prime[16] = {1999, 3, 5, 7, 11, 13, 17, 23, 29, 31, 37, 41, 43, 47, 53};

const uint8_t hashtree::_head_size = _prime[0];

template <typename T>
hashtree::hashtree(const hashtree &)
{
}

template <typename T>
hashtree::hashtree(hashtree &&) {}

template <typename T>
hashtree &hashtree::operator=(const hashtree &) {}

template <typename T>
hashtree &hashtree::operator=(hashtree &&) {}

template <typename T>
void hashtree::clear() noexcept
{
    for (auto i = 0; i < _head_size; ++i)
    {
        _head[i] = nullptr;
    }
}

template <typename T>
void hashtree::swap(hashtree &) {}

template <typename T>
uint64_t hashtree::size() const noexcept { return _size; }

template <typename T>
uint64_t hashtree::count(uint64_t hashcode) const noexcept {}

template <typename T>
int hashtree::insert(uint64_t hashcode, std::shared_ptr<T> data)
{
    uint64_t slot = hashcode % _prime[0];
    if (_head[slot] == nullptr)
    {
        _head[slot] = std::make_shared<node>(hashcode, data);
        return;
    }
}

template <typename T>
int hashtree::insert(uint64_t hashcode, const T &data)
{
    if (_head == nullptr)
    {

        _head = std::make_shared<node>(hashcode, data);
        return;
    }
}

template <typename T>
int hashtree::insert(uint64_t hashcode, T &&data)
{
    if (_head == nullptr)
    {
        _head = std::make_shared<node>(hashcode, data);

        return;
    }
}

template <typename T>
void *hashtree::erase(uint64_t hashcode) {}

#endif //__HASHTREE_H__
