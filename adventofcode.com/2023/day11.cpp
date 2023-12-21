#include "utils.h"
#include "ut.hpp"
#include "point.h"
#include <format>
#include <limits>

using T = long long;
using Galaxies = std::vector<Point>;

void parse(const Lines& lines, std::vector<int>& emptyRows, std::vector<int>& emptyColumns, Galaxies& galaxies) {
    std::string mask(lines[0].size(), '.');
    for (size_t y = 0; y < lines.size(); ++y) {
        bool hasGalaxy = false;
        for (size_t x = 0; x < lines[y].size(); ++x) {
            if (lines[y][x] == '#') {
                hasGalaxy = true;
                mask[x] = '#';
                galaxies.emplace_back(static_cast<int>(x), static_cast<int>(y));
            }
        }

        if (!hasGalaxy) {
            emptyRows.emplace_back(static_cast<int>(y));
        }
    }

    for (size_t x = 0; x < mask.size(); ++x) {
        if (mask[x] == '.') {
            emptyColumns.emplace_back(static_cast<int>(x));
        }
    }
}

Galaxies expand(std::vector<int> emptyRows, std::vector<int> emptyColumns, Galaxies galaxies, int scaleFactor) {
    for (Point& galaxy: galaxies) {
        const int above = std::count_if(begin(emptyRows), end(emptyRows), [y = galaxy.y](int emptyRow) { return emptyRow < y; });
        const int left = std::count_if(begin(emptyColumns), end(emptyColumns), [x = galaxy.x](int emptyCol) { return emptyCol < x; });

        galaxy.x += left * scaleFactor;
        galaxy.y += above * scaleFactor;
    }
    return galaxies;
}

T sumShortestPaths(const Galaxies& galaxies) {
    T sum = 0;

    for (size_t x1 = 0; x1 < galaxies.size() - 1; ++x1) {
        for (size_t x2 = x1 + 1; x2 < galaxies.size(); ++x2) {
            const Point& g1 = galaxies[x1];
            const Point& g2 = galaxies[x2];
            const T distance = std::abs(g2.y - g1.y) + std::abs(g2.x - g1.x);

            sum += distance;
        }
    }

    return sum;
}

void solve(const char* filename, int scaleFactor) {
    const Lines& lines = readFile(filename);

    std::vector<int> emptyRows;
    std::vector<int> emptyColumns;
    Galaxies galaxies;

    parse(lines, emptyRows, emptyColumns, galaxies);
    galaxies = expand(emptyRows, emptyColumns, galaxies, scaleFactor);

    const T sum = sumShortestPaths(galaxies);
    std::cout << std::format("Sum of shortest paths: {}\n", sum);
}

void test() {
    using namespace boost::ut;

    ""_test = [] {
        const Galaxies galaxies = {{0, 1}, {2, 1}, {0, 3}, {2, 5}};
        const std::vector<int> emptyRows = {0, 2, 4};
        const std::vector<int> emptyCols = {1};

        const Galaxies expanded = expand(emptyRows, emptyCols, galaxies, 1);
        expect(0_i == expanded[0].x);
        expect(2_i == expanded[0].y);
        expect(3_i == expanded[1].x);
        expect(2_i == expanded[1].y);
        expect(0_i == expanded[2].x);
        expect(5_i == expanded[2].y);
        expect(3_i == expanded[3].x);
        expect(8_i == expanded[3].y);
    };
}

int main(int argc, const char** argv) {
    test();
    const char* filename = argc > 1 ? argv[1] : "day11.sample";
    const int scaleFactor = argc > 2 ? std::atoi(argv[2]) : 1;
    solve(filename, scaleFactor);
}