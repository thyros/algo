#include "utils.h"
#include "hash.h"
#include <array>
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <unordered_map>
#include <unordered_set>

using Point3d = std::array<int, 3>;
using Points3d = std::vector<Point3d>;
using Grid = std::unordered_set<Point3d, ArrayHash<3>>;

constexpr const int INF = 1000;
constexpr const std::array<Point3d, 6> neighbours{-1, 0, 0, 1, 0, 0, 0, -1, 0, 0, 1, 0, 0, 0, -1, 0, 0, 1};

bool comp(const Point3d& lhs, const Point3d& rhs)
{
    return lhs[2] < rhs[2] || lhs[2] == rhs[2] && lhs[1] < rhs[1] || lhs[2] == rhs[2] && lhs[1] == rhs[1] && lhs[0] < rhs[0];
}

Points3d parse(const Lines& lines)
{
    Points3d result;
    const auto toPoint3d = [](const std::string& line) -> Point3d
    {
        int x, y, z;
        sscanf(line.c_str(), "%i,%i,%i", &x, &y, &z);
        return {x, y, z};
    };

    std::transform(begin(lines), end(lines), std::back_inserter(result), toPoint3d);
    return result;
}

void print(const Points3d& points3d)
{
    std::for_each(begin(points3d), end(points3d), [](const Point3d& p) { printf("%i,%i,%i\n", p[0], p[1], p[2]); });
}

Grid floodFill(const Grid& lavaDroplets, const Point3d& min, const Point3d& max, const Point3d& current)
{
    Grid air;
    std::vector<Point3d> toFill{current};
    while (!toFill.empty())
    {
        const auto p = toFill.front();
        toFill.erase(begin(toFill));

        for (const Point3d& n : neighbours) {
            Point3d np {p[0] + n[0], p[1] + n[1], p[2] + n[2]};

            if (np[0] < min[0] || np[0] > max[0] ||
                np[1] < min[1] || np[1] > max[1] ||
                np[2] < min[2] || np[2] > max[2])
            {
                continue;
            }

            if (air.contains(np)) {
                continue;
            }
            else if (lavaDroplets.contains(np)) {
                continue;
            }

            air.insert(np);
            toFill.push_back(np);
        }
    }
    return air;
}

void solve(const char* filename)
{
    const Lines& lines = readFile(filename);
    Points3d points = parse(lines);
    print(points);

    Grid lavaDroplets;

    int minX = INF;
    int minY = INF;
    int minZ = INF;
    int maxX = 0;
    int maxY = 0;
    int maxZ = 0;

    for (const Point3d& p : points)
    {
        lavaDroplets.insert(p);

        minX = std::min(minX, p[0]);
        minY = std::min(minY, p[1]);
        minZ = std::min(minZ, p[2]);
        maxX = std::max(maxX, p[0]);
        maxY = std::max(maxY, p[1]);
        maxZ = std::max(maxZ, p[2]);
    }
    printf("min: %i,%i,%i\n", minX, minY, minZ);
    printf("max: %i,%i,%i\n", maxX, maxY, maxZ);

    Grid air = floodFill(lavaDroplets, {minX-1, minY-1, minZ-1}, {maxX+1, maxY+1, maxZ+1}, {minX-1, minY-1, minZ-1});

    for (const auto& ff: air) {
        printf("%i,%i,%i\n", ff[0], ff[1], ff[2]);
    }
    printf("Air: %zi\n", air.size());

    int part1 = 0;
    int part2 = 0;
    for (int z = minZ; z <= maxZ; ++z)
    {
        for (int y = minY; y <= maxY; ++y)
        {
            for (int x = minX; x <= maxX; ++x)
            {
                const auto doesNotContain = [&lavaDroplets, x, y, z](const Point3d& n) {
                    return !lavaDroplets.contains({x + n[0], y + n[1], z + n[2]});
                };
                const auto reachableFromSurface = [&air, x, y, z](const Point3d& n) {
                    return air.contains({x + n[0], y + n[1], z + n[2]});
                };
                if (lavaDroplets.contains({x, y, z}))
                {
                    part1 += std::count_if(begin(neighbours), end(neighbours), doesNotContain);
                    part2 += std::count_if(begin(neighbours), end(neighbours), reachableFromSurface);
                }
            }
        }
    }
    printf("Part 1: %i\n", part1);
    printf("Part 2: %i\n", part2);
}

void test() {}

int main(int argc, char* argv[])
{
    test();

    const char* filename = argc > 1 ? argv[1] : "day18.sample";
    solve(filename);
}