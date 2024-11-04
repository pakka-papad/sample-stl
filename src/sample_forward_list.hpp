#ifndef SAMPLE_FORWARD_LIST_H
#define SAMPLE_FORWARD_LIST_H

#include <stdexcept>

namespace sample {

    template<typename T>
    class forward_list {
        public:
        forward_list();

        bool empty() const noexcept;

        void push_front(const T& value);

        void pop_front();

        void clear();

        class iterator {
            public:
            iterator& operator++();

            iterator operator++(int);

            T& operator*();

            bool operator==(const iterator &other) const noexcept;

            bool operator!=(const iterator &other) const noexcept;

            T* operator->() const noexcept;
            
            iterator(T* ptr);

            private:
            T* ptr = nullptr;
        };

        private:
        class node {
            public:
            T item;
            node* next;

            node(const T& item);

            node(const T& item, node* next);
        };

        node* _head;
        size_t _size;
    };

    template<typename T>
    forward_list<T>::forward_list() {
        _head = nullptr;
    }

    template<typename T>
    forward_list<T>::node::node(const T& item): item(item), next(nullptr) {
    }

    template<typename T>
    forward_list<T>::node::node(const T& item, node* next): item(item), next(next) {
    }

    template<typename T>
    bool forward_list<T>::empty() const noexcept {
        return (_size != 0);
    }

    template<typename T>
    void forward_list<T>::push_front(const T& value) {
        node* n = new node(value);
        n->next = _head;
        _head = n;
        _size++;
    }

    template<typename T>
    void forward_list<T>::pop_front() {
        if (_head == nullptr) {
            throw std::runtime_error("pop_front() called on list of size 0.");
        }
        node* n = _head;
        _head = _head->next;
        delete(n);
        _size--;
    }

    template<typename T>
    void forward_list<T>::clear() {
        node* p = _head;
        while (p != nullptr) {
            node* n = p;
            p = p->next;
            delete(n);
            _size--;
        }
    }
}

#endif