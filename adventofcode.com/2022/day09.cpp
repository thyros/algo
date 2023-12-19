#include "utils.h"
#include <cassert>
#include <cstdio>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

/**
 * Head moves one square at the time
 * Tail needs to be pulled towards head once they are 2 squares apart
 * Tail prefers diagonal moves
*/

struct Point {
    int x = 0;
    int y = 0;
};

template<>
struct std::hash<Point> {
    std::size_t operator()(const Point& p) const noexcept
    {
        constexpr int MAX_VALUE = 1000;
        return ((p.x+MAX_VALUE)<<16)|((p.y+MAX_VALUE)&0xFFFF);
    }
};

bool operator==(const Point& lhs, const Point& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator<(const Point& lhs, const Point& rhs) {
    return (lhs.x < rhs.x) || ((lhs.x == rhs.x) && (lhs.y < rhs.y));
}

Point moveTail(const Point& head, const Point& tail) {
    // if distance between head and tail is not longer than 2
    // return current position

    // move tail towards head in the X axis
    // move tail towards head in the Y axis
    // that should be a new tail position

    int dx = head.x - tail.x;
    int dy = head.y - tail.y;

    if (std::abs(dx) <= 1 && std::abs(dy) <= 1) {
        return tail;
    }

//    printf("%i %i -> ", dx, dy);

    // normalize delta
    if (dx != 0) dx /= abs(dx);
    if (dy != 0) dy /= abs(dy);

//    printf("%i %i\n", dx, dy);
    return {tail.x + dx, tail.y + dy};
}


void solve(const char* filename) {
    const Lines lines = readFile(filename);
    const std::unordered_map<char, Point> directions {
        {'L', {-1, 0}}, {'R', {1, 0}}, {'U', {0, 1}}, {'D', {0, -1}}
    };

    // for each line do
    // make a move in the given direction and length
    // first do it square by square, later do it in long steps (what about visited squares)

    std::unordered_set<Point> visitedLocations;
    constexpr int ropeLength = 10;
    std::vector<Point> rope {ropeLength, Point {}};

    for (const std::string& line: lines) {
        char d;
        int length {};

        const int read = sscanf(line.c_str(), "%c %i", &d, &length);

        printf("Read %i %c %i\n", read, d, length);

        const Point direction = directions.at(d);
        for (int i = 0; i < length; ++i) {
            Point& head = rope[0];

            printf("Moving %i,%i towards %i,%i", head.x, head.y, direction.x, direction.y);

            head.x += direction.x;
            head.y += direction.y;

            for (size_t i = 1; i < rope.size(); ++i) {
                rope[i] = moveTail(rope[i-1], rope[i]);
            }

            visitedLocations.insert(rope.back());

            printf(" new positions h: %i,%i t: %i,%i\n", head.x, head.y, rope.back().x, rope.back().y);
        }
    }

    for (const Point& p: visitedLocations) {
        printf("\t%i,%i\n", p.x, p.y);
    }

    std::cout << "Part 1: " << visitedLocations.size() << std::endl;
}

void test() {
    // the same square
    assert((Point{1, 1} == moveTail(Point{1, 1}, Point{1, 1})));

    // no need to move
    assert((Point{1, 1} == moveTail(Point{1, 2}, Point{1, 1})));
    assert((Point{1, 1} == moveTail(Point{2, 1}, Point{1, 1})));
    assert((Point{1, 1} == moveTail(Point{1, 0}, Point{1, 1})));
    assert((Point{1, 1} == moveTail(Point{0, 1}, Point{1, 1})));

    // no need to move diagonal case
    assert((Point{1, 1} == moveTail(Point{0, 0}, Point{1, 1})));
    assert((Point{1, 1} == moveTail(Point{2, 0}, Point{1, 1})));
    assert((Point{1, 1} == moveTail(Point{0, 2}, Point{1, 1})));
    assert((Point{1, 1} == moveTail(Point{2, 2}, Point{1, 1})));

    // move in the same direction
    assert((Point{1, 0} == moveTail(Point{2, 0}, Point{0, 0})));
    assert((Point{1, 0} == moveTail(Point{0, 0}, Point{2, 0})));
    assert((Point{0, 1} == moveTail(Point{0, 2}, Point{0, 0})));
    assert((Point{0, 1} == moveTail(Point{0, 0}, Point{0, 2})));

    // move diagonally
    assert((Point{1, 1} == moveTail(Point{2, 1}, Point{0, 0})));
    assert((Point{1, 0} == moveTail(Point{0, 0}, Point{2, 1})));
    assert((Point{1, 1} == moveTail(Point{1, 2}, Point{0, 0})));
    assert((Point{0, 1} == moveTail(Point{0, 0}, Point{1, 2})));
}

int main() {
    test();

    constexpr char filename[] { "day09.input" };
    solve(filename);
}