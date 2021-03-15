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

static const uint8_t hashtree_max_level = 9;
static const uint8_t hashtree_prime[hashtree_max_level - 1] = {29, 23, 19, 17,
                                                               13, 11, 7,  5};

template <typename T>
struct hashtree {
private:
    struct node {
        node(uint64_t hashcode, std::shared_ptr<T> data)
            : hashcode(hashcode), data(data) {}

        node(uint64_t hashcode) : node(hashcode, nullptr) {}

        node() : node(0llu, nullptr) {}

        node(std::shared_ptr<node> ptr)
            : node(ptr->hashcode, ptr->data), children(ptr->children) {}

        node& operator=(std::shared_ptr<node> ptr) {
            hashcode = ptr->hashcode;
            data = ptr->data;
            children = ptr->children;
        }

        uint64_t hashcode;

        std::shared_ptr<T> data;

        std::vector<std::shared_ptr<node>> children;

    } __attribute__((packed));

public:
    hashtree(uint32_t level1) : _level1(level1), _size(0llu) {
        _head.resize(_level1);
    }

    hashtree(uin32_t level1) : hashtree(31) {}

    // todo(copy on write)
    hashtree(const hashtree&) = delete;

    hashtree(hashtree&&);

    // todo(copy on write)
    hashtree& operator=(const hashtree&) = delete;

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
    int insert(uint64_t hashcode, const T& data);
    int insert(uint64_t hashcode, T&& data);

    // Removes specified elements from the container. Returns the data address
    // if the element exist.
    std::shared_ptr<T> erase(uint64_t hashcode);

private:
    int insert(uint64_t hashcode, std::shared_ptr<T> data);

    int fixup_children(std::shared_ptr<node>, uint8_t level);

    uint32_t _level1;  // 质数
    uint64_t _size;
    std::vector<std::shared_ptr<node>> _head;
};  // struct hashtree

template <typename T>
hashtree<T>::hashtree(hashtree<T>&& other) {
    _level1 = other._level1;
    _size = other._size;
    _head = other._head;

    other._level1 = 0u;
    other._size = 0llu;
    other._head.clear();
}

template <typename T>
hashtree<T>& hashtree<T>::operator=(hashtree<T>&& other) {
    _level1 = other._level1;
    _size = other._size;
    _head = other._head;

    other._level1 = 0u;
    other._size = 0llu;
    other._head.clear();
}

template <typename T>
void hashtree<T>::clear() noexcept {
    _size = 0llu;
    _head.clear();
}

template <typename T>
void hashtree<T>::swap(hashtree<T>& other) {
    std::swap(_level1, other._level1);
    std::swap(_size, other._size);
    std::swap(_head, other._head);
}

template <typename T>
uint64_t hashtree<T>::size() const noexcept {
    return _size;
}

template <typename T>
int hashtree<T>::fixup_children(std::shared_ptr<node> p, uint8_t level) {
    if (p.size() == level) {
        return 0;
    } else if (p.size() > level) {
        return -1;
    } else if ((level + 1) < p.size() * 2) {
        return 0;
    }

    uint8_t size = p->children.size();
    std::vector<std::shared_ptr<node>> tmp;
    tmp.resize(level);

    for (auto& it : p->children) {
        tmp[(it->hashcode) % level] = it;
    }

    p->children.swap(tmp);
    return 0;
}

template <typename T>
uint64_t hashtree<T>::count(uint64_t hashcode) const noexcept {
    uint64_t slot = hashcode % _level1;

    if (_head[slot] == nullptr) {
        return 0;
    }

    uint64_t count = (hashcode == _head[slot]->hashcode);

    std::shared_ptr<node> cur = _head[slot];
    for (uint8_t level = 1; level < hashtree_max_level; ++level) {
        uint8_t hashslots = hashtree_prime[level - 1];
        slot = hashcode % hashslots;

        if (cur->children.size() != hashslots) {
            bool found = false;

            for (auto& it : cur->children) {
                if (it->hashcode % hashslots == slot) {
                    found = true;
                    count += (it->hashcode == hashcode);
                    cur = it;
                    break;
                }
            }

            if (!found) {
                return count;
            }
        } else {
            if (cur->children[slot] == nullptr) {
                return count;
            } else {
                cur = cur->children[slot];
                count += (cur->hashcode == hashcode);
            }
        }
    }

    for (auto& it : cur->children) {
        count += (it->hashcode == hashcode);
    }
    return count;
}

