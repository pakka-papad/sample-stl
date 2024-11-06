#include "sample_forward_list.hpp"
#include <iostream>

int t = 1;

class A {
    public:
    int x;
    A() {
        x = t;
        t++;
        std::cout << "A constructor called " << x << '\n';
    }
    ~A() {
        std::cout << "A destructor called " << x << '\n';
    }
    A(const A& other) {
        this->x = other.x;
        std::cout << "A copy constructor called " << x << '\n';
    }
    A& operator=(const A& other) {
        if (this != &other) {
            this->x = other.x;
            std::cout << "A assignment operator called " << x << '\n';
        }
        return *this;
    }
    void print(){
        std::cout << "A print called " << x << '\n';
    }
};

int main() {
    sample::forward_list<A> l;
    for (int i = 0; i < 5; i++) {
        std::cout << "\tPushing " << t << "\n";
        l.push_front(A());
        std::cout << "\tPushed " << t-1 << "\n";
    }
    for (auto it = l.begin(); it != l.end(); ++it) {
        std::cout << it->x << ' ';
    }
    std::cout << '\n';
    for (auto &a: l) {
        std::cout << a.x << ' ';
    }
    std::cout << '\n';
    l.pop_front();
    l.clear();
    for (int i = 0; i < 3; i++) {
        std::cout << "\tPushing " << t << "\n";
        l.emplace_front();
        std::cout << "\tPushed " << t-1 << "\n";
    }
    return EXIT_SUCCESS;
}

