#include "utils.h"
#include "ut.hpp"
#include "point.h"
#include <algorithm>
#include <format>
#include <limits>
#include <unordered_map>

using T = long long;
using Matrix = std::vector<std::vector<int>>;

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

inline bool isValid(int x, int y, int width, int height) {
    return x >= 0 && y >= 0 && x < width && y < height;
}

inline bool reachedEnd(int x, int y, Point to) {
    return x == to.x && y == to.y;
}

std::vector<Point> getNeighbours(int x, int y) {
    const std::vector<Point> result { Point{x-1, y}, Point{x, y-1}, Point{x+1,y}, Point{x, y+1}};
    return result;
}

Nodes makePath(const Map& map, Point to) {
    Nodes result;

    int x = to.x;
    int y = to.y;
    while (!(map[y][x].parent.x == x && map[y][x].parent.y == y) && map[y][x].pos.x != -1 && map[y][x].pos.y != -1) {
        result.push_back(map[y][x]);
        const Point p = map[y][x].pos;
        x = p.x;
        y = p.y;
    };
    result.push_back(map[y][x]);


    return Nodes {result.rbegin(), result.rend() };
} 

float calculateH(int x, int y, Point to) {
    return (to.x - x) + (to.y - y);
}

auto& at(auto& container, Point p) {
    return container[p.y][p.x];
}

Nodes astar(Point from, Point to, const Matrix& matrix) {
    const T height = matrix.size();
    const T width = matrix[0].size();

    const float limit = std::numeric_limits<float>::max();

    std::vector<std::vector<bool>> closedList(height, std::vector<bool>(width, false));
    std::vector<std::vector<Node>> allMap(height, Nodes(width, Node {.pos = {0, 0}, .parent = {-1, -1}, .fCost = limit, .gCost = limit, .hCost = limit }));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            allMap[y][x].pos = Point {x, y};
        }
    }


    int x = from.x;
    int y = from.y;

    allMap[y][x].fCost = 0;
    allMap[y][x].gCost = 0;
    allMap[y][x].hCost = 0;
    allMap[y][x].parent = Point {x, y};
    
    std::vector<Node> openList;
    openList.emplace_back(allMap[x][y]);
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
        } while (isValid(node.pos.x, node.pos.y, width, height) == false);

        x = node.pos.x;
        y = node.pos.y;
        closedList[y][x] = true;

        for (Point offset: getNeighbours(x, y)) {
            int newX = x + offset.x;
            int newY = y + offset.y;

            if (!isValid(newX, newY, width, height)) {
                continue;
            }

            else if (reachedEnd(newX, newY, to)) {
                allMap[y][x].parent = Point {x, y};
                destinationFound = true;
                return makePath(allMap, to);
            }

            else if (closedList[newY][newX] == false) {
                const float gNew = matrix[newY][newX];
                const float hNew = calculateH(newX, newY, to);
                const float fNew = gNew + hNew;

                Node& nextNode = allMap[newY][newX];

                if (nextNode.fCost == limit ||
                    nextNode.fCost > fNew) {
                    nextNode.fCost = fNew;
                    nextNode.gCost = gNew;
                    nextNode.hCost = hNew;
                    nextNode.parent = Point {x, y};
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