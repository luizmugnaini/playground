// ------------------------------------------------------------------------------------------------
// Title: Least recently used cache.
// Category: Medium.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
// ------------------------------------------------------------------------------------------------
// @NOTE: The LRU cache algorithm is stupid. You're basically juggling cold cache lines, which
// makes no sense if all you want is cache temporal locality. You should consider using the most
// recently used (MRU) caching algorithm.
// ------------------------------------------------------------------------------------------------
// Description:
//
// Implement the Least Recently Used (LRU) cache class LRUCache. The class should support the
// following operations
//
// - LRUCache(int capacity) Initialize the LRU cache of size capacity.
// - int get(int key) Return the value coresponding to the key if the key exists, otherwise
//   return -1.
// - void put(int key, int value) Update the value of the key if the key exists. Otherwise, add
//   the key-value pair to the cache. If the introduction of the new pair causes the cache to
//   exceed its capacity, remove the least recently used key.
//
// A key is considered used if a get or a put operation is called on it.
//
// Ensure that get and put each run in O(1) average time complexity.
//
//
// Constraints:
// - 1 <= capacity <= 3000
// - 0 <= key <= 10^4
// - 0 <= value <= 10^5
// - At most 2 * 10^5 calls will be made to get and put.
// ------------------------------------------------------------------------------------------------

#include <common.hpp>
#include <list>
#include <unordered_map>

struct BadLRUCache {
    using Key   = int32_t;
    using Value = int32_t;

    struct ValueInfo {
        Value                    value;
        std::list<Key>::iterator priority_position;
    };

    size_t                             cache_capacity;
    std::unordered_map<Key, ValueInfo> cache;
    std::list<Key>                     priorities;

    BadLRUCache(size_t capacity) {
        this->cache_capacity = capacity;
    }

    Value get(Key key) {
        Value value = -1;

        if (auto pair = this->cache.find(key); pair != this->cache.end()) {
            // Update the priority position of the key, put it in front.
            this->priorities.erase(pair->second.priority_position);
            this->priorities.push_front(key);
            pair->second.priority_position = this->priorities.begin();

            value = pair->second.value;
        }

        return value;
    }

    void put(Key key, Value value) {
        if (auto pair = this->cache.find(key); pair != this->cache.end()) {
            this->priorities.erase(pair->second.priority_position);
            this->priorities.push_front(key);

            pair->second.value             = value;
            pair->second.priority_position = this->priorities.begin();
        } else {
            if (this->cache.size() == this->cache_capacity) {
                Key least_priority_key = this->priorities.back();
                this->priorities.pop_back();

                auto least_priority_element = this->cache.find(least_priority_key);
                this->cache.erase(least_priority_element);
            }

            // Insert the new key-value pair as the top-priority element.
            this->priorities.push_front(key);
            this->cache.insert(std::make_pair(key, ValueInfo{value, this->priorities.begin()}));
        }
    }
};

// @NOTE: Too many pointer dereferences for my taste. Slow, computer no go vroom vroom. Me no likey :(
struct LRUCache {
    using Key   = int32_t;
    using Value = int32_t;

    static constexpr Key   MAX_KEY       = 10'000;
    static constexpr Value INVALID_VALUE = -1;

    struct CacheNode {
        CacheNode* next_node  = nullptr;
        CacheNode* prior_node = nullptr;
        Value      value      = INVALID_VALUE;
    };

    CacheNode cache[MAX_KEY + 1];
    size_t    cache_capacity = 0;
    size_t    cache_size     = 0;

    // The start endpoint next_node is the actual first node (lowest priority) of the cache,
    // and the prior_node is end endpoint.
    //
    // @NOTE: This is not a valid node of the cache.
    CacheNode start = {};

    // The end endpoint next_node is start, and the prior_node is the actual last
    // node (highest priority) of the cache, and the prior_node is start endpoint.
    //
    // @NOTE: This is not a valid node of the cache.
    CacheNode end = {};

    LRUCache(size_t capacity) {
        this->cache_capacity = capacity;

        this->start.next_node = &this->end;
        this->end.prior_node  = &this->start;
    }

    void delete_node(CacheNode* node) {
        // Connect the neighbours of the node.
        node->prior_node->next_node = node->next_node;
        node->next_node->prior_node = node->prior_node;

        // Reset the node state.
        node->next_node  = nullptr;
        node->prior_node = nullptr;

        --this->cache_size;
    }

