#ifndef SAMPLE_UNORDERED_MAP_H
#define SAMPLE_UNORDERED_MAP_H

#include <functional>

namespace sample {
    template<
        typename key, 
        typename value, 
        typename hash = std::hash<key>,
        typename key_equal = std::equal_to<key>
    >
    class unordered_map {
        public:
        unordered_map();

        private:
        hash _hash;
        key_equal _key_equal; 
    };


    template<typename key, typename value, typename hash, typename key_equal>
    unordered_map<key, value, hash, key_equal>::unordered_map() {
        _hash = hash();
        _key_equal = key_equal();
    }
}

#endif