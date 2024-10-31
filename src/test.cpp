#include "sample_vector.hpp"

#include <iostream>
#include <string>

int main() {
    sample::vector<std::string> v;
    v.push_back("Hello");
    v.push_back("World");
    std::cout << v[1] << '\n';
    v[1] = "Sumit";
    std::cout << v[1] << '\n';
    v.resize(1);
    std::cout << v[0] << '\n';
    v.resize(10);


    sample::vector<int> vi;
    vi.push_back(7);
    vi.push_back(5);
    std::cout << vi[0] << ' ' << vi[1] << '\n';
    vi.resize(10);
    std::cout << vi[0] << ' ' << vi[1] << ' ' << vi[2] << '\n';
    vi.resize(1);
    return EXIT_SUCCESS;
}