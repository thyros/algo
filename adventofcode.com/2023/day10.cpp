#include "point.h"
#include "utils.h"
#include "ut.hpp"
#include <array>
#include <format>
#include <optional>
#include <unordered_set>

using Offsets = std::array<Point, 2>;

struct MoveResult {
    Point nextPosition;
    Point nextOffset;
};

// finds the replacement character for the 'S' tile based on surrounding tiles
char getStartingPointReplacement(const Lines& lines, const size_t height, const size_t width, Point start) {
    const auto getTile = [&lines, height, width](int x, int y) -> std::optional<char> {
        if (x < 0 || x >= width || y < 0 || y >= height) {
            return {};
        }
        return std::make_optional(lines[y][x]);
    };

    std::unordered_set<char> result {'-', '|', 'F', 'J', 'L', '7'};
    const auto up = getTile(start.x, start.y - 1);
    if (!up || (*up != '|' && *up != 'F' && *up != '7')) {
        result.erase('|');
        result.erase('J');
        result.erase('L');
    }
    const auto left = getTile(start.x - 1, start.y);
    if (!left || (*left != '-' && *left != 'L' && *left != 'F')) {
        result.erase('-');
        result.erase('J');
        result.erase('7');
    }
    const auto down = getTile(start.x, start.y + 1);
    if (!down || (*down != '|' && *down != 'L' && *down != 'J')) {
        result.erase('|');
        result.erase('7');
        result.erase('F');
    }
    const auto right = getTile(start.x + 1, start.y);
    if (!right || (*right!= '-' && *right!= '7' && *right!= 'J')) {
        result.erase('-');
        result.erase('L');
        result.erase('F');
    }
    return *result.begin();
}

// returns two possible directions of the given pipe
// it assumes each pipe has two ends
Offsets getOffsetsForChar(const char c) {
    if (c == '|') return { Point {0, -1}, Point {0, 1} };
    if (c == '-') return { Point {-1, 0}, Point {1, 0} };
    if (c == 'L') return { Point {0, -1}, Point {1, 0} };
    if (c == 'J') return { Point {0, -1}, Point {-1, 0} };
    if (c == 'F') return { Point {0,  1}, Point {1, 0} };
    if (c == '7') return { Point {0,  1}, Point {-1, 0} };

    return { Point {0,0}, Point {0,0} };
}

// an utility to extract an element from two dimentional array
auto& elementAt(auto& c, const Point& p) {
    return c[p.y][p.x];
}

MoveResult move(const Lines& lines, const Point& pos, const Point& offset) {
    const Point nextPoint { pos.x + offset.x, pos.y + offset.y };
    const char c = elementAt(lines, nextPoint);

    const Offsets nextOffsets = getOffsetsForChar(c);
    const int chosenOffset = (offset.x == -nextOffsets[0].x && offset.y == -nextOffsets[0].y) ? 1 : 0;

    return MoveResult {.nextPosition = nextPoint, .nextOffset = nextOffsets[chosenOffset] };
}

int solvePart1(const Lines& lines, Point start) {
    const size_t height = lines.size();
    const size_t width = lines[0].size();
    std::vector<std::vector<int>> network(height, std::vector<int>(width, 0));

    std::array<Point, 2> pos {start, start};
    const char startingPointReplacement = getStartingPointReplacement(lines, height, width, start);
    Offsets offset = getOffsetsForChar(startingPointReplacement);

    while (true) {
        for (int i = 0; i <= 1; ++i) {
            const MoveResult result = move(lines, pos[i], offset[i]);
            const int visitedValue = elementAt(network, result.nextPosition);

            if (visitedValue != 0) {
                return visitedValue;
            }

            elementAt(network, result.nextPosition) = elementAt(network, pos[i]) + 1;

            pos[i] = result.nextPosition;
            offset[i] = result.nextOffset;
        }
    }

    return 0;
}

int solvePart2(const Lines& lines, Point start) {
    const size_t height = lines.size();
    const size_t width = lines[0].size();
    const char startingPointReplacement = getStartingPointReplacement(lines, height, width, start);

    std::vector<std::string> cleanedUp(height, std::string(width, '.'));
    elementAt(cleanedUp, start) = startingPointReplacement;

    Point offset = getOffsetsForChar(startingPointReplacement)[0];
    Point pos = start;
    while (true) {
        const MoveResult result = move(lines, pos, offset);
        if (elementAt(cleanedUp, result.nextPosition) != '.') {
            break;
        }

        elementAt(cleanedUp, result.nextPosition) = elementAt(lines, result.nextPosition);
        pos = result.nextPosition;
        offset = result.nextOffset;
    }

    int innerTilesCount = 0;
    for (const std::string& row: cleanedUp) {
        bool isInside = false;
        bool isOnEdge = false;
        bool edgePointsDown = false;
        for (const char c: row) {
            if (c == '.' && isInside) {
                ++innerTilesCount;
            } else if (c == '|') {
                isInside = !isInside;
            } else if (c == 'F') {
                isOnEdge = true;
                edgePointsDown = true;
            } else if (c == 'L') {
                isOnEdge = true;
                edgePointsDown = false;
            } else if (c == '7') {
                isOnEdge = false;
                if (!edgePointsDown) {
                    isInside = !isInside;
                }
            } else if (c == 'J') {
                isOnEdge = false;
                if (edgePointsDown) {
                    isInside = !isInside;
                }
            }
        }
    }

    return innerTilesCount;
}

void solve(const char* filename) {
    const Lines& lines = readFile(filename);
    const size_t height = lines.size();
    const size_t width = lines[0].size();

    Point start;
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            if (lines[y][x] == 'S') {
                start.y = y;
                start.x = x;                
            }
        }
    }

    const int stepsCount = solvePart1(lines, start); 
    std::cout << std::format("Longest point can be reached in {} steps\n", stepsCount);

    const int tilesCount = solvePart2(lines, start);
    std::cout << std::format("There are {} inner tiles\n", tilesCount);
}


void test() {
    using namespace boost::ut;

    ""_test = [] {
        
    };
}

int main(int argc, const char** argv) {
    test();
    const char* filename = argc > 1 ? argv[1] : "day10.sample4";
    solve(filename);
}