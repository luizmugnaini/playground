#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>
#include <vector>
#include "common.hpp"

template <typename T>
struct BinaryTree {
    // -----------------------------------------------------------------------------
    // Static methods, associated structs, and constants.
    // -----------------------------------------------------------------------------
    struct Node {
        ptrdiff_t left_node_idx  = LEAF_NODE;
        ptrdiff_t right_node_idx = LEAF_NODE;
        T         value;
    };

    enum struct ChildSide {
        LEFT = 0,
        RIGHT,
    };

    struct NodeSearchResult {
        ptrdiff_t parent_of_node_idx = INVALID_NODE_INDEX;
        ptrdiff_t node_idx           = INVALID_NODE_INDEX;
        ChildSide side_wrt_parent;
    };

    // Enough space a pair of nodes (2) and their child nodes (4).
    static constexpr size_t MIN_NEW_NODE_BLOCK = 6;

    static constexpr T INVALID_VALUE = std::numeric_limits<T>::max();

    static constexpr ptrdiff_t ROOT_NODE           = 0;
    static constexpr ptrdiff_t LEAF_NODE           = -1;
    static constexpr ptrdiff_t NODE_ALREADY_EXISTS = -2;
    static constexpr ptrdiff_t INVALID_NODE_INDEX  = -3;

    static constexpr ptrdiff_t parent_of_child_node(ptrdiff_t child_node_idx) {
        return (child_node_idx & 0b1) ? (child_node_idx / 2) - 1 : (child_node_idx - 1) / 2;
    }

    static constexpr ptrdiff_t left_child_index_from_node(ptrdiff_t current_node_idx) {
        return 2 * current_node_idx + 1;
    }

    static constexpr ptrdiff_t right_child_index_from_node(ptrdiff_t current_node_idx) {
        return 2 * (current_node_idx + 1);
    }

    // -----------------------------------------------------------------------------
    // Members.
    // -----------------------------------------------------------------------------

    std::vector<Node> memory;

    // -----------------------------------------------------------------------------
    // Methods.
    // -----------------------------------------------------------------------------

    BinaryTree()  = delete;
    ~BinaryTree() = default;

    BinaryTree(T root_value, size_t initial_capacity) {
        this->memory.reserve(max_value(initial_capacity, MIN_NEW_NODE_BLOCK));

        // Put space for the two root child nodes.
        this->memory.resize(MIN_NEW_NODE_BLOCK);
        this->memory[ROOT_NODE] = {
            .left_node_idx  = LEAF_NODE,
            .right_node_idx = LEAF_NODE,
            .value          = root_value,
        };
    }

    T min() const {
        ptrdiff_t current_node_idx = ROOT_NODE;
        ptrdiff_t next_child_idx   = this->memory[ROOT_NODE].left_node_idx;
        while (next_child_idx >= 0) {
            current_node_idx = next_child_idx;
            next_child_idx   = this->memory[next_child_idx].left_node_idx;
        }

        return this->memory[current_node_idx].value;
    }

    T max() const {
        ptrdiff_t current_node_idx = ROOT_NODE;
        ptrdiff_t next_child_idx   = this->memory[ROOT_NODE].right_node_idx;
        while (next_child_idx >= 0) {
            current_node_idx = next_child_idx;
            next_child_idx   = this->memory[next_child_idx].right_node_idx;
        }

        return this->memory[current_node_idx].value;
    }

    // @NOTE: Bad and inefficient.
    size_t impl_max_depth(ptrdiff_t current_node_idx) const {
        if (current_node_idx < 0) {
            return 0;
        }

        Node const& current_node = this->memory[current_node_idx];
        return 1u + max_value(impl_max_depth(current_node.left_node_idx), impl_max_depth(current_node.right_node_idx));
    }

    size_t max_depth() const {
        return impl_max_depth(ROOT_NODE);
    }

    ptrdiff_t find_node(T value) const {
        ptrdiff_t current_node_idx = ROOT_NODE;
        while (current_node_idx >= 0) {
            Node current_node = this->memory[current_node_idx];
            if (current_node.value == value) {
                break;
            } else if (current_node.value > value) {
                current_node_idx = current_node.left_node_idx;
            } else {
                current_node_idx = current_node.right_node_idx;
            }
        }

        return (current_node_idx >= 0) ? current_node_idx : INVALID_NODE_INDEX;
    }

    ptrdiff_t insert_node(T value) {
        NodeSearchResult traversal_result;
        this->impl_traverse_to_node_index(traversal_result, value, ROOT_NODE);

        if (traversal_result.node_idx == NODE_ALREADY_EXISTS) {
            // Do nothing.
        } else if (traversal_result.parent_of_node_idx == INVALID_NODE_INDEX) {
            // The only node whose parent doesn't exist is the root.
            if (this->memory[ROOT_NODE].value != value) {
                this->memory[ROOT_NODE].value = value;
            } else {
                traversal_result.node_idx = NODE_ALREADY_EXISTS;
            }
        } else {
            if (traversal_result.node_idx >= static_cast<ptrdiff_t>(this->memory.size())) {
                this->memory.resize(static_cast<size_t>(traversal_result.node_idx) + MIN_NEW_NODE_BLOCK);
            }

            Node& new_node    = this->memory[traversal_result.node_idx];
            Node& parent_node = this->memory[traversal_result.parent_of_node_idx];

            switch (traversal_result.side_wrt_parent) {
                case ChildSide::LEFT:  parent_node.left_node_idx = traversal_result.node_idx; break;
                case ChildSide::RIGHT: parent_node.right_node_idx = traversal_result.node_idx; break;
            }

            new_node.value = value;
        }

        return traversal_result.node_idx;
    }

    // -----------------------------------------------------------------------------
    // Implementation details.
    // -----------------------------------------------------------------------------

    void impl_traverse_to_node_index(NodeSearchResult& result, T value, ptrdiff_t current_node_idx) {
        assert(current_node_idx != LEAF_NODE);

        if (current_node_idx >= static_cast<ptrdiff_t>(this->memory.size())) {
            // If the node doesn't even exist, then it surely is a vacant leaf.
            result.node_idx = current_node_idx;
            return;
        }

        Node node = this->memory[current_node_idx];

        ptrdiff_t next_child_idx = INVALID_NODE_INDEX;

        if (node.value == value) {
            result.node_idx = NODE_ALREADY_EXISTS;
        } else if (value < node.value) {
            if (node.left_node_idx == LEAF_NODE) {
                // Success found to the left of the current node.
                result.parent_of_node_idx = current_node_idx;
                result.node_idx           = this->left_child_index_from_node(current_node_idx);
                result.side_wrt_parent    = ChildSide::LEFT;
            } else {
                next_child_idx = node.left_node_idx;
            }
        } else {
            if (node.right_node_idx == LEAF_NODE) {
                // Success found to the right of the current node.
                result.parent_of_node_idx = current_node_idx;
                result.node_idx           = this->right_child_index_from_node(current_node_idx);
                result.side_wrt_parent    = ChildSide::RIGHT;
            } else {
                next_child_idx = node.right_node_idx;
            }
        }

        if (next_child_idx != INVALID_NODE_INDEX) {
            // Continue the search.
            this->impl_traverse_to_node_index(result, value, next_child_idx);
        }
    }
};
