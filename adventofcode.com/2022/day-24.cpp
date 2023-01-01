#include "utils.h"
#include "point.h"
#include "ut.hpp"
#include <algorithm>
#include <map>
#include <vector>

struct Wind
{
    Point position;
    Point direction;

    auto operator<=>(const Wind&) const = default;
};
using Winds = std::vector<Wind>;
using Maze = std::vector<std::vector<bool>>;
using Points = std::vector<Point>;

std::vector<Point> validMoves{{0, 0}, {1, 0}, {-1, 0}, {0, 1}, {0, -1}};

struct State {
    Winds winds;
    Point expedition;
    int time;
    auto operator<=>(const State&) const = default;
};

struct Node : public State {
    int gCost;
    int hCost;
    int fCost;

    auto operator<=>(const Node&) const = default;
};
inline bool operator<(const Node& lhs, const Node& rhs)
{
    return lhs.fCost < rhs.fCost || lhs.fCost == rhs.fCost && lhs.hCost < rhs.hCost;
}

struct ParseResult
{
    Winds winds;
    Point start;
    Point goal;
    int width;
    int height;
};

Maze emptyMaze(int width, int height)
{
    Maze result(height, std::vector<bool>(width, true));
    for (int y = 0; y < height; ++y)
    {
        result[y][0] = false;
        result[y][width - 1] = false;
    }

    for (int x = 0; x < width; ++x)
    {
        result[0][x] = false;
        result[height - 1][x] = false;
    }

    return result;
}

Winds simulate(Winds winds, int width, int height)
{
    for (Wind& w : winds)
    {
        w.position.x += w.direction.x;
        w.position.y += w.direction.y;

        if (w.position.x == 0)
        {
            w.position.x = width - 2;
        }
        else if (w.position.x == width - 1)
        {
            w.position.x = 1;
        }
        else if (w.position.y == 0)
        {
            w.position.y = height - 2;
        }
        else if (w.position.y == height - 1)
        {
            w.position.y = 1;
        }
    }
    return winds;
}

Maze createMaze(const Winds& winds, int width, int height)
{
    Maze result = emptyMaze(width, height);
    for (const Wind& w : winds)
    {
        result[w.position.y][w.position.x] = false;
    }
    return result;
}

ParseResult parse(const Lines& lines)
{
    const int height = lines.size();
    const int width = lines[0].size();

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

    Winds winds;
    for (int y = 1; y < height - 2; ++y)
    {
        for (int x = 1; x < width - 2; ++x)
        {
            const char c = lines[y][x];
            if (c == '<')
                winds.push_back(Wind{{x, y}, {-1, 0}});
            else if (c == '>')
                winds.push_back(Wind{{x, y}, {1, 0}});
            else if (c == '^')
                winds.push_back(Wind{{x, y}, {0, -1}});
            else if (c == 'v')
                winds.push_back(Wind{{x, y}, {0, 1}});
        }
    }

    Point start{findDot(lines.front()), 0};
    Point goal{findDot(lines.back()), height - 1};

    return {.winds = std::move(winds), .start = start, .goal = goal, .width = width, .height = height};
}



void part1(const Winds& winds, const Point& start, const Point& goal, int width, int height)
{
    std::vector<Node> openList;
    std::map<State, std::tuple<int,int,int>> wholeMap;
    std::map<State, bool> closedList;


    Node n {{winds, start, 0}, 0, 0, 0};
    openList.push_back(n);
    while (!openList.empty()) {
        std::sort(begin(openList), end(openList));

        const auto it = begin(openList);
        const Node node = *it;
        openList.erase(it);
        closedList[node] = true;

        const Winds winds = simulate(node.winds, width, height);
        const Maze maze = createMaze(winds, width, height);

        for (const Point& o : validMoves) {
            Point neighbour = {node.expedition.x + o.x, node.expedition.y + o.y};
            if (neighbour == goal)
            {
                // wholeMap[neighbour].previousRoom = currentRoom;
                // here we can construct the path [startRoom..endRoom] if needed
                break;
            }
            if (neighbour.x < 1 || neighbour.y < 1 || neighbour.x > width - 2 || neighbour.y > height - 2) {
                continue;
            }

            State newState {.winds = winds, .expedition = neighbour, .time = node.time + 1};
            if (closedList[newState] == false)
            {
                const int gNew = node.fCost + 1;
                const int hNew = goal.y - node.expedition.y + goal.x - node.expedition.y;
                const int fNew = gNew + hNew;

                // Check if this path is better than the one already present

                auto it = wholeMap.find(newState);
                if (it == end(wholeMap) || std::get<0>(it->second) > fNew)
                {
                    // Update the details of this neighbour node
                    wholeMap[newState] = std::make_tuple(fNew, gNew, hNew);

                    openList.emplace_back(newState);
                }
            }
        }
    }

    // std::vector<State> fifo;
    // fifo.push_back(s);

    // int count = 0;
    // while (!fifo.empty())
    // {
    //     const State current = fifo.front();

    //     printf("%i: %i %zi\n", ++count, current.count, fifo.size());

    //     if (current.expedition == current.goal || count == 20)
    //     {
    //         break;
    //     }

    //     const Winds winds = simulate(current.winds, current.width, current.height);
    //     const Maze maze = createMaze(winds, current.width, current.height);

    //     for (const Point& o : validMoves)
    //     {
    //         Point newE = {current.expedition.x + o.x, current.expedition.y + o.y};
    //         if (newE == current.goal) {
    //             break;
    //         }

    //         if (newE.x < 1 || newE.y < 1 || newE.x > current.width - 2 || newE.y > current.height - 2) {
    //             continue;
    //         }

    //         if (maze[newE.y][newE.x] == true)
    //         {
    //             printf("\tadding %i,%i %i,%i\n", newE.x, newE.y, o.x, o.y);
    //             fifo.push_back(State {.winds = winds,
    //                 .expedition = newE,
    //                 .goal = current.goal,
    //                 .count = current.count + 1,
    //                 .width = current.width,
    //                 .height = current.height});
    //         }
    //     }

    //     fifo.erase(begin(fifo));
    // }

    // printf("Part 1: %i\n", fifo.front().count);
}

