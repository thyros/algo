#include "utils.h"
#include "ut.hpp"
#include "point.h"
#include <algorithm>
#include <format>
#include <limits>
#include <unordered_map>

// NOTE:
// this one does not work
// I made a mistake where I don't consider direction as a part of the node
// and therefore make some nodes unavailable (adding them to the closed list too early)
// I'm leaving it as an example to refactor later

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

inline Dir toDir(Point from, Point to) {
    return toDir(from.x, from.y, to.x, to.y);
}

bool isValidMove(Point current, Point newP, const Map& map) {
    const auto x = current.x;
    const auto y = current.y;
    const Point parent = at(map, current).parent;
    // the first move
    if (parent.x == x && parent.y == y) {
        return true;
    }

    // cannot turn back
    const Dir newDir = toDir(current.x, current.y, newP.x, newP.y);
    const Dir oldDir = toDir(parent.x, parent.y, x, y);
    if ((newDir == Dir::L && oldDir == Dir::R) ||
        (newDir == Dir::R && oldDir == Dir::L) ||
        (newDir == Dir::U && oldDir == Dir::D) ||
        (newDir == Dir::D && oldDir == Dir::U)) {
        return false;
    }


    // checking if path goes 3 tiles in the same direction 
    Point temp = current;
    for (int i = 0; i < 3; ++i) {
        Point tempParent = at(map, temp).parent;
        if (tempParent == temp) {
            return true;
        }

        Dir subDir = toDir(tempParent, temp);
        if (subDir != newDir) {
            return true;
        }

        temp = tempParent;
    }

    return false;
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

void printPath(const Map& map, const Nodes& path) {
    for (int y = 0; y < map.size(); ++y) {
        for (int x = 0; x < map[y].size(); ++x) {
            const Point p {x, y};
            const Node& n = at(map, p);

            if (std::any_of(begin(path), end(path), [p](const Node& n) { return p == n.pos; })) {
                if (p != n.parent) {
                    const Dir d = toDir(p, n.parent);   
                    if (d == Dir::U) std::cout << "^";
                    else if (d == Dir::D) std::cout << "v";
                    else if (d == Dir::L) std::cout << "<";
                    else if (d == Dir::R) std::cout << ">";
                    break;
                }
            };

            std::cout << " ";
        }
        std::cout << "\n";
    }

}

void printMap(const Map& map, Point p) {
    const Node& n = at(map, p);
    const auto l = [](float f) { return f > 10000 ? 0 : f; };

    std::cout << std::format("{},{} f: {} g: {} h: {}\n", p.x, p.y, n.fCost, n.gCost, n.hCost);
    for (const auto& row: map) {
        for (const Node& node: row) {
            // std::cout << std::format("{:3},{:3} ", node.pos.x, node.pos.y);

            const float cost = node.fCost;
            std::cout << std::format("{:4}", cost > 100000 ? 0 : cost);

            const Point pos = node.pos;
            const Point par = node.parent;
            const Dir d = toDir(pos, par);
            if (pos == par) {
                std::cout << "O";
            }
            else if (par == Point {-1, -1}) {
                std::cout << ".";
            }
            else if (d == Dir::U) std::cout << "^";
            else if (d == Dir::D) std::cout << "v";
            else if (d == Dir::L) std::cout << "<";
            else if (d == Dir::R) std::cout << ">";
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

        p = node.pos;
        closedList[p.y][p.x] = true;
        printMap(allMap, p);
    
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
                printMap(allMap, p);
                return makePath(allMap, to);
            }

            else if (closedList[newY][newX] == false) {
                const float gNew = at(allMap, p).gCost + at(matrix, newP);
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

    const Nodes path = astar(Point {0,0}, Point{width-1, height-1}, matrix);
    T result = 0;
    for (const Node& n: path) {
        const Point& p = n.pos;
        std::cout << std::format(" {},{}\n", p.x, p.y);
        result += matrix[p.y][p.x];
    }

    std::cout << std::format("Part 1: total heat loss: {} path length: {}\n", result, path.size());
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