#include "sample_unordered_map.hpp"
#include <iostream>

int main(){
    sample::unordered_map<std::string,int> m;
    for(int i = 1; i <= 500; i++){
        m.insert(std::to_string(i),i);
    }
    for(int i = 201; i <= 210; i++){
        std::cout << m[std::to_string(i)] << '\n';
    }
    for(int i = 11; i <= 500; i++){
        m.erase(std::to_string(i));
    }
    std::cout << m.size() << '\n';
    for (auto it = m.begin(); it != m.end(); ++it) {
        std::cout << "{" << it->first << "," << it->second << "}\n";
    }
    return EXIT_SUCCESS;
}