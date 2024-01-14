#include "pointL.h"
#include "utils.h"
#include "ut.hpp"
#include <deque>
#include <format>
#include <functional>
#include <stack>
#include <unordered_map>
#include <unordered_set>

using T = long long;

struct StackEntry {
    Point p;
    T len;
};
using Graph = std::unordered_map<Point, std::unordered_map<Point, T>>;
using Points = std::vector<Point>;

const std::unordered_map<char, std::vector<Point>> dirs {
    {'.', std::vector<Point> {Point{-1, 0}, Point{0, -1}, Point{1, 0}, Point{0, 1}}},
    {'<', std::vector<Point> {Point{-1, 0}}},
    {'^', std::vector<Point> {Point{0, -1}}},
    {'>', std::vector<Point> {Point{1, 0}}},
    {'v', std::vector<Point> {Point{0, 1}}},
};

auto& at(auto& c, auto x, auto y) {
    return c[y][x];
}

auto& at(auto& c, Point p) {
    return at(c, p.x, p.y);
}

T dsf(const Graph& g, std::unordered_set<Point>& seen, Point p, Point end) {
    if (p == end) {
        return 0;
    }

    T m = 0;
    seen.insert(p);
    for (auto [n, l]: g.at(p)) {
        if (!seen.contains(n)) {
            m = std::max(m, dsf(g, seen, n, end) + l);
        }
    }
    seen.erase(p);
    return m;
}

void solvePart1(const Lines& lines, const Points& points, const Point start, const Point end) {
    const T width = lines[0].size();
    const T height = lines.size();

    const auto inPoints = [&points](Point p){ return std::find(std::begin(points), std::end(points), p) != std::end(points); };

    Graph graph;
    for (const Point& p: points) {
        graph[p];        
    }

    for (Point sp: points) {
        std::stack<StackEntry> stack;
        stack.push(StackEntry {sp, 0});

        std::unordered_set<Point> seen {sp};

        while (!stack.empty()) {
            const StackEntry c = stack.top(); // current
            stack.pop();

            if (c.len != 0 && inPoints(c.p)) {
                graph[sp][c.p] = c.len;
                continue;
            }

            for (Point offset: dirs.at(at(lines, c.p))) {
                const Point n = {c.p.x + offset.x, c.p.y + offset.y};
                if (n.x > 0 && n.y > 0 && n.x < width && n.y < height && at(lines, n) != '#' && !seen.contains(n)) {
                    stack.push(StackEntry{ n, c.len + 1});
                    seen.insert(n);
                }
            }
        }
    }

    std::unordered_set<Point> seen;
    const T longestPath = dsf(graph, seen, start, end);
    std::cout << std::format("Part 1: longest path {}\n", longestPath);
}

void solvePart2(const Lines& lines, const Points& points, const Point start, const Point end) {
    const T width = lines[0].size();
    const T height = lines.size();

    const auto inPoints = [&points](Point p){ return std::find(std::begin(points), std::end(points), p) != std::end(points); };

    Graph graph;
    for (const Point& p: points) {
        graph[p];        
    }

    for (Point sp: points) {
        std::stack<StackEntry> stack;
        stack.push(StackEntry {sp, 0});

        std::unordered_set<Point> seen {sp};

        while (!stack.empty()) {
            const StackEntry c = stack.top(); // current
            stack.pop();

            if (c.len != 0 && inPoints(c.p)) {
                graph[sp][c.p] = c.len;
                continue;
            }

            for (Point offset: {Point{-1, 0}, Point{0, -1}, Point{1, 0}, Point{0, 1}}) {
                const Point n = {c.p.x + offset.x, c.p.y + offset.y};
                if (n.x > 0 && n.y > 0 && n.x < width && n.y < height && at(lines, n) != '#' && !seen.contains(n)) {
                    stack.push(StackEntry{ n, c.len + 1});
                    seen.insert(n);
                }
            }
        }
    }

    std::unordered_set<Point> seen;
    const T longestPath = dsf(graph, seen, start, end);
    std::cout << std::format("Part 2: longest path {}\n", longestPath);    
}

void solve(const char* filename) {
    const Lines& lines = readFile(filename);
    const T width = lines[0].size();
    const T height = lines.size();

    const Point start {static_cast<long long>(lines[0].find('.')), 0};
    const Point end {static_cast<long long>(lines[height - 1].find('.')), height - 1};

    std::vector<Point> points {start, end};
    for (T y = 0; y < height; ++y) {
        for (T x = 0; x < width; ++x) {
            if (at(lines, x, y) == '#') {
                continue;
            }

            T neighbors = 0;
            for (const Point n: {Point{x-1, y}, Point{x, y-1}, Point{x+1, y}, Point{x, y+1}}) {
                if (n.x > 0 && n.y > 0 && n.x < width && n.y < height && at(lines, n) != '#') {
                    ++neighbors;
                }
            }
            if (neighbors >= 3) {
                points.push_back(Point{x, y});
            }
        }
    }


    solvePart1(lines, points, start, end);
    solvePart2(lines, points, start, end);
}

void test() {
    using namespace boost::ut;

    ""_test = [] {
    };
}

int main(int argc, const char** argv) {
    test();
    const char* filename = argc > 1 ? argv[1] : "day23.sample";
    solve(filename);
}