#include <algorithm>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <iterator>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

// https://adventofcode.com/2020/day/1

template<typename T>
std::vector<T> ReadToVector(std::istream& istr)
{
    std::istream_iterator<T> endOfStream;
    std::istream_iterator<T> iit(istr);

    std::vector<T> result;
    std::copy(iit, endOfStream, std::back_inserter(result));

    return result;
}

std::optional<int> BruteForce_part1(const std::vector<int>& xs) {
    const size_t len = xs.size();
    for (size_t i = 0; i < len - 1; ++i) {
        for (size_t j = i + 1;  j < len; ++j) {
            if (xs[i] + xs[j] == 2020) {
                return xs[i] * xs[j];
            }
        }
    }
    return {};
}

std::optional<int> WithSorting_part1(std::vector<int> xs) {
    std::sort(begin(xs), end(xs));

    for (auto it = begin(xs); it != end(xs); ++it) {
        auto x = find(it + 1, end(xs), 2020 - *it);
        if (x != end(xs)) {
            return *it * *x;
        }
    }
    return {};
}

std::optional<int> WithVector_part1(const std::vector<int>& xs) {
    std::vector<int> alreadySeenNumbers;
    alreadySeenNumbers.reserve(xs.size());

    for (auto it = begin(xs); it != end(xs); ++it) {
        int otherNumber = 2020 - *it;
        if (binary_search(begin(alreadySeenNumbers), end(alreadySeenNumbers), otherNumber)) {
            return *it * otherNumber;
        }

        auto where = std::lower_bound(begin(alreadySeenNumbers), end(alreadySeenNumbers), *it);
        alreadySeenNumbers.insert(where, *it);
    }

    return {};
}

std::optional<int> WithMap_part1(const std::vector<int>& xs) {
    std::unordered_map<int, size_t> alreadySeenNumbers;
    alreadySeenNumbers.reserve(xs.size());

    for (auto it = begin(xs); it != end(xs); ++it) {
        int otherNumber = 2020 - *it;
        auto x = alreadySeenNumbers.find(otherNumber);
        if (x != end(alreadySeenNumbers)) {
            return *it * otherNumber;
        }
        alreadySeenNumbers.insert(std::make_pair(*it, 0));
    }

    return {};
}

std::optional<int> BruteForce_part2(const std::vector<int>& xs) {
    const size_t len = xs.size();
    for (size_t i = 0; i < len - 2; ++i) {
        for (size_t j = i + 1;  j < len - 1; ++j) {
            for (size_t g = j + 1; g < len; ++g) {
                if (xs[i] + xs[j] + xs[g] == 2020) {
                    return xs[i] * xs[j] * xs[g];
                }
            }
        }
    }
    return {};
}

std::optional<int> WithSorting_part2(std::vector<int> xs) {
    std::sort(begin(xs), end(xs));

    const size_t len = xs.size();
    for (int i = 0; i < len - 2; ++i) {
        for (int j = i + 1; j < len - 1; ++j) {
            int thirdNumber = 2020 - xs[i] - xs[j];
            auto x = find(begin(xs) + j + 1, end(xs), thirdNumber);
            if (x != end(xs)) {
                return thirdNumber * xs[i] * xs[j];
            }
        }
    }
    return {};
}

template<typename T>
void Measure(T t, const std::vector<int>& xs, int N, const std::string& caption) {
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        t(xs);
    }
    auto t2 = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    std::cout << caption << ": " << duration << std::endl;
}

int main()
{
    std::ifstream ifstr("day-01.input");
    auto xs = ReadToVector<int>(ifstr);

    std::cout << "Part one" << std::endl;
    std::cout << "BruteForce: " << BruteForce_part1(xs).value_or(-1) << std::endl;
    std::cout << "WithSorting: " << WithSorting_part1(xs).value_or(-1) << std::endl;
    std::cout << "WithVector: " << WithVector_part1(xs).value_or(-1) << std::endl;
    std::cout << "WithMap: " << WithMap_part1(xs).value_or(-1) << std::endl;

    Measure(BruteForce_part1, xs, 100000, "BruteForce");
    Measure(WithSorting_part1, xs, 100000, "WithSorting");
    Measure(WithVector_part1, xs, 100000, "WithVector");
    Measure(WithMap_part1, xs, 100000, "WithMap");

    std::cout << "Part two" << std::endl;
    std::cout << "BruteForce: " << BruteForce_part2(xs).value_or(-1) << std::endl;
    std::cout << "WithSorting: " << WithSorting_part2(xs).value_or(-1) << std::endl;
    Measure(BruteForce_part2, xs, 100000, "BruteForce");
    Measure(WithSorting_part2, xs, 100000, "WithSorting");
}

// cl /Fo"build\\1.obj" /c /EHsc 1.cpp
// link /NOLOGO /OUT:build/1.exe build/1.obj
