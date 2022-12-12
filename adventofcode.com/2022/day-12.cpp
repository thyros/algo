#include "utils.h"
#include <cassert>
#include <cstdio>
#include <functional>
#include <unordered_set>
#include <vector>

using Grid = std::vector<std::vector<int>>;
struct Point {
    int x, y;
};

using Directions = std::vector<Point>;
Directions allDirections {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

template<>
struct std::hash<Point> {
    std::size_t operator()(const Point& p) const noexcept
    {
        constexpr int MAX_VALUE = 1000;
        return ((p.x+MAX_VALUE)<<16)|((p.y+MAX_VALUE)&0xFFFF);
    }
};

template<>
struct std::equal_to<Point> {
    bool operator()( const Point& lhs, const Point& rhs ) const {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }
};

struct PuzzleInput {
    Grid grid;
    Point start;
    Point goal;
};

PuzzleInput parseInput(const Lines& lines) {
    const size_t height = lines.size();
    const size_t width = lines[0].size();

    Grid grid(height, std::vector<int>(width, 0));
    Point start;
    Point goal;

    const auto toHeight = [](char c){ return c - 'a' + 1; };

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (lines[y][x] == 'S') {
                start = Point {x, y};
                grid[y][x] = toHeight('a');
            } else if (lines[y][x] == 'E') {
                goal = {x, y};
                grid[y][x] = toHeight('z');
            } else {
                grid[y][x] = toHeight(lines[y][x]);
            }
        }
    }

    return PuzzleInput{.grid = std::move(grid), .start = start, .goal = goal};
}

int depthFirstSearch(const Grid& grid, const Point& start, std::function<bool(const Point&, const Point&)> canMove, 
            std::function<bool(const Point&)> isPathComplete) {

    const int width = grid[0].size();
    const int height = grid.size();
    
    const auto onGrid = [width, height](Point p) { return p.x >= 0 && p.x < width && p.y >= 0 && p.y < height ;};

    std::vector<Point> pointsToSee;
    std::unordered_set<Point> pointsSeen;

    int length = 0;
    pointsToSee.push_back(start);

    while(!pointsToSee.empty()) {
        std::vector<Point> nextBatch;

        for (const Point& p: pointsToSee) {
 
            if (isPathComplete(p)) {
                return length;
            }

            for (const auto& d: allDirections) {
                const Point nextPoint {p.x + d.x, p.y + d.y};

                if (onGrid(nextPoint) && canMove(p, nextPoint)) {
                    if (!pointsSeen.contains(nextPoint)) {
                        pointsSeen.insert(nextPoint);
                        nextBatch.push_back(nextPoint);
                    }
                }
            }
        }

        ++length;
        pointsToSee = nextBatch;
    }

    return -1;
}


void printGrid(const Grid& grid) {
    for (const auto& row: grid) {
        for (const int h: row) {
            printf("%3i", h);
        }
        printf("\n");   
    }
}

void part1(const Grid& grid, const Point& start, const Point& goal) {
    const auto canClimb = [&grid](Point from, Point to) { return grid[to.y][to.x] - grid[from.y][from.x] <= 1; };
    const auto reachedGoal = [goal](const Point& p) { return p.x == goal.x && p.y == goal.y; };

    const int length = depthFirstSearch(grid, start, canClimb, reachedGoal);
    printf("Part 1: %i\n", length);
}

void part2(const Grid& grid, const Point& start, const Point& goal) {
    const auto canDescend = [&grid](Point from, Point to) { return grid[from.y][from.x] - grid[to.y][to.x] <= 1; };
    const auto reachedGround = [&grid](const Point& p) { return grid[p.y][p.x] == 1; };

    const int length = depthFirstSearch(grid, goal, canDescend, reachedGround);
    printf("Part 2: %i\n", length);
}

void solve(const char* filename) {
    const Lines lines = readFile(filename);
    auto [grid, start, goal] = parseInput(lines);

    part1(grid, start, goal);
    part2(grid, start, goal);
}

void test() {

}

int main() {
    test();

    constexpr char filename[] { "day-12.input" };
    solve(filename);
}