    void add_node(CacheNode* node) {
        // Put the current last node before the given node.
        CacheNode* previous_last_node = this->end.prior_node;
        previous_last_node->next_node = node;
        node->prior_node              = previous_last_node;

        // Make node the current last node.
        node->next_node      = &this->end;
        this->end.prior_node = node;

        ++this->cache_size;
    }

    Value get(Key key) {
        assert(key <= MAX_KEY);

        CacheNode* node = &this->cache[key];

        Value value;
        if (node->prior_node == nullptr) {
            // This node was never initialized or got deleted.
            value = INVALID_VALUE;
        } else {
            value = node->value;

            // Promote the node to highest priority.
            this->delete_node(node);
            this->add_node(node);
        }

        return value;
    }

    void put(Key key, Value value) {
        assert(key <= MAX_KEY);

        CacheNode* node = &this->cache[key];

        if (node->prior_node != nullptr) {
            // The node is already initialized, update its value.
            node->value = value;

            // Put the node as the last one.
            this->delete_node(node);
            this->add_node(node);
        } else {
            if (this->cache_size == this->cache_capacity) {
                // Evict the first node.
                this->delete_node(this->start.next_node);
            }

            node->value = value;
            this->add_node(node);
        }
    }
};

int main() {
    // Bad LRU test 1.
    {
        BadLRUCache lru{2};

        lru.put(1, 10);             // Cache: {1 = 10}
        assert_eq(lru.get(1), 10);  // Cache: {1 = 10}
        lru.put(2, 20);             // Cache: {1 = 10, 2 = 20}
        lru.put(3, 30);             // Cache: {2 = 20, 3 = 30}, key 1 was evicted
        assert_eq(lru.get(2), 20);  // Cache: {3 = 30, 2 = 20}
        assert_eq(lru.get(1), -1);  // Cache: {3 = 30, 2 = 20}
        lru.put(4, 40);             // Cache: {2 = 20, 4 = 40}, key 3 was evicted
        assert_eq(lru.get(3), -1);  // Cache: {2 = 20, 4 = 40}
    }

    // Bad LRU test 2.
    {
        BadLRUCache lru{2};

        assert_eq(lru.get(2), -1);  // Cache: {}
        lru.put(2, 6);              // Cache: {2 = 6}
        assert_eq(lru.get(2), 6);   // Cache: {2 = 6}
        assert_eq(lru.get(1), -1);  // Cache: {2 = 6}
        lru.put(1, 5);              // Cache: {2 = 6, 1 = 5}
        lru.put(1, 2);              // Cache: {2 = 6, 1 = 2}
        assert_eq(lru.get(1), 2);   // Cache: {2 = 6, 1 = 2}
        assert_eq(lru.get(2), 6);   // Cache: {2 = 6, 1 = 2}
    }

    // Test 1.
    {
        LRUCache lru{2};

        lru.put(1, 10);             // Cache: {1 = 10}
        assert_eq(lru.get(1), 10);  // Cache: {1 = 10}
        lru.put(2, 20);             // Cache: {1 = 10, 2 = 20}
        lru.put(3, 30);             // Cache: {2 = 20, 3 = 30}, key 1 was evicted
        assert_eq(lru.get(2), 20);  // Cache: {3 = 30, 2 = 20}
        assert_eq(lru.get(1), -1);  // Cache: {3 = 30, 2 = 20}
        lru.put(4, 40);             // Cache: {2 = 20, 4 = 40}, key 3 was evicted
        assert_eq(lru.get(3), -1);  // Cache: {2 = 20, 4 = 40}
    }

    // Test 2.
    {
        LRUCache lru{2};

        assert_eq(lru.get(2), -1);  // Cache: {}
        lru.put(2, 6);              // Cache: {2 = 6}
        assert_eq(lru.get(2), 6);   // Cache: {2 = 6}
        assert_eq(lru.get(1), -1);  // Cache: {2 = 6}
        lru.put(1, 5);              // Cache: {2 = 6, 1 = 5}
        lru.put(1, 2);              // Cache: {2 = 6, 1 = 2}
        assert_eq(lru.get(1), 2);   // Cache: {2 = 6, 1 = 2}
        assert_eq(lru.get(2), 6);   // Cache: {2 = 6, 1 = 2}
    }

    report_success();
    return 0;
}
