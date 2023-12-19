#include "utils.h"
#include <algorithm>
#include <cstdio>
#include <functional>
#include <iostream>
#include <vector>

struct Pair {
    int firstLeft;
    int firstRight;
    int secondLeft;
    int secondRight;
};

Pair toPair(const std::string& line) {
    int firstLeft {0};
    int firstRight {0};
    int secondLeft {0};
    int secondRight {0};

    std::sscanf(line.c_str(), "%d-%d,%d-%d", &firstLeft, &firstRight, &secondLeft, &secondRight);

    return {firstLeft, firstRight, secondLeft, secondRight};
}


using Pairs = std::vector<Pair>;
Pairs toPairs(const Lines& lines) {
    Pairs result;
    result.reserve(lines.size());

    std::transform(begin(lines), end(lines), std::back_inserter(result), toPair);
    return result;
}

bool fullyOverlap(const Pair& pair) {
    return pair.firstLeft >= pair.secondLeft && pair.firstRight <= pair.secondRight ||
           pair.secondLeft >= pair.firstLeft && pair.secondRight <= pair.firstRight;
}

bool overlap(const Pair& pair) {
    const bool dontOverlap = pair.firstRight < pair.secondLeft ||
                             pair.firstLeft > pair.secondRight ||
                             pair.secondRight < pair.firstLeft ||
                             pair.secondLeft > pair.firstRight;
    return !dontOverlap;
}

int solve(const char* filename, std::function<bool(const Pair&)> f) {
    Lines lines = readFile(filename);
    Pairs pairs = toPairs(lines);

    int count = 0;
    for(const Pair& pair: pairs) {
        if (f(pair)) {
            ++count;
        }
    }

    return count;
}

int main() {
    std::cout << "Result: " << solve("day04.input", overlap) << std::endl;
}