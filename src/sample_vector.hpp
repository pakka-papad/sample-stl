#ifndef SAMPLE_VECTOR_H
#define SAMPLE_VECTOR_H

#include <cstddef>
#include <stdexcept>

namespace sample {

    template<typename T>
    class vector {
        public:
        vector();

        size_t size();

        void resize(const size_t &count);

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
    size_t vector<T>::size() {
        return _size;
    }

    template<typename T>
    void vector<T>::resize(const size_t &count) {
        if (count == _capacity) return;
        T* new_data = new T[count];
        size_t n = (_size < count ? _size : count);
        for (int i = 0; i < n; i++) {
            new_data[i] = _data[i];
        }
        _data = new_data;
        _capacity = count;
        _size = n;
    }

    template<typename T>
    void vector<T>::push_back(const T &value) {
        if (_capacity == 0) {
            resize(1);
            _data[0] = value;
            _size++;
            return;
        }
        if (_size >= _capacity) {
            resize((_capacity << 1));
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
            throw std::runtime_error("Trying to access elements with negative index");
        }
        if (pos >= _size) {
            throw std::runtime_error("Out of bounds access");
        }
        return _data[pos];
    }
}

#endif