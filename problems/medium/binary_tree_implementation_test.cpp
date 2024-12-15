#include <binary_tree.hpp>
#include <common.hpp>

int main() {
    // [5, 2, 6, 1, 3, ., 9, -3, .]
    BinaryTree<int32_t> bt{5, 3};

    assert_eq(bt.insert_node(5), BinaryTree<int32_t>::NODE_ALREADY_EXISTS);
    {
        assert_eq(bt.find_node(5), 0);

        assert_eq(bt.find_node(-10), BinaryTree<int32_t>::INVALID_NODE_INDEX);

        assert_eq(bt.min(), 5);
        assert_eq(bt.max(), 5);
    }

    assert_eq(bt.insert_node(2), 1);
    {
        assert_eq(bt.find_node(5), 0);
        assert_eq(bt.find_node(2), 1);

        assert_eq(bt.find_node(6), BinaryTree<int32_t>::INVALID_NODE_INDEX);
        assert_eq(bt.find_node(9), BinaryTree<int32_t>::INVALID_NODE_INDEX);
        assert_eq(bt.find_node(-39), BinaryTree<int32_t>::INVALID_NODE_INDEX);

        assert_eq(bt.min(), 2);
        assert_eq(bt.max(), 5);
    }

    assert_eq(bt.insert_node(6), 2);
    {
        assert_eq(bt.find_node(5), 0);
        assert_eq(bt.find_node(2), 1);
        assert_eq(bt.find_node(6), 2);

        assert_eq(bt.find_node(-1), BinaryTree<int32_t>::INVALID_NODE_INDEX);
        assert_eq(bt.find_node(1), BinaryTree<int32_t>::INVALID_NODE_INDEX);

        assert_eq(bt.min(), 2);
        assert_eq(bt.max(), 6);
    }

    assert_eq(bt.insert_node(1), 3);
    {
        assert_eq(bt.find_node(5), 0);
        assert_eq(bt.find_node(2), 1);
        assert_eq(bt.find_node(6), 2);
        assert_eq(bt.find_node(1), 3);

        assert_eq(bt.find_node(9), BinaryTree<int32_t>::INVALID_NODE_INDEX);
        assert_eq(bt.find_node(-39), BinaryTree<int32_t>::INVALID_NODE_INDEX);

        assert_eq(bt.min(), 1);
        assert_eq(bt.max(), 6);
    }

    assert_eq(bt.insert_node(-3), 7);
    {
        assert_eq(bt.find_node(5), 0);
        assert_eq(bt.find_node(2), 1);
        assert_eq(bt.find_node(6), 2);
        assert_eq(bt.find_node(1), 3);
        assert_eq(bt.find_node(-3), 7);

        assert_eq(bt.find_node(-2), BinaryTree<int32_t>::INVALID_NODE_INDEX);
        assert_eq(bt.find_node(0), BinaryTree<int32_t>::INVALID_NODE_INDEX);

        assert_eq(bt.min(), -3);
        assert_eq(bt.max(), 6);
    }

    assert_eq(bt.insert_node(-3), BinaryTree<int32_t>::NODE_ALREADY_EXISTS);
    {
        assert_eq(bt.find_node(5), 0);
        assert_eq(bt.find_node(2), 1);
        assert_eq(bt.find_node(6), 2);
        assert_eq(bt.find_node(1), 3);
        assert_eq(bt.find_node(-3), 7);

        assert_eq(bt.find_node(100), BinaryTree<int32_t>::INVALID_NODE_INDEX);
        assert_eq(bt.find_node(98), BinaryTree<int32_t>::INVALID_NODE_INDEX);

        assert_eq(bt.min(), -3);
        assert_eq(bt.max(), 6);
    }

    assert_eq(bt.insert_node(3), 4);
    {
        assert_eq(bt.find_node(5), 0);
        assert_eq(bt.find_node(2), 1);
        assert_eq(bt.find_node(6), 2);
        assert_eq(bt.find_node(1), 3);
        assert_eq(bt.find_node(3), 4);
        assert_eq(bt.find_node(-3), 7);

        assert_eq(bt.min(), -3);
        assert_eq(bt.max(), 6);
    }

    assert_eq(bt.insert_node(5), BinaryTree<int32_t>::NODE_ALREADY_EXISTS);
    {
        assert_eq(bt.find_node(5), 0);
        assert_eq(bt.find_node(2), 1);
        assert_eq(bt.find_node(6), 2);
        assert_eq(bt.find_node(1), 3);
        assert_eq(bt.find_node(3), 4);
        assert_eq(bt.find_node(-3), 7);

        assert_eq(bt.min(), -3);
        assert_eq(bt.max(), 6);
    }

    assert_eq(bt.insert_node(9), 6);
    {
        assert_eq(bt.find_node(5), 0);
        assert_eq(bt.find_node(2), 1);
        assert_eq(bt.find_node(6), 2);
        assert_eq(bt.find_node(1), 3);
        assert_eq(bt.find_node(3), 4);
        assert_eq(bt.find_node(9), 6);
        assert_eq(bt.find_node(-3), 7);

        assert_eq(bt.min(), -3);
        assert_eq(bt.max(), 9);
    }

    assert_eq(bt.insert_node(3), BinaryTree<int32_t>::NODE_ALREADY_EXISTS);
    {
        assert_eq(bt.find_node(5), 0);
        assert_eq(bt.find_node(2), 1);
        assert_eq(bt.find_node(6), 2);
        assert_eq(bt.find_node(1), 3);
        assert_eq(bt.find_node(3), 4);
        assert_eq(bt.find_node(9), 6);
        assert_eq(bt.find_node(-3), 7);

        assert_eq(bt.min(), -3);
        assert_eq(bt.max(), 9);
    }

    report_success();
    return 0;
}
