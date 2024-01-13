#include "pointL.h"
#include "utils.h"
#include "ut.hpp"
#include <deque>
#include <cassert>
#include <format>
#include <functional>
#include <unordered_set>

using T = long long;

struct QueueEntry {
    Point p;
    T stepsLeft;
};

T fill(const Lines& lines, Point start, T steps) {
    const Point size {static_cast<T>(lines[0].size()), static_cast<T>(lines.size())};

    std::unordered_set<Point> seen;
    std::unordered_set<Point> answer;
    std::deque<QueueEntry> queue;

    queue.push_back(QueueEntry {start, steps});

    while (!queue.empty()) {
        const auto [p, s] = queue.front();
        queue.pop_front();

        // std::cout << std::format("{} {} {}\n", p.x, p.y, s);

        if (s % 2 == 0) {
            answer.insert(p);
        } else if (s <= 0) {
            continue;
        }

        for (const Point np: {Point{p.x-1, p.y}, Point{p.x, p.y-1}, Point{p.x+1, p.y}, Point{p.x, p.y+1}}) {
            if (np.x < 0 || np.y < 0 || np.x >= size.x || np.y >= size.y) {
                continue;
            } if (lines[np.y][np.x] == '#' || seen.contains(np)) {
                continue;
            }
        
            seen.insert(np);
            queue.push_back({np, s - 1});
        }
    }

    return answer.size();
}

void solvePart1(const Lines& lines, Point start, T steps) {

    const T total = fill(lines, start, steps);
    std::cout << std::format("Part 1: reached plots: {}\n", total);
}

void solvePart2(const Lines& lines, Point start, T steps) {
    assert(static_cast<T>(lines[0].size()) == static_cast<T>(lines.size()));
    const T size = lines.size();

    const T gridSize = steps /  size - 1;
    const T odds = std::pow((gridSize / 2 * 2 + 1), 2);
    const T evens = std::pow((gridSize + 1) / 2 * 2, 2);

    const T oddPoints = fill(lines, start, size * 2 + 1);
    const T evenPoints = fill(lines, start, size * 2);

    const T cornerT = fill(lines, Point { start.x, size - 1}, size - 1);
    const T cornerR = fill(lines, Point { 0, start.y}, size - 1);
    const T cornerB = fill(lines, Point { start.x, 0}, size - 1);
    const T cornerL = fill(lines, Point { size - 1, start.y}, size - 1);
    
    const T smallTR = fill(lines, Point {0, size - 1}, size / 2 - 1);
    const T smallTL = fill(lines, Point {size - 1, size - 1}, size / 2 - 1);
    const T smallBR = fill(lines, Point {0, 0}, size / 2 - 1);
    const T smallBL = fill(lines, Point {size - 1, 0 }, size / 2 - 1);
    const T smallCount = gridSize + 1;


    const T bigTR = fill(lines, Point {0, size - 1}, size * 3 / 2 - 1);
    const T bigTL = fill(lines, Point {size - 1, size - 1}, size * 3 / 2 - 1);
    const T bigBR = fill(lines, Point {0, 0}, size * 3 / 2 - 1);
    const T bigBL = fill(lines, Point {size - 1, 0 }, size * 3 / 2 - 1);
    const T bigCount = gridSize;

    const T total = odds * oddPoints + 
                    evens * evenPoints + 
                    cornerT + cornerR + cornerB + cornerL +
                    smallCount * (smallTR + smallTL + smallBR + smallBL) +
                    bigCount * (bigTR + bigTL + bigBR + bigBL);

    std::cout << std::format("Part 2: reached plots: {}\n", total);
}

void solve(const char* filename) {
    const Lines& lines = readFile(filename);

    const Point start = [&lines]{
        for (T y = 0; y < lines.size(); ++y) {
            for (T x = 0; x < lines[0].size(); ++x) {
                if (lines[y][x] == 'S') {
                    return Point {x, y};
                }
            }
        }
        return Point {-1, -1};
    }();

    // solvePart1(lines, start, 64);
    solvePart2(lines, start, 26501365);
}

void test() {
    using namespace boost::ut;

    ""_test = [] {
    };
}

int main(int argc, const char** argv) {
    test();
    const char* filename = argc > 1 ? argv[1] : "day21.sample";
    solve(filename);
}