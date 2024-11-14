#include "sample_unordered_map.hpp"
#include <iostream>

int main(){
    sample::unordered_map<int,int> m;
    m.insert(5,5);
    std::cout << m[5] << '\n';
    return EXIT_SUCCESS;
}