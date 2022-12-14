#include "utils.h"
#include "point.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <charconv>
#include <cstdio>
#include <iostream>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Path = std::vector<Point>;
using Paths = std::vector<Path>;
using Grid = std::unordered_set<Point>;

Point toPoint(std::string_view sv) {
    const auto tokens = tokenize(sv, ",");
    const auto left = tokens[0];
    const auto right = tokens[1];

    Point result;
    result.x = std::stoi(toString(left));
    result.y = std::stoi(toString(right));

    return result;
}

Path toPath(const std::string& line) {
    const auto tokens = tokenize(line, " -> ");

    Path result;
    result.reserve(tokens.size());

    std::transform(begin(tokens), end(tokens), std::back_inserter(result), toPoint);
    return result;
}

Paths toPaths(const Lines& lines) {
    Paths result;

    for (const std::string& line: lines) {
        result.push_back(toPath(line));
    }

    return result;
}

Point simulate(const Grid& grid, Point current, int maxY) {
    constexpr std::array<Point, 3> dropOffsets { Point{.x = 0, .y = 1}, Point {.x = -1, .y = 1}, Point {.x = 1, .y = 1}};

    while(current.y <= maxY) {
        bool moved = false;
        for (const Point& offset: dropOffsets) {
            const Point next {.x = current.x + offset.x, .y = current.y + offset.y};
            if (!grid.contains(next)) {
                moved = true;
                current = next;
                break;
            }
        }

        if (!moved) {
            break;
        }
    };
    return current;
}

Grid toGrid(const Paths& paths) {
    Grid result;
    
    for(const auto& path: paths) {
        for (int i = 1; i < path.size(); ++i) {
            const Point& from = path[i - 1];
            const Point& to = path[i];

            for (int x = std::min(from.x, to.x); x <= std::max(from.x, to.x); ++x) {
                result.insert(Point{.x = x, .y = from.y});
            }
            for (int y = std::min(from.y, to.y); y <= std::max(from.y, to.y); ++y) {
                result.insert(Point{.x = from.x, .y = y});
            }
        }
    }

    return result;
}

int findMaxY(const Paths& paths) {
    int maxY = 0;
    for(const auto& path: paths) {
        for (int i = 1; i < path.size(); ++i) {
            const Point& from = path[i - 1];
            const Point& to = path[i];

            maxY = std::max(maxY, std::max(from.y, to.y));
        }
    }
    return maxY;
}

void part1(Grid grid, Point start, int maxY) {
    int count = 0;
    bool abyss = false;
    while (!abyss) {
        const Point restPoint = simulate(grid, start, maxY);
        grid.insert(restPoint);

        abyss = restPoint.y > maxY;
        if (!abyss) {
            ++count;
        }
    }

    printf("Part 1: %i\n", count);
}

void part2(Grid grid, Point start, int maxY) {
    int count = 0;
    bool startBlocked = false;
    while (!startBlocked) {
        const Point restPoint = simulate(grid, start, maxY);
        grid.insert(restPoint);

        startBlocked = start.x == restPoint.x && start.y == restPoint.y;
        ++count;
    }

    printf("Part 2: %i\n", count);
}

void print(const Paths& paths) {
    for (const Path& path: paths) {
        for (const Point& p: path) {
            printf("(%i,%i)->", p.x, p.y);
        }
        printf("\n");
    }
}

void solve(const char* filename) {
    const Lines lines = readFile(filename);
    const Paths paths = toPaths(lines);

    const Grid grid = toGrid(paths);
    const int maxY = findMaxY(paths);
    const Point start {500, 0};

    // print(paths);
    part1(grid, start, maxY);
    part2(grid, start, maxY);
}

void test() {

}

int main() {
    test();

    constexpr char filename[] { "day-14.input" };
    solve(filename);
}