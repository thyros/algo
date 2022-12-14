#include "utils.h"
#include <cassert>
#include <charconv>
#include <cstdio>
#include <iostream>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Point {
    int x;
    int y;
};

using Path = std::vector<Point>;
using Paths = std::vector<Path>;


Point toPoint(std::string_view sv) {
    Point result;

    const auto coma = sv.find(',');
    std::from_chars(sv.begin(), sv.begin() + coma, result.x);
    std::from_chars(sv.begin() + coma + 1, sv.end(), result.y);

    return result;
}

Path toPath(const std::string& line) {
    Path result;

    std::string_view sv = line;

    size_t lhs = 0;

    while(lhs != std::string::npos) {
        size_t rhs = line.find(" -> ");

        Point from = toPoint(sv.substr(lhs, rhs - lhs));
        



    }



    return result;
}

Paths toPaths(const Lines& lines) {
    Paths result;

    for (const std::string& line: lines) {
        result.push_back(toPath(line));
    }

    return result;
}

void solve(const char* filename) {
    const Lines lines = readFile(filename);
    const Paths paths = toPaths(lines);
}

void test() {

}

int main() {
    test();

    constexpr char filename[] { "day-14.sample" };
    solve(filename);
}