void solve(const char* filename)
{
    const Lines& lines = readFile(filename);
    const auto&[winds, start, goal, width, height] = parse(lines);

    part1(winds, start, goal, width, height);
}

void test() {
    using namespace boost::ut;

    "when space in front is empty, wind moves"_test = []{
        // ######
        // #    #
        // # <^ #
        // # V> #
        // #    #
        // ######
        const Winds winds { {{2,2}, {-1, 0}}, {{3,2},{0,-1}}, {{3,3},{1,0}}, {{2,3},{0,1}}};
        const Winds given = simulate(winds, 6, 6);
        const Winds expected { {{1,2}, {-1, 0}}, {{3,1},{0,-1}}, {{4,3},{1,0}}, {{2,4},{0,1}}};
        expect(given == expected);
    };

    "when space in front is a wall, wind wraps"_test = []{
        // #####
        // #< ^#
        // #   #
        // #V >#
        // #####
        const Winds winds { {{1,1}, {-1, 0}}, {{3,1},{0,-1}}, {{3,3},{1,0}}, {{1,3},{0,1}}};
        const Winds given = simulate(winds, 5, 5);
        const Winds expected { {{3,1}, {-1, 0}}, {{3,3},{0,-1}}, {{1,3},{1,0}}, {{1,1},{0,1}}};
        expect(given == expected);
    };

    "winds can move to the same space"_test = []{
        // #####
        // # V #
        // #> <#
        // # ^ #
        // #####
        const Winds winds { {{3,2}, {-1, 0}}, {{2,3},{0,-1}}, {{1,2},{1,0}}, {{2,1},{0,1}}};
        const Winds given = simulate(winds, 5, 5);
        const Winds expected { {{2,2}, {-1, 0}}, {{2,2},{0,-1}}, {{2,2},{1,0}}, {{2,2},{0,1}}};
        expect(given == expected);
    };

    "winds can move out of the same space"_test = []{
        // #####
        // #   #
        // # 4 #
        // #   #
        // #####
        const Winds winds  { {{2,2}, {-1, 0}}, {{2,2},{0,-1}}, {{2,2},{1,0}}, {{2,2},{0,1}}};
        const Winds given = simulate(winds, 5, 5);
        const Winds expected { {{1,2}, {-1, 0}}, {{2,1},{0,-1}}, {{3,2},{1,0}}, {{2,3},{0,1}}};
        expect(given == expected);
    };

    "generate maze"_test = []{
        // #####
        // # V #
        // #> <#
        // # ^ #
        // #####
        const Winds winds { {{3,2}, {-1, 0}}, {{2,3},{0,-1}}, {{1,2},{1,0}}, {{2,1},{0,1}}};
        Maze maze = createMaze(winds, 5, 5);

        expect(maze[1][1] == true);
        expect(maze[3][1] == true);
        expect(maze[2][2] == true);
        expect(maze[1][3] == true);
        expect(maze[3][3] == true);

        expect(maze[2][1] == false);
        expect(maze[1][2] == false);
        expect(maze[3][2] == false);
        expect(maze[2][3] == false);
    };
}

int main(int argc, char* argv[])
{
    test();

    const char* filename = argc > 1 ? argv[1] : "day-24.sample";
    solve(filename);
}