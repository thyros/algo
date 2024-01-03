#include "utils.h"
#include "ut.hpp"
#include "point.h"
#include <algorithm>
#include <format>
#include <limits>
#include <unordered_map>

using T = long long;
using Matrix = std::vector<std::vector<int>>;

enum class Dir {
    U, D, R, L
};

struct Node {
    Point pos {0,0};
    Point parent {0,0};
    float fCost {0};
    float gCost {0};
    float hCost {0};
};
using Nodes = std::vector<Node>;
using Map = std::vector<std::vector<Node>>;

inline bool operator < (const Node& lhs, const Node& rhs) {
    return lhs.fCost < rhs.fCost;
}

auto& at(auto& container, Point p) {
    return container[p.y][p.x];
}

inline bool isOnBoard(int x, int y, int width, int height) {
    return x >= 0 && y >= 0 && x < width && y < height;
}

Dir toDir(int x, int y, int newX, int newY) {
    if (newX > x) return Dir::R;
    if (newX < x) return Dir::L;
    if (newY > y) return Dir::D;
    return Dir::U;
}

bool isValidMove(Point current, Point newP, const Map& map) {
    const auto x = current.x;
    const auto y = current.y;
    const Point parent = at(map, current).parent;
    if ((parent.x == -1 && parent.y == -1) || (parent.x == x && parent.y == y)) {
        return true;
    }

    const Dir newDir = toDir(current.x, current.y, newP.x, newP.y);
    const Dir oldDir = toDir(parent.x, parent.y, x, y);
    if (oldDir == Dir::R && newP.x < x) {
        return false;
    }
    if (oldDir == Dir::L && newP.x > x) {
        return false;
    }
    if (oldDir == Dir::U && newP.y > y) {
        return false;
    }
    if (oldDir == Dir::D && newP.y < y) {
        return false;
    }
    return true;
}

inline bool reachedEnd(int x, int y, Point to) {
    return x == to.x && y == to.y;
}

std::vector<Point> getNeighbours(Point p) {
    const auto x = p.x;
    const auto y = p.y;
    const std::vector<Point> result { Point{x-1, y}, Point{x, y-1}, Point{x+1,y}, Point{x, y+1}};
    return result;
}

Nodes makePath(const Map& map, Point to) {
    Nodes result;

    int x = to.x;
    int y = to.y;
    while (!(map[y][x].parent.x == x && map[y][x].parent.y == y) && map[y][x].pos.x != -1 && map[y][x].pos.y != -1) {
        result.push_back(map[y][x]);
        const Point p = map[y][x].parent;
        x = p.x;
        y = p.y;
    };
    result.push_back(map[y][x]);


    return Nodes {result.rbegin(), result.rend() };
} 

float calculateH(int x, int y, Point to) {
    return (to.x - x) + (to.y - y);
}

void printMap(const Map& map, Point p) {
    std::cout << std::format("{},{}\n", p.x, p.y);
    for (const auto& row: map) {
        for (const Node& node: row) {
            // std::cout << std::format("{:2},{:2} ", node.pos.x, node.pos.y);
            const auto cost = node.fCost;
            std::cout << std::format("{:2} ", cost > 10000 ? 0 : cost);
        }
        std::cout << "\n";
    }
}

Nodes astar(Point from, Point to, const Matrix& matrix) {
    const T height = matrix.size();
    const T width = matrix[0].size();

    const float limit = std::numeric_limits<float>::max();

    std::vector<std::vector<bool>> closedList(height, std::vector<bool>(width, false));
    std::vector<std::vector<Node>> allMap(height, Nodes(width, Node {.pos = {0, 0}, .parent = {-1, -1}, .fCost = limit, .gCost = limit, .hCost = limit }));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            at(allMap, Point{x, y}).pos = Point {x, y};
        }
    }


    Point p = from;

    at(allMap, p).fCost = 0;
    at(allMap, p).gCost = 0;
    at(allMap, p).hCost = 0;
    at(allMap, p).parent = p;
    
    std::vector<Node> openList;
    openList.emplace_back(at(allMap, p));
    bool destinationFound = false;

    while (!openList.empty() && openList.size() < width * height) {
        Node node;

        do {
            float temp = limit;
            std::vector<Node>::iterator itNode;
            for (std::vector<Node>::iterator it = begin(openList); it != end(openList); ++it) {
                Node n = *it;
                if (n.fCost < temp) {
                    temp = n.fCost;
                    itNode = it;
                }
            }

            node = *itNode;
            openList.erase(itNode);
        } while (isOnBoard(node.pos.x, node.pos.y, width, height) == false);

        printMap(allMap, p);

        p = node.pos;
        closedList[p.y][p.x] = true;
    
        for (Point newP: getNeighbours(p)) {
            const int newX = newP.x;
            const int newY = newP.y;
            const int pX = at(allMap, p).parent.x;
            const int pY = at(allMap, p).parent.y;

            if (!isOnBoard(newX, newY, width, height)) {
                continue;
            }

            if (!isValidMove(p, newP, allMap)) {
                continue;
            }

            else if (reachedEnd(newX, newY, to)) {
                at(allMap, newP).parent = p;
                destinationFound = true;
                return makePath(allMap, to);
            }

            else if (closedList[newY][newX] == false) {
                const float gNew = at(matrix, newP);
                const float hNew = calculateH(newX, newY, to);
                const float fNew = gNew + hNew;

                Node& nextNode = allMap[newY][newX];

                if (nextNode.fCost == limit ||
                    nextNode.fCost > fNew) {
                    nextNode.fCost = fNew;
                    nextNode.gCost = gNew;
                    nextNode.hCost = hNew;
                    nextNode.parent = p;
                    openList.emplace_back(nextNode);
                }
            }
        }
    }

    return {};
}

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

void solvePart1(const Matrix& matrix) {
    const int height = matrix.size();
    const int width = matrix[0].size();

    const Nodes nodes = astar(Point {0,0}, Point{width-1, height-1}, matrix);
    T result = 0;
    for (const Node& n: nodes) {
        const Point& p = n.pos;
        result += matrix[p.y][p.x];
    }

    std::cout << std::format("Part 1: total heat loss: {}\n", result);
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