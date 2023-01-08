#pragma once
#include <array>
#include <algorithm>

template <typename T, int C>
struct ArrayHash {
    std::size_t operator()(const std::array<T, C>& arr) const {
        // std::size_t result = 0;
        // for (int i: arr) {
        //     result ^= i + 0x9e3779b9 + (result<<6) + (result>>2);
        // }
        // return result;

        std::size_t seed = arr.size();
        for(auto x : arr) {
            x = ((x >> 16) ^ x) * 0x45d9f3b;
            x = ((x >> 16) ^ x) * 0x45d9f3b;
            x = (x >> 16) ^ x;
            seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

template <typename T, int C>
struct ArrayEqual {
    bool operator()(const std::array<T, C>& lhs, const std::array<T, C>& rhs) const
    {
        if (lhs.size() != rhs.size()) {
            return false;
        }

        return std::mismatch(begin(lhs), end(lhs), begin(rhs)).first == end(lhs);
    }
};

// #include <tuple>
// #include <iostream>
// #include <functional>
// #include <type_traits>

// template<typename Tuple, std::size_t... ids>
// std::size_t tupleHash(Tuple const& tuple, std::index_sequence<ids...> const&);

// template<typename T>
// std::size_t hashValue(T const& value)
// {
//     // SFINAE kicks in here for tuples.
//     // There is no std::hash that works for tuples.
//     // So this candidate will be ignored if you use a tuple.
//     std::hash<T>    hasher;
//     return hasher(value);
// }

// template<typename... Args>
// std::size_t hashValue(std::tuple<Args...> const& value)
// {
//     return tupleHash(value, std::make_index_sequence<sizeof...(Args)>());
// }
// // Special case the empty tuple (as Args... can not be the empty list).
// std::size_t hashValue(std::tuple<> const& value)
// {
//     // Not an expert in hashing.
//     // Do some appropriate thing to get a value;
//     return 1;
// }

// template<typename Tuple, std::size_t... ids>
// std::size_t tupleHash(Tuple const& tuple, std::index_sequence<ids...> const&)
// {
//     // Not an expert in hashing.
//     // Not sure 0 is a good seed (or this algorithm is good) solely here for demo purpose.
//     std::size_t  result = 0;
//     for(auto const& hash: {hashValue(std::get<ids>(tuple))...})
//     {
//         result ^= hash + 0x9e3779b9 + (result<<6) + (result>>2);
//     }
//     return result;
// };