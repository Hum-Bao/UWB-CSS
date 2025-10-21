#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include <iostream>
#include <memory>
#include <utility>
#include <vector>

static const double LOAD_FACTOR = 0.7;
static const int DEFAULT_SIZE = 10;

template <typename Key, typename Value>
class HashTable {
 public:
    HashTable();
    explicit HashTable(int capacity);
    ~HashTable();

    bool insert(const Key& key, const Value& value);
    [[nodiscard]] bool contains(const Key& key) const;
    bool erase(const Key& key);
    [[nodiscard]] Value get(const Key& key) const;

    void rehash();

    [[nodiscard]] int size() const;
    [[nodiscard]] int capacity() const;

    template <typename K, typename V>
    friend std::ostream& operator<<(std::ostream& os,
                                    const HashTable<K, V>& table);

 private:
    using KeyValuePair = std::pair<Key, Value>;
    std::vector<KeyValuePair*> buckets_ = std::vector<KeyValuePair*>();

    int size_;
    int capacity_;

    [[nodiscard]] int hash(const Key& key) const;

    [[nodiscard]] int probe(const Key& key, int num) const;
};

template <typename Key, typename Value>
HashTable<Key, Value>::HashTable() : HashTable(DEFAULT_SIZE) {}

template <typename Key, typename Value>
HashTable<Key, Value>::HashTable(int capacity) : size_(0), capacity_(capacity) {
    buckets_.resize(capacity, nullptr);
}

template <typename Key, typename Value>
HashTable<Key, Value>::~HashTable() {
    for (auto bucket : buckets_) {
        delete bucket;
    }
}

template <typename Key, typename Value>
bool HashTable<Key, Value>::insert(const Key& key, const Value& value) {
    if (size_ >= capacity_ * LOAD_FACTOR) {
        rehash();
    }
    int index = 0;
    for (int count = 0; count < capacity_; ++count) {
        index = probe(key, count);
        if (buckets_[index] == nullptr) {
            buckets_[index] = new KeyValuePair(key, value);
            ++size_;
            return true;
        } else if (buckets_[index]->first == key) {
            buckets_[index]->second =
                value;  // Update value if key already exists
            return true;
        }
    }
    return false;
}

template <typename Key, typename Value>
bool HashTable<Key, Value>::contains(const Key& key) const {
    int index = 0;
    for (int count = 0; count < capacity_; ++count) {
        index = probe(key, count);
        if (buckets_[index] == nullptr) {
            return false;
        } else if (buckets_[index]->first == key) {
            return true;
        }
    }
    return false;
}

template <typename Key, typename Value>
bool HashTable<Key, Value>::erase(const Key& key) {
    int index = 0;
    for (int count = 0; count < capacity_; ++count) {
        index = probe(key, count);
        if (buckets_[index] == nullptr) {
            return false;
        } else if (buckets_[index]->first == key) {
            delete buckets_[index];
            buckets_[index] = nullptr;
            --size_;
            return true;
        }
    }
    return false;
}

template <typename Key, typename Value>
Value HashTable<Key, Value>::get(const Key& key) const {
    int index = 0;
    for (int count = 0; count < capacity_; ++count) {
        index = probe(key, count);
        if (buckets_[index] == nullptr) {
            return Value();
        } else if (buckets_[index]->first == key) {
            return buckets_[index]->second;
        }
    }
    return Value();
}

template <typename Key, typename Value>
int HashTable<Key, Value>::size() const {
    return size_;
}

template <typename Key, typename Value>
int HashTable<Key, Value>::capacity() const {
    return capacity_;
}

template <typename Key, typename Value>
std::ostream& operator<<(std::ostream& os, const HashTable<Key, Value>& table) {
    for (const auto& bucket : table.buckets_) {
        if (bucket != nullptr) {
            os << bucket->first << "=>" << bucket->second << ",";
        }
    }
    return os;
}

template <typename Key, typename Value>
int HashTable<Key, Value>::hash(const Key& key) const {
    return std::hash<Key>{}(key) % capacity_;
}

template <typename Key, typename Value>
int HashTable<Key, Value>::probe(const Key& key, int num) const {
    return (hash(key) + num * num) % capacity_;
}

template <typename Key, typename Value>
void HashTable<Key, Value>::rehash() {
    capacity_ *= 2;
    const std::vector<KeyValuePair*> old_buckets = std::move(buckets_);
    buckets_.resize(capacity_, nullptr);
    size_ = 0;

    for (auto bucket : old_buckets) {
        if (bucket != nullptr) {
            insert(bucket->first, bucket->second);
            delete bucket;
        }
    }
}

#endif
