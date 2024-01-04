#include "utils.h"
#include "ut.hpp"
#include "point.h"
#include <algorithm>
#include <format>
#include <queue>
#include <unordered_set>

using T = long long;
using Matrix = std::vector<std::vector<int>>;

auto& at(auto& container, Point p) {
    return container[p.y][p.x];
}

struct Node {
    int heatloss;
    int x, y;
    int dx, dy;
    int n;
};
struct NodeCmp {
    bool operator()(const Node& l, const Node& r) {
        return l.heatloss > r.heatloss;
    }
};

struct Seen {
    int x, y;
    int dx, dy;
    int n;
    auto operator<=>(const Seen&) const = default;
};
template <>
struct std::hash<Seen> {
    std::size_t operator()(const Seen& s) const noexcept {
        std::size_t seed = 5;
        for(auto x : {s.x, s.y, s.dx, s.dy, s.n}) {
            x = ((x >> 16) ^ x) * 0x45d9f3b;
            x = ((x >> 16) ^ x) * 0x45d9f3b;
            x = (x >> 16) ^ x;
            seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

Matrix parse(const Lines& lines) {
    const T height = lines.size();
    const T width = lines[0].size();

    Matrix matrix(height, std::vector<int>(width, 0));

    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            matrix[y][x] = lines[y][x] - '0';
        }
    }

    return matrix;
}

int calculateHeatLoss(Point from, Point to, const Matrix& matrix, int minStep, int maxStep) {
    const int width = matrix[0].size();
    const int height = matrix.size();

    const auto isOnBoard = [width, height](int x, int y) { return x >= 0 && y >= 0 && x < width && y < height; };

    std::priority_queue<Node, std::vector<Node>, NodeCmp> queue;
    std::unordered_set<Seen> seen;

    queue.push(Node {0, 0, 0, 0, 0, 0});

    while (!queue.empty()) {
        const Node n = queue.top();
        queue.pop();

        if (Point {n.x, n.y} == to && n.n >= minStep) {
            return n.heatloss;
        }

        const Seen s {n.x, n.y, n.dx, n.dy, n.n};
        if (seen.contains(s)) {
            continue;
        }
            
        seen.insert(s);
        if (n.n < maxStep) {
            int nx = n.x + n.dx;
            int ny = n.y + n.dy;

            if (isOnBoard(nx, ny)) {
                queue.push(Node {n.heatloss + at(matrix, Point{nx, ny}), nx, ny, n.dx, n.dy, n.n + 1});
            }
        }

        if (n.n >= minStep || (n.dx == 0 && n.dy == 0) ) {
            for (Point d: {Point {-1, 0}, Point {1, 0}, Point {0, -1}, Point {0, 1}}) {
                if (d != Point{n.dx, n.dy} && d != Point{-n.dx, -n.dy}) {
                    int nx = n.x + d.x;
                    int ny = n.y + d.y;

                    if (isOnBoard(nx, ny)) {
                        queue.push(Node {n.heatloss + at(matrix, Point{nx, ny}), nx, ny, d.x, d.y, 1});
                    }
                }
            }
        }
    }

    return -1;
}

void solvePart1(const Matrix& matrix) {
    const int height = matrix.size();
    const int width = matrix[0].size();

    std::cout << std::format("Part 1: total heat loss: {}\n", calculateHeatLoss(Point {0,0}, Point{width-1, height-1}, matrix, 1, 3));
    std::cout << std::format("Part 2: total heat loss: {}\n", calculateHeatLoss(Point {0,0}, Point{width-1, height-1}, matrix, 4, 10));
}

void solve(const char* filename) {
    const Lines& lines = readFile(filename);

    const Matrix m = parse(lines);
    solvePart1(m);
}

void test() {
    using namespace boost::ut;

    ""_test = [] {
    };
}

int main(int argc, const char** argv) {
    test();
    const char* filename = argc > 1 ? argv[1] : "day17.sample";
    solve(filename);
}