#include "hash.h"
#include "utils.h"
#include "point.h"
#include "ut.hpp"
#include <algorithm>
#include <array>
#include <deque>
#include <map>
#include <numeric>
#include <vector>
#include <unordered_set>

using Wind = std::unordered_set<Point>;
using Winds = std::vector<Wind>;
using State = std::array<int, 3>;   // time, x, y
using Seen = std::unordered_set<State, ArrayHash<3>>;

const std::vector<Point> validMoves{{1, 0}, {-1, 0}, {0, -1}, {0, 1}, {0, 0}};

// state modifiers: index of the wind, direction of the wind
const std::vector<std::tuple<int, int, int>> stateModifiers{{0, -1, 0}, {1, 1, 0}, {2, 0, -1}, {3, 0, 1}};

struct ParseResult
{
    Winds winds;
    Point start;
    Point goal;
    int width;
    int height;
};

ParseResult parse(const Lines& lines)
{
    const int height = lines.size() - 2;
    const int width = lines[0].size() - 2;

    auto findDot = [](const std::string& l) -> int
    {
        for (int x = 0; x < l.size(); ++x)
        {
            if (l[x] == '.')
            {
                return x;
            }
        }
        return -1;
    };

    Winds winds{4, Wind{}};
    for (int y = 1; y <= height; ++y)
    {
        for (int x = 1; x <= width; ++x)
        {
            const char c = lines[y][x];
            if (c != '.')
            {
                const int i = c == '<' ? 0 : c == '>' ? 1 : c == '^' ? 2 : 3;

                const int wx = x - 1;
                const int wy = y - 1;
                winds[i].insert({wx, wy});
            }
        }
    }

    Point start{findDot(lines.front()) - 1, -1};
    Point goal{findDot(lines.back()) - 1, height};

    return {.winds = std::move(winds), .start = start, .goal = goal, .width = width, .height = height};
}

int traverse_bfs(const Winds& winds, const Point& start, const Point& goal, int initTime, int width, int height) {
    std::deque<State> queue;
    queue.push_back({initTime, start.x, start.y});
    Seen seen;

    const int lcm = width * height / std::gcd(width, height);

    while (!queue.empty())
    {
        const auto& [t, x, y] = queue.front();
        queue.pop_front();

        const int time = t + 1;

        for (const auto [dx, dy] : validMoves)
        {
            const int nextX = x + dx;
            const int nextY = y + dy;

            if (nextX == goal.x && nextY == goal.y)
            {
                return time;
            }

            if ((nextX < 0 || nextY < 0 || nextX >= width || nextY >= height) && !(nextX == start.x && nextY == start.y))
            {
                continue;
            }

            bool collide = false;
            if (!(nextX == start.x && nextY == start.y))
            {
                for (const auto [i, dx, dy] : stateModifiers)
                {
                    int ttx = (nextX - dx * time) % width;
                    if (ttx < 0) ttx += width;
                    int tty = (nextY - dy * time) % height;
                    if (tty < 0) tty += height;
                    const Point p{ttx, tty};

                    if (winds[i].contains(p))
                    {
                        collide = true;
                        break;
                    }
                }
            }

            if (!collide)
            {
                const State key{time % lcm, nextX, nextY};
                if (seen.contains(key))
                {
                    continue;
                }
                queue.push_back({time, nextX, nextY});
                seen.insert(key);
            }
        
        }
    }
    return -1;
}

void part1(const Winds& winds, const Point& start, const Point& goal, int width, int height)
{
    const int time = traverse_bfs(winds, start, goal, 0, width, height);
    printf("Part 1: %i\n", time);
}

void part2(const Winds& winds, const Point& start, const Point& goal, int width, int height)
{
    const int t1 = traverse_bfs(winds, start, goal, 0, width, height);
    const int t2 = traverse_bfs(winds, goal, start, t1, width, height);
    const int t3 = traverse_bfs(winds, start, goal, t2, width, height);
    printf("Part 2: %i %i %i\n", t3, t2, t1);
}

void solve(const char* filename)
{
    const Lines& lines = readFile(filename);
    const auto& [winds, start, goal, width, height] = parse(lines);

    part1(winds, start, goal, width, height);
    part2(winds, start, goal, width, height);
}

void test() {}

int main(int argc, char* argv[])
{
    test();

    const char* filename = argc > 1 ? argv[1] : "day-24.sample";
    solve(filename);
}