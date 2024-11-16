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

        size_t inline getPos(uint32_t hash) const;

        void reHash(size_t size);

        float inline getLoadFactor() const;

        constexpr static float MAX_LOAD_FACTOR = 0.75f;
        constexpr static float MIN_LOAD_FACTOR = 0.422f;
        constexpr static int MIN_SLOTS = 16;

        public:
        unordered_map(); 

        size_t size() const noexcept;

        void clear();

        void insert(Key key, Value value);

        Value& operator[](Key key) const;

        void erase(Key key);

        class iterator {
            private:
            friend class unordered_map<Key, Value, Hash, KeyEqual>;

            forward_list<entry>* list;

            typename forward_list<entry>::iterator it;

            size_t& slots;

            size_t cur_slot;

            std::pair<Key, Value> val;

            iterator(forward_list<entry>* list, typename forward_list<entry>::iterator it,
                size_t& slots, size_t cur_slot);

            public:
            iterator& operator++();

            iterator operator++(int);

            std::pair<Key,Value>& operator*();

            std::pair<Key,Value>* operator->();

            bool operator==(const iterator &other) const noexcept;

            bool operator!=(const iterator &other) const noexcept;
        };

        iterator begin();

        iterator end();
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
    size_t inline unordered_map<Key, Value, Hash, KeyEqual>::getPos(uint32_t hash) const {
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
    float inline unordered_map<Key, Value, Hash, KeyEqual>::getLoadFactor() const {
        return (float)_keys/(float)_slots;
    }

    template<typename Key, typename Value, typename Hash, typename KeyEqual>
    size_t unordered_map<Key, Value, Hash, KeyEqual>::size() const noexcept {
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
    Value& unordered_map<Key, Value, Hash, KeyEqual>::operator[](Key key) const {
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

    template<typename Key, typename Value, typename Hash, typename KeyEqual>
    unordered_map<Key, Value, Hash, KeyEqual>::iterator::iterator(forward_list<entry>* list, 
        typename forward_list<entry>::iterator it, size_t& slots, 
        size_t cur_slot): list(list), it(it), slots(slots), cur_slot(cur_slot) {
        if (list != nullptr && it != list->end()) {
            val.first = it->key;
            val.second = it->value;
        }
    }

    template<typename Key, typename Value, typename Hash, typename KeyEqual>
    typename unordered_map<Key, Value, Hash, KeyEqual>::iterator& unordered_map<Key, Value, Hash, KeyEqual>::iterator::operator++() {
        if (cur_slot >= slots) {
            throw std::runtime_error("Not a valid iterator");
        }
        ++it;
        if (list->end() == it) {
            do {
                ++list;
                ++cur_slot;
            } while (cur_slot < slots && list->size() == 0);
            if (cur_slot != slots) {
                it = list->begin();
            } else {
                list = nullptr;
            }
        }
        if (list != nullptr && it != list->end()) {
            val.first = it->key;
            val.second = it->value;
        }
        return *this;
    }

    template<typename Key, typename Value, typename Hash, typename KeyEqual>
    typename unordered_map<Key, Value, Hash, KeyEqual>::iterator unordered_map<Key, Value, Hash, KeyEqual>::iterator::operator++(int) {
        if (cur_slot >= slots) {
            throw std::runtime_error("Not a valid iterator");
        }
        ++it;
        if (list->end() == it) {
            do {
                ++list;
                ++cur_slot;
            } while (cur_slot < slots && list->size() == 0);
            if (cur_slot != slots) {
                it = list->begin();
            } else {
                list = nullptr;
            }
        }
        if (list != nullptr && it != list->end()) {
            val.first = it->key;
            val.second = it->value;
        }
        return unordered_map<Key, Value, Hash, KeyEqual>::iterator(list, it, slots, cur_slot);
    }

    template<typename Key, typename Value, typename Hash, typename KeyEqual>
    std::pair<Key,Value>& unordered_map<Key, Value, Hash, KeyEqual>::iterator::operator*() {
        return val;
    }

    template<typename Key, typename Value, typename Hash, typename KeyEqual>
    std::pair<Key,Value>* unordered_map<Key, Value, Hash, KeyEqual>::iterator::operator->() {
        return &val;
    }

    template<typename Key, typename Value, typename Hash, typename KeyEqual>
    bool unordered_map<Key, Value, Hash, KeyEqual>::iterator::operator==(const iterator &other) const noexcept {
        if (this->cur_slot >= slots && other.cur_slot >= slots) return true;
        if (this->cur_slot >= slots || other.cur_slot >= slots) return false;
        return (this->cur_slot == other.cur_slot && this->it == other.it);
    }

    template<typename Key, typename Value, typename Hash, typename KeyEqual>
    bool unordered_map<Key, Value, Hash, KeyEqual>::iterator::operator!=(const iterator &other) const noexcept {
        if (this->cur_slot >= slots && other.cur_slot >= slots) return false;
        if (this->cur_slot >= slots || other.cur_slot >= slots) return true;
        return (this->cur_slot != other.cur_slot || this->it != other.it);
    }

    template<typename Key, typename Value, typename Hash, typename KeyEqual>
    typename unordered_map<Key, Value, Hash, KeyEqual>::iterator unordered_map<Key, Value, Hash, KeyEqual>::begin() {
        size_t pos = -1;
        do {
            pos++;
        } while (pos < _slots && _data[pos].size() == 0);
        return unordered_map<Key, Value, Hash, KeyEqual>::iterator(&_data[pos], _data[pos].begin(), _slots, pos);
    }

    template<typename Key, typename Value, typename Hash, typename KeyEqual>
    typename unordered_map<Key, Value, Hash, KeyEqual>::iterator unordered_map<Key, Value, Hash, KeyEqual>::end() {
        return unordered_map<Key, Value, Hash, KeyEqual>::iterator(nullptr, _data[_slots-1].end(), _slots, _slots);
    }
}

#endif