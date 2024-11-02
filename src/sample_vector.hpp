#ifndef SAMPLE_VECTOR_H
#define SAMPLE_VECTOR_H

#include <cstddef>
#include <stdexcept>
#include <algorithm>

namespace sample {

    template<typename T>
    class vector {
        public:
        vector();

        size_t size() const noexcept;

        size_t capacity() const noexcept;

        void shrink_to_fit();

        void reserve(const size_t &new_cap);

        void resize(const size_t &new_size);

        void push_back(const T &value);

        void pop_back();

        void clear();

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
        _data = nullptr;
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
    void vector<T>::shrink_to_fit() {
        if (_capacity <= _size || _data == nullptr) return;
        T* new_data = (_size == 0 ? nullptr : 
            static_cast<T*>(operator new[](_size * sizeof(T))));
        if (_size != 0 && new_data == nullptr) {
            throw std::bad_alloc();
        }
        for (int i = 0; i < _size; i++) {
            new_data[i] = _data[i];
        }
        delete[](_data);
        _data = new_data;
        _capacity = _size;
    }

    template<typename T>
    void vector<T>::reserve(const size_t &new_cap) {
        if (new_cap <= _capacity) return;
        T* new_data = static_cast<T*>(operator new[](new_cap * sizeof(T)));
        if (new_data == nullptr) {
            throw std::bad_alloc();
        }
        for (int i = 0; i < _size; i++) {
            new_data[i] = _data[i];
        }
        if (_data != nullptr) {
            delete[](_data);
        }
        _data = new_data;
        _capacity = new_cap;
    }

    template<typename T>
    void vector<T>::resize(const size_t &new_size) {
        if (new_size == _size) return;
        if (new_size > _size) {
            reserve(new_size);
            for (int i = _size; i < new_size; i++) {
                new (&_data[i])T(); // https://isocpp.org/wiki/faq/dtors#placement-new
            }
        } else {
            for (int i = new_size; i < _size; i++) {
                _data[i].~T();
            }
        }
        _size = new_size;
    }

    template<typename T>
    void vector<T>::push_back(const T &value) {
        if (_capacity == 0) {
            reserve(1);
        } else if (_size >= _capacity) {
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
        _data[_size].~T();
    }

    template<typename T>
    void vector<T>::clear() {
        resize(0);
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