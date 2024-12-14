#include <binary_tree.hpp>
#include <common.hpp>

int main() {
    // [5, 2, 6, 0, 3, ., 9, -3, .]
    BinaryTree<int32_t> bt{5, 3};

    assert_eq(bt.insert(5), BinaryTree<int32_t>::NODE_ALREADY_EXISTS);
    assert_eq(bt.min(), 5);

    assert_eq(bt.insert(2), 1);
    assert_eq(bt.min(), 2);

    assert_eq(bt.insert(6), 2);

    assert_eq(bt.min(), 2);

    assert_eq(bt.insert(1), 3);
    assert_eq(bt.min(), 1);

    assert_eq(bt.insert(-3), 7);
    assert_eq(bt.min(), -3);

    assert_eq(bt.insert(-3), BinaryTree<int32_t>::NODE_ALREADY_EXISTS);

    assert_eq(bt.insert(3), 4);
    assert_eq(bt.min(), -3);

    assert_eq(bt.insert(5), BinaryTree<int32_t>::NODE_ALREADY_EXISTS);
    assert_eq(bt.min(), -3);

    assert_eq(bt.insert(9), 6);
    assert_eq(bt.min(), -3);

    assert_eq(bt.insert(3), BinaryTree<int32_t>::NODE_ALREADY_EXISTS);
    assert_eq(bt.min(), -3);

    report_success();
    return 0;
}
