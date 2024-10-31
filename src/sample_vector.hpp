#ifndef SAMPLE_VECTOR_H
#define SAMPLE_VECTOR_H

#include <cstddef>
#include <stdexcept>

namespace sample {

    template<typename T>
    class vector {
        public:
        vector();

        size_t size() const noexcept;

        size_t capacity() const noexcept;

        void reserve(const size_t &new_cap);

        void push_back(const T &value);

        void pop_back();

        T& operator[](const size_t &pos);

        private:
        T* _data;
        size_t _size;
        size_t _capacity;
    };

    template<typename T>
    vector<T>::vector() {
        _size = 0;
        _capacity = 0;
    }

    template<typename T>
    size_t vector<T>::size() const noexcept {
        return _size;
    }

    template<typename T>
    size_t vector<T>::capacity() const noexcept {
        return _capacity;
    }

    template<typename T>
    void vector<T>::reserve(const size_t &new_cap) {
        if (new_cap <= _capacity) return;
        T* new_data = new T[new_cap];
        for (int i = 0; i < _size; i++) {
            new_data[i] = _data[i];
        }
        _data = new_data;
        _capacity = new_cap;
    }

    template<typename T>
    void vector<T>::push_back(const T &value) {
        if (_capacity == 0) {
            reserve(1);
            _data[0] = value;
            _size++;
            return;
        }
        if (_size >= _capacity) {
            reserve((_capacity << 1));
        }
        _data[_size] = value;
        _size++;
    }

    template<typename T>
    void vector<T>::pop_back() {
        if (_size == 0) {
            throw std::runtime_error("pop_back() called on vector of size 0.\n");
        }
        _size--;
    }

    template<typename T>
    T& vector<T>::operator[](const size_t &pos) {
        if (pos < 0) {
            throw std::runtime_error("Accessing element at negative index");
        }
        if (pos >= _size) {
            throw std::runtime_error("Out of bounds access");
        }
        return _data[pos];
    }
}

#endif