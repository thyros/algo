#include "utils.h"
#include "ut.hpp"
#include <algorithm>
#include <format>
#include <functional>
#include <numeric>

using T = long long;
using Numbers = std::vector<T>;
using Algo = std::function<T(const Numbers& )>;

T part1Algorithm(Numbers numbers) {
    size_t t = numbers.size();
    bool allZeros = false;
    while (!allZeros && t>1) {
        t -= 1;
        allZeros = true;

        for (size_t i = 0; i < t; ++i) {
            numbers[i] = numbers[i+1] - numbers[i]; 
            allZeros &= numbers[i] == 0;
        }
    }

    return std::accumulate(begin(numbers) + t, end(numbers), 0ll, std::plus<T>());
}

T part2Algorithm(Numbers numbers) {
    size_t t = 0;
    bool allZeros = false;
    while (!allZeros && t < numbers.size()) {
        t += 1;
        allZeros = true;
        for (size_t i = numbers.size() - 1; i >=t; --i) {
            numbers[i] = numbers[i] - numbers[i-1];
            allZeros &= numbers[i] == 0;
        }
    }

    T result = 0;
    for (int i = t - 1; i >= 0; --i) {
        result = numbers[i] - result;
    }

    return result;
}

T solve(const std::vector<Numbers>& allNumbers, const Algo& algo) {
    T result = 0;
    for (const Numbers& numbers: allNumbers) {
        result += algo(numbers);
    }

    return result;    
}

void solve(const char* filename) {
    const Lines& lines = readFile(filename);

    std::vector<Numbers> numbers;
    std::transform(begin(lines), end(lines), back_inserter(numbers), [](const std::string& str) { return tokensToNumbers<T>(tokenize(str)); });

    const T part1 = solve(numbers, part1Algorithm);
    std::cout << std::format("Part 1: {}\n", part1);

    const T part2 = solve(numbers, part2Algorithm);
    std::cout << std::format("Part 2: {}\n", part2);
}

void test() {
    using namespace boost::ut;

    "Testing sample data on part1 algorithm"_test = [] {
        expect(18_i == part1Algorithm({0, 3, 6, 9, 12, 15}));
        expect(68_i == part1Algorithm({10, 13, 16, 21, 30, 45}));
    };

    "Testing sample data on part2 algorithm"_test = [] {
        expect(-3_i == part2Algorithm({0, 3, 6, 9, 12, 15}));
        expect(0_i == part2Algorithm({1, 3, 6, 10, 15, 21}));
        expect(5_i == part2Algorithm({10, 13, 16, 21, 30, 45}));
    };
}

int main(int argc, const char** argv) {
    test();
    const char* filename = argc > 1 ? argv[1] : "day09.sample";
    solve(filename);
}