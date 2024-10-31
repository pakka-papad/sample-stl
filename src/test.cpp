#include "sample_vector.hpp"

#include <iostream>

int main() {
    sample::vector<int> v;
    v.push_back(3);
    v.push_back(7);
    std::cout << v[1] << '\n';
    v[1] = 4;
    std::cout << v[1] << '\n';
    return EXIT_SUCCESS;
}