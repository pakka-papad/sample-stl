#ifndef SAMPLE_FORWARD_LIST_H
#define SAMPLE_FORWARD_LIST_H

namespace sample {
    
    template<typename T>
    class forward_list {
        public:
        forward_list();

        private:
        class node {
            public:
            T item;
            node* next;

            node(const T& item);

            node(const T& item, node* next);
        };

        node* _head;
    };

    template<typename T>
    forward_list<T>::forward_list() {
        _head = nullptr;
    }

    template<typename T>
    forward_list<T>::node::node(const T& item) {
        this->item = item;
        this->next = nullptr;
    }

    template<typename T>
    forward_list<T>::node::node(const T& item, node* next) {
        this->item = item;
        this->next = next;
    }
}

#endif