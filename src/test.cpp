#include "sample_vector.hpp"

#include <iostream>
#include <string>
#include <vector>

int t = 1;

class A {
    public:
    int x;
    A() {
        x = t;
        t++;
        std::cout << "A constructor called " << x << '\n';
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
    std::vector<A> stdv;
    sample::vector<A> samv;
    for(int i = 0; i < 5; i++){
        std::cout << "\tPushing " << t << "\n";
        stdv.push_back(A());
        std::cout << "\tPushed " << t-1 << "\n";
    }
    for(int i = 0; i < 5; i++){
        std::cout << "\tPushing " << t << "\n";
        samv.push_back(A());
        std::cout << "\tPushed " << t-1 << "\n";
    }
    for(int i = 0; i < 5; i++){
        stdv[i].print();
    }
    for(int i = 0; i < 5; i++){
        samv[i].print();
    }
    samv[0] = A();
    A x = samv[0];
    return EXIT_SUCCESS;
}