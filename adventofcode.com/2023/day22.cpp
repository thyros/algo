#include "pointL.h"
#include "utils.h"
#include "ut.hpp"
#include <deque>
#include <format>
#include <functional>
#include <unordered_set>

using T = long long;

struct QueueEntry {
    Point p;
    T stepsLeft;
};

void solvePart1(const Lines& lines, Point start, T steps) {
    const Point size {lines[0].size(), lines.size()};

    std::unordered_set<Point> seen;
    std::unordered_set<Point> answer;
    std::deque<QueueEntry> queue;

    queue.push_back(QueueEntry {start, steps});

    while (!queue.empty()) {
        const auto [p, s] = queue.front();
        queue.pop_front();

        if (s % 2 == 0) {
            answer.insert(p);
        } else if (s == 0) {
            continue;
        }

        for (const Point np: {Point{p.x-1, p.y}, Point{p.x, p.y-1}, Point{p.x+1, p.y}, Point{p.x, p.y+1}}) {
            if (p.x < 0 || p.y < 0 || p.x >= size.x || p.y >= size.y) {
                continue;
            }
            

            seen.insert(np);
        }
    }

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
}

void test() {
    using namespace boost::ut;

    ""_test = [] {
    };
}

int main(int argc, const char** argv) {
    test();
    const char* filename = argc > 1 ? argv[1] : "day22.sample";
    solve(filename);
}