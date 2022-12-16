#include "utils.h"
#include "point.h"
#include "ut.hpp"
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <limits>
#include <optional>

struct Reading {
    Point sensor;
    Point beacon;
    int distance = 0;
};
using Readings = std::vector<Reading>;

struct Range {
    int from = 0;
    int to = 0;
    auto operator<=>(const Range&) const = default;
};

struct MinMax {
    int min;
    int max;
};
MinMax findMinMaxX(const Readings& readings) {
    int minX = std::numeric_limits<int>::max();
    int maxX = std::numeric_limits<int>::min();
    for (const Reading& r: readings) {
        const int readingMin = std::min(r.beacon.x, r.sensor.x) - r.distance;
        minX = std::min(minX, readingMin);

        const int readingMax = std::max(r.beacon.x, r.sensor.x) + r.distance;
        maxX = std::max(maxX, readingMax);
    }
    return {.min = minX, .max = maxX};
}


int manchatanDistance(int fx, int fy, int tx, int ty) {
    return std::abs(fx - tx) + std::abs(fy - ty);
}

int manchatanDistance(Point from, Point to) {
    return manchatanDistance(from.x, from.y, to.x, to.y);
}

Readings toReadings(const Lines& lines) {
    Readings result;

    for (const std::string& line: lines) {
        int sx, sy, bx, by;
        sscanf(line.c_str(), "Sensor at x=%i, y=%i: closest beacon is at x=%i, y=%i", &sx, &sy, &bx, &by);

        const int distance = manchatanDistance({.x = sx, .y = sy}, {.x = bx, .y = by});
        result.push_back(Reading{.sensor{.x = sx, .y = sy}, .beacon {.x = bx, .y = by}, .distance = distance});
    }

    return result;
}

void print(const Readings& readings, MinMax minMax) {
    for (const Reading& r: readings) {
        printf("%i,%i -> %i,%i distance: %i\n", r.sensor.x, r.sensor.y, r.beacon.x, r.beacon.y, r.distance);
    }
    printf("Min: %i Max %i\n", minMax.min, minMax.max);
}

bool covered(const Readings& readings, int x, int y) {
    for (const Reading& r: readings) {
        if (r.beacon.x == x && r.beacon.y == y) {
            continue;
        } else if (r.sensor.x == x && r.sensor.y == y) {
            continue;
        }

        if (manchatanDistance(r.sensor.x, r.sensor.y, x, y) <= r.distance) {
            return true;
        }
    }
    return false;
}

void part1(const Readings& readings, MinMax minMax, int y) {
    int count = 0;
    for (int x = minMax.min; x <= minMax.max; ++x) {
        if (covered(readings, x, y)) {
            ++count;
        }
    }

    printf("Part 1: %i\n", count);
}

std::optional<Range> toRange(Point sensor, int strength, int y) {
    const int horizontalSpread = strength - std::abs(y - sensor.y);
    if (horizontalSpread >= 0) {
        return std::make_optional(Range {.from = sensor.x - horizontalSpread, .to = sensor.x + horizontalSpread});
    }
    return std::nullopt;
}

void part2(const Readings& readings, int areaSize) {
    for (int y = 0; y <= areaSize; ++y) {
        std::vector<Range> ranges;

        for (const Reading& r: readings) {
            auto maybeRange = toRange(r.sensor, r.distance, y);
            if (maybeRange) {
                ranges.push_back(maybeRange.value());
            }

        }

        std::sort(ranges.begin(), ranges.end());

        // printf("---------------------\n");
        // for (const Range& r: ranges) {
        //     printf("%i, %i\n", r.from, r.to);
        // }

        int x = ranges[0].to;
        for (int i = 1; i < ranges.size(); ++i) {
            if (x >= 0 && x <= areaSize && ranges[i].from - x > 1) {
                const long long bx = x + 1;
                const long long by = y;
                const long long result = bx * 4000000 + by;
                printf("Part 2: %llu,%llu -> %llu\n", bx, by, result);
                break;
            } else {
                x = std::max(x, ranges[i].to);
            }
        }

    }
}

void solve(const char* filename, int y, int areaSize) {
    Lines input = readFile(filename);
    Readings readings = toReadings(input);

    MinMax minMax = findMinMaxX(readings);
    print(readings, minMax);

    part1(readings, minMax, y);
    part2(readings, areaSize);
}

void test() {
    using namespace boost::ut;

    "manhatanDistance"_test = [] {
        expect(5_i == manchatanDistance(2, 2, -3, 2));
        expect(5_i == manchatanDistance(2, 2, 2, -3));
        expect(5_i == manchatanDistance(-2, 2, 3, 2));
        expect(5_i == manchatanDistance(2, -2, 2, 3));
        expect(4_i == manchatanDistance(1, 1, 3, 3));
        expect(4_i == manchatanDistance(3, 3, 1, 1));
        expect(4_i == manchatanDistance(-2, -2, -4, -4));
        expect(4_i == manchatanDistance(-4, -4, -2, -2));
    };


    //   0 1 2 3 4 5 6
    // 0       X      
    // 1     X X X    
    // 2   X X X X X  
    // 3 X X X S X X X
    "toRange"_test = [] {
        expect(Range {.from=3, .to=3} == toRange({.x=3, .y=3}, 3, 0));
        expect(Range {.from=2, .to=4} == toRange({.x=3, .y=3}, 3, 1));
        expect(Range {.from=1, .to=5} == toRange({.x=3, .y=3}, 3, 2));
        expect(Range {.from=0, .to=6} == toRange({.x=3, .y=3}, 3, 3));
    };
}

int main() {
    toRange({.x=3, .y=3}, 1, 0);

    test();

    // solve("day-15.sample", 10, 20);
    solve("day-15.input", 2000000, 4000000);
}