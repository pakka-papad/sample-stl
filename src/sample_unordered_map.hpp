#ifndef SAMPLE_UNORDERED_MAP_H
#define SAMPLE_UNORDERED_MAP_H

#include <functional>
#include "sample_forward_list.hpp"
#include <iostream>

namespace sample {
    template<
        typename Key, 
        typename Value, 
        typename Hash = std::hash<Key>,
        typename KeyEqual = std::equal_to<Key>
    >
    class unordered_map {
        private:
        Hash _hash;

        KeyEqual _key_equal;

        class entry {
            public:
            Key key;
            Value value;
            uint32_t hash;
            entry(Key key, Value value, Hash hasher): key(key), value(value), hash(hasher(key)) {
            }
        };

        size_t _keys;

        size_t _slots; 

        forward_list<entry>* _data;

        size_t inline getPos(uint32_t hash);

        void reHash(size_t size);

        float inline getLoadFactor();

        constexpr static float MAX_LOAD_FACTOR = 0.75f;
        constexpr static float MIN_LOAD_FACTOR = 0.422f;
        constexpr static int MIN_SLOTS = 16;

        public:
        unordered_map(); 

        size_t size();

        void clear();

        void insert(Key key, Value value);

        Value& operator[](Key key);

        void erase(Key key);
    };


    template<typename Key, typename Value, typename Hash, typename KeyEqual>
    unordered_map<Key, Value, Hash, KeyEqual>::unordered_map() {
        _hash = Hash();
        _key_equal = KeyEqual();
        _keys = 0;
        _slots = 0;
        _data = nullptr;
    }

    template<typename Key, typename Value, typename Hash, typename KeyEqual>
    size_t inline unordered_map<Key, Value, Hash, KeyEqual>::getPos(uint32_t hash) {
        return (hash%_slots);
    }

    template<typename Key, typename Value, typename Hash, typename KeyEqual>
    void unordered_map<Key, Value, Hash, KeyEqual>::reHash(size_t size) {
        if (size <= 0) return;
        forward_list<entry>* new_data = static_cast<forward_list<entry>*>(operator new[](size * sizeof(forward_list<entry>)));
        if (new_data == nullptr) {
            throw std::bad_alloc();
        }
        for (size_t i = 0; i < size; i++) {
            new (&new_data[i])forward_list<entry>();
        }
        size_t oldSize = _slots;
        _slots = size;
        for (size_t i = 0; i < oldSize; ++i) {
            typename forward_list<entry>::iterator it = _data[i].begin();
            typename forward_list<entry>::iterator end = _data[i].end();
            while (it != end) {
                size_t pos = getPos(it->hash);
                new_data[pos].push_front(*it);
                ++it;
            }
        }
        if (_data != nullptr) {
            delete[](_data);
        }
        _data = new_data;
    }

    template<typename Key, typename Value, typename Hash, typename KeyEqual>
    float inline unordered_map<Key, Value, Hash, KeyEqual>::getLoadFactor() {
        return (float)_keys/(float)_slots;
    }

    template<typename Key, typename Value, typename Hash, typename KeyEqual>
    size_t unordered_map<Key, Value, Hash, KeyEqual>::size() {
        return _keys;
    }

    template<typename Key, typename Value, typename Hash, typename KeyEqual>
    void unordered_map<Key, Value, Hash, KeyEqual>::clear() {
        if (_data == nullptr) return;
        for (size_t i = 0; i < _slots; ++i) {
            _data[i].clear();
        }
        _keys = 0;
        reHash(MIN_SLOTS);
    }

    template<typename Key, typename Value, typename Hash, typename KeyEqual>
    void unordered_map<Key, Value, Hash, KeyEqual>::insert(Key key, Value value) {
        if (_data == nullptr) {
            reHash(MIN_SLOTS);
        }
        entry e(key, value, _hash);
        size_t pos = getPos(e.hash);
        std::cout << "get key: " << key << ", hash: " << e.hash << ", pos: " << pos << ", chain_length: " << _data[pos].size() << ", slots: " << _slots << "\n";
        typename forward_list<entry>::iterator it = _data[pos].begin();
        typename forward_list<entry>::iterator end = _data[pos].end();
        while (it != end) {
            if (_key_equal(it->key, e.key)) {
                it->value = e.value;
                return;
            }
            ++it;
        }
        _data[pos].push_front(e);
        _keys++;
        if (getLoadFactor() >= MAX_LOAD_FACTOR) {
            reHash(_slots << 1);
        }
    }

    template<typename Key, typename Value, typename Hash, typename KeyEqual>
    Value& unordered_map<Key, Value, Hash, KeyEqual>::operator[](Key key) {
        uint32_t hash = _hash(key);
        size_t pos = getPos(hash);
        std::cout << "get key: " << key << ", hash: " << hash << ", pos: " << pos << ", chain_length: " << _data[pos].size() << ", slots: " << _slots << "\n";
        typename forward_list<entry>::iterator it = _data[pos].begin();
        typename forward_list<entry>::iterator end = _data[pos].end();
        while (it != end) {
            if (_key_equal(key, it->key)) {
                return it->value;
            }
            ++it;
        }
        throw std::runtime_error("Key not found.");
    }

    template<typename Key, typename Value, typename Hash, typename KeyEqual>
    void unordered_map<Key, Value, Hash, KeyEqual>::erase(Key key) {
        uint32_t hash = _hash(key);
        size_t pos = getPos(hash);
        std::cout << "erase key: " << key << ", hash: " << hash << ", pos: " << pos << ", chain_length: " << _data[pos].size() << ", slots: " << _slots << "\n";
        if (_data[pos].size() == 0) return;
        typename forward_list<entry>::iterator it = _data[pos].begin();
        typename forward_list<entry>::iterator end = _data[pos].end();
        if (_key_equal(key, it->key)) {
            _data[pos].pop_front();
            _keys--;
            if (getLoadFactor() < MIN_LOAD_FACTOR) {
                reHash(_slots >> 1);
            }
            return;
        }
        typename forward_list<entry>::iterator prev = it;
        ++it;
        while (it != end) {
            if (_key_equal(key, it->key)) {
                _data[pos].erase_after(prev);
                _keys--;
                if (getLoadFactor() < MIN_LOAD_FACTOR) {
                    reHash(_slots >> 1);
                }
                return;
            }
            prev = it;
            ++it;
        }
        std::cout << "key not found: " << key << "\n";
    }
}

#endif