template <typename T>
int hashtree<T>::insert(uint64_t hashcode, std::shared_ptr<T> data) {
    uint64_t slot = hashcode % _level1;

    if (_head[slot] == nullptr) {
        _head[slot] = std::make_shared<node>(hashcode, data);
        ++_size;
        return 0;
    }

    std::shared_ptr<node> cur = _head[slot];
    for (uint8_t level = 1; level < hashtree_max_level; ++level) {
        uint8_t hashslots = hashtree_prime[level - 1];
        slot = hashcode % hashslots;

        if (cur->children.size() != hashslots) {
            bool found = false;

            for (auto& it : cur->children) {
                if (it->hashcode % hashslots == slot) {
                    found = true;
                    cur = it;
                    break;
                }
            }

            if (!found) {
                cur->children.push_back(std::make_shared<node>(hashcode, data));
                fixup_children(cur);
            }
        } else {
            if (cur->children[slot] == nullptr) {
                cur->children[slot]->hashcode = hashcode;
                cur->children[slot]->data = data;
                return 0;
            } else {
                cur = cur->children[slot];
            }
        }
    }

    cur->children.push_back(std::make_shared<node>(hashcode, data));
    ++_size;

    return 0;
}

template <typename T>
int hashtree<T>::insert(uint64_t hashcode, const T& data) {
    return insert(hashcode, std::make_shared<T>(data));
}

template <typename T>
int hashtree<T>::insert(uint64_t hashcode, T&& data) {
    return insert(hashcode, std::make_shared<T>(std::move(data)));
}

template <typename T>
std::shared_ptr<T> hashtree<T>::erase(uint64_t hashcode) {
    uint64_t slot = hashcode % _level1;

    if (_head[slot] == nullptr) {
        return nullptr;
    }

    std::shared_ptr<node> last = nullptr;
    std::shared_ptr<node> cur = _head[slot];
    std::shared_ptr<T> res;

    uint8_t level = 1;
    for (; level < hashtree_max_level; ++level) {
        uint8_t hashslots = hashtree_prime[level - 1];
        slot = hashcode % hashslots;

        if (cur->children.size() != hashslots) {
            bool found = false;

            for (auto& it : cur->children) {
                if (it->hashcode % hashslots == slot) {
                    found = true;
                    if (it->hashcode == hashcode) {
                        res = it->data;
                    }
                    last = cur;
                    cur = it;
                    break;
                }
            }

            if (!found) {
                return nullptr;
            }
        } else {
            if (cur->children[slot] == nullptr) {
                return nullptr;
            } else {
                last = cur;
                cur = cur->children[slot];
                if (cur->hashcode == hashcode) {
                    res = it->data;
                }
            }
        }

        if (res != nullptr) {
            break;
        }
    }

    if (res == nullptr) {
        for (auto& it : cur->children) {
            if (it->hashcode == hashcode) {
                res = it->data;
                cur->children.erase(it);
                break;
            }
        }
    } else {
        while (++level < hashtree_max_level && cur->children.size() > 0) {
            cur->data = cur->children[0].data;
            last = cur;
            cur = cur->children[0];
        }

        cur->data = nullptr;
        if (level == hashtree_max_level) {
            for (auto& it : last->children) {
                if (cur->data == nullptr) {
                    last->children.erase(cur);
                }
            }
        } else if (last->children.size() != hashtree_prime[level]) {
            for (auto& it : last->children) {
                if (cur->data == nullptr) {
                    last->children.erase(cur);
                }
            }
        }
    }

    return res;
}

#endif  //__HASHTREE_H__
