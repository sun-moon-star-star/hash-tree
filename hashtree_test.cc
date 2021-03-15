/**
 * @file hashtree_test.cc
 * @author rongming-lu (823952051@qq.com)
 * @brief hashtree
 * @version 0.1
 * @date 2021-03-16
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "hashtree.h"

#include <iostream>

void TestBase() {
    hashtree<int> tree;
    assert(tree.size() == 0llu);

    tree.insert(1llu, 12);
    assert(tree.size() == 1llu && tree.count(1llu) == 1llu);
    assert(tree.count(123llu) == 0llu && tree.erase(123llu) == nullptr);

    std::shared_ptr<int> ptr = tree.erase(1llu);
    assert(ptr != nullptr && *ptr == 12 && tree.size() == 0);

    std::cout << "TestBase() pass" << std::endl;
}

int main() {
    TestBase();
    return 0;
}
