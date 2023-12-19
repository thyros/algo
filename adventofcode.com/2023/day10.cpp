#include "point.h"
#include "utils.h"
#include "ut.hpp"
#include <array>
#include <format>

using Offsets = std::array<Point, 2>;


Offsets getOffsetsForStartingPoints(const Lines& lines, const size_t height, const size_t width, Point start) {
    Offsets offsets;
    int currentOffset = 0;
    if (start.y > 0) {
        const char c = lines[start.y - 1][start.x];
        if (c == '|' || c == 'L' || c == 'J') {
            offsets[currentOffset++] = Point {0, -1};
        }
    }
    if (start.x > 0) {
        const char c = lines[start.y][start.x - 1];
        if (c == '-' || c == 'L' || c == 'F') {
            offsets[currentOffset++] = Point {-1, 0};
        }
    }
    if (start.y < height - 1) {
        const char c = lines[start.y + 1][start.x];
        if (c == '|' || c == 'F' || c == 'J') {
            offsets[currentOffset++] = Point {0, 1};
        }
    }
    if (start.x < width - 1) {
        const char c = lines[start.y][start.x + 1];
        if (c == '-' || c == 'L' || c == 'F') {
            offsets[currentOffset++] = Point {1, 0};
        }
    }
    return offsets;
}

Offsets getOffsetForChar(const char c) {
    if (c == '|') return { Point {0, -1}, Point {0, 1} };
    if (c == '-') return { Point {-1, 0}, Point {1, 0} };
    if (c == 'L') return { Point {0, -1}, Point {1, 0} };
    if (c == 'J') return { Point {0, -1}, Point {1, 0} };
    if (c == 'F') return { Point {0,  1}, Point {0, 1} };
    if (c == '7') return { Point {0,  1}, Point {-1, 0} };

    return { Point {0,0}, Point {0,0} };
}

bool move(Point& pos, Point& offset, std::vector<std::vector<int>>& network) {
    return false;
}

int solve(const Lines& lines, Point start) {
    const size_t height = lines.size();
    const size_t width = lines.size();
    std::vector<std::vector<int>> network(height, std::vector<int>(width, 0));

    std::array<Point, 2> pos {start, start};

    while (true) {
        if (move(pos[0], offset[0], network)) {
            return network.at(pos[0].y + offset[0].y]).at([pos[0].x + offset[0].x]);
        }
    }


    return 0;
}

void solve(const char* filename) {
    const Lines& lines = readFile(filename);
    const size_t height = lines.size();
    const size_t width = lines.size();

    Point start;
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            if (lines[y][x] == 'S') {
                start.y = y;
                start.x = x;                
            }
        }
    }

    const int result = solve(lines, start);    
    std::cout << std::format("Longest point can be reached in {} steps\n", result);
}

void test() {
    using namespace boost::ut;

    ""_test = [] {
        
    };
}

int main(int argc, const char** argv) {
    test();
    const char* filename = argc > 1 ? argv[1] : "day10.sample";
    solve(filename);
}