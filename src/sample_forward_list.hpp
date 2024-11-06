#ifndef SAMPLE_FORWARD_LIST_H
#define SAMPLE_FORWARD_LIST_H

#include <stdexcept>

namespace sample {

    template<typename T>
    class forward_list { 
        private:
        class node {
            public:
            T item;
            node* next;

            node(const T& item);

            node(const T& item, node* next);

            template<typename... Args>
            node(Args&&... args);
        };

        node* _head;
        size_t _size;

        public:
        forward_list();

        size_t size() const noexcept;

        bool empty() const noexcept;

        void push_front(const T& value);

        template<typename... Args>
        void emplace_front(Args&&... args);

        void pop_front();

        void clear();

        class iterator {
            public:
            iterator& operator++();

            iterator operator++(int);

            T& operator*();

            bool operator==(const iterator &other) const noexcept;

            bool operator!=(const iterator &other) const noexcept;

            T* operator->();

            private:
            friend class forward_list<T>;
            iterator (node* ptr);

            node* _ptr = nullptr;
        };

        iterator begin();

        iterator end();
    };

    template<typename T>
    forward_list<T>::forward_list() {
        _head = nullptr;
        _size = 0;
    }

    template<typename T>
    forward_list<T>::node::node(const T& item): item(item), next(nullptr) {
    }

    template<typename T>
    forward_list<T>::node::node(const T& item, node* next): item(item), next(next) {
    }

    template<typename T>
    template<typename... Args>
    forward_list<T>::node::node(Args&&... args): item(std::forward<Args>(args)...) {

    }

    template<typename T>
    size_t forward_list<T>::size() const noexcept {
        return _size;
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
    template<typename... Args>
    void forward_list<T>::emplace_front(Args&&... args) {
        node* n = new node(std::forward<Args>(args)...);
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
        _head = nullptr;
    }

    template<typename T>
    forward_list<T>::iterator::iterator (node* ptr) : _ptr(ptr) {
        
    }

    template<typename T>
    typename forward_list<T>::iterator& forward_list<T>::iterator::operator++ () {
        this->_ptr = this->_ptr->next;
        return *this;
    }

    template<typename T>
    typename forward_list<T>::iterator forward_list<T>::iterator::operator++ (int) {
        this->_ptr = this->_ptr->next;
        return forward_list<T>::iterator(this->_ptr);
    }

    template<typename T>
    T* forward_list<T>::iterator::operator-> () {
        return &(this->_ptr->item);
    }

    template<typename T>
    T& forward_list<T>::iterator::operator* () {
        return this->_ptr->item;
    }

    template<typename T>
    bool forward_list<T>::iterator::operator!= (const iterator &other) const noexcept {
        if (this->_ptr == nullptr && other._ptr == nullptr) return false;
        if (this->_ptr == nullptr || other._ptr == nullptr) return true;
        return (&(this->_ptr->item) != &(other._ptr->item));
    }

    template<typename T>
    bool forward_list<T>::iterator::operator== (const iterator &other) const noexcept {
        if (this->_ptr == nullptr && other._ptr == nullptr) return true;
        if (this->_ptr == nullptr || other._ptr == nullptr) return false;
        return (&(this->_ptr->item) == &(other._ptr->item));
    }

    template<typename T>
    typename forward_list<T>::iterator forward_list<T>::begin() {
        return forward_list<T>::iterator(_head);
    }

    template<typename T>
    typename forward_list<T>::iterator forward_list<T>::end() {
        return forward_list<T>::iterator(nullptr);
    }
}

#endif