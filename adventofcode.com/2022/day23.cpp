#include "point.h"
#include "utils.h"
#include "ut.hpp"
#include "ut-utils.h"
#include <algorithm>
#include <array>
#include <unordered_set>
#include <span>

constexpr int Inf = 10000;

namespace Dir {
constexpr int North = 0;
constexpr int South = 1;
constexpr int West = 2;
constexpr int East = 3;
constexpr int Count = 4;
};

using Elves = std::unordered_set<Point>;
using Points = std::vector<Point>;
using Move = std::pair<Point, Point>;
using Moves = std::vector<Move>;

const std::vector<Point> allNeighbours {Point{-1, -1}, Point{0, -1}, Point{1, -1}, Point{-1, 0}, Point{1, 0}, Point{-1, 1}, Point{0, 1}, Point{1, 1}};
const std::vector<Points> dirNeighbours {{{-1, -1}, {0, -1}, {1, -1}}, {{-1, 1}, {0, 1}, {1, 1}}, {{-1, -1}, {-1, 0}, {-1, 1}}, {{1, -1}, {1, 0}, {1, 1}}};

Point North(const Point& p) {
    return {p.x, p.y-1};
}

Point South(const Point& p) {
    return {p.x, p.y+1};
}

Point West(const Point& p) {
    return {p.x-1, p.y};
}

Point East(const Point& p) {
    return {p.x+1, p.y};
}

Elves findNeighbours(const Elves& elves, const Point& position, const std::vector<Point>& offsets) {
    Elves result;
    // for (const auto& elf: elves) {
    //     if (std::any_of(std::begin(offsets), std::end(offsets), 
    //         [&elf, &position](const Point& p) { return elf.x == p.x + position.x && elf.y == p.y + position.y; })) {
    //         result.insert(elf);
    //     }
    // }
    for (const Point& o: offsets) {
        const Point p {position.x + o.x, position.y + o.y};
        if (elves.contains(p)) {
            result.insert(p);
        }
    }
    return result;
}

Move proposeMove(const Elves& elves, const Point& position, const std::vector<Points>& dirNeighbours, int currentDir) {
    if (findNeighbours(elves, position, allNeighbours).empty()) {
        return {position, position};
    }

    for (int i = 0; i < Dir::Count; ++i) {
        const int d = (currentDir + i) % Dir::Count;
        const Elves neighbours = findNeighbours(elves, position, dirNeighbours[d]);
        if (neighbours.empty()) {
            if (d == Dir::North) return {position, North(position)};
            if (d == Dir::South) return {position, South(position)};
            if (d == Dir::West) return {position, West(position)};
            if (d == Dir::East) return {position, East(position)};
        }
    }

    return {position, position};
}

Moves proposeMoves(const Elves& elves, const std::vector<Points>& dirNeighbours, int currentDir) {
    Moves result;

    std::transform(begin(elves), end(elves), std::back_inserter(result), [&elves, &dirNeighbours, currentDir](const Point& elf) {
        return proposeMove(elves, elf, dirNeighbours, currentDir);
    });

    return result;
}

Moves applyRules(const Moves& moves) {
    std::unordered_map<Point, Points> possibleMoves;
    for (const Move& m: moves) {
        possibleMoves[m.second].push_back(m.first);
    }

    bool adjusted = false;
    do {
        adjusted = false;
        for (const auto [to, from]: possibleMoves) {
            if (from.size() > 1) {
                possibleMoves.erase(to);

                for (const auto& f: from) {
                    possibleMoves[f].push_back(f);
                }

                adjusted = true;
                break;
            }
        }

    } while (adjusted);

    Moves result;
    for (const auto& [to, from]: possibleMoves) {
        result.push_back({from.front(), to});
    }
    return result;
}

Elves executeMoves(const Moves& moves) {
    const Moves validMoves = applyRules(moves);

    Elves result;
    for (const auto& [from, to]: validMoves) {
        result.insert(to);
    }

    return result;
}

Elves parse(const Lines& lines) {
    Elves result;
    for (int y = 0; y < lines.size(); ++y) {
        for (int x = 0; x < lines[0].size(); ++x) {
            if (lines[y][x] == '#') {
                result.insert({x, y});
            }
        }
    }
    return result;
}

void printResults(const Elves& elves, int part) {

}

void part1(Elves elves, int count = 10) {
    for (int i = 0; i < count; ++i) {
        printf("turn %i\n", i);
        const Moves moves = proposeMoves(elves, dirNeighbours, i);
        elves = executeMoves(moves);
    }

    int minX = Inf;
    int minY = Inf;
    int maxX = -Inf;
    int maxY = -Inf;
    for (const Point& p: elves) {
        minX = std::min(minX, p.x);
        minY = std::min(minY, p.y);
        maxX = std::max(maxX, p.x);
        maxY = std::max(maxY, p.y);
    }

    const int area = (maxX - minX + 1) * (maxY - minY + 1);
    const int emptyArea = area - elves.size();
    printf("Part 1: %i %zi %i\n", area, elves.size(), emptyArea);
}

void part2(Elves elves) {
    int count = 0;
    do {
        const Moves moves = proposeMoves(elves, dirNeighbours, count++);
        const Moves validMoves = applyRules(moves);

        elves = executeMoves(validMoves);
        const bool moved = std::any_of(begin(validMoves), end(validMoves), [](const Move& m) {
            return m.first != m.second;
        });

        if (!moved) {
            break;
        }

        printf("turn %i\n", count);
    } while(true);

    printf("Part 2: %i\n", count);
}

void solve(const char* filename)
{
    const Lines& lines = readFile(filename);
    Elves elves = parse(lines);

    part1(elves);
    part2(elves);
}

void test() {
    using namespace boost::ut;

    "When no other elves at all, do nothing"_test = [] {
        const Elves elves;
        const Point position {3,3};
        expect(proposeMove(elves, position, dirNeighbours, Dir::North) == Move {position, position});
    };

    "When no other leves around, do nothing"_test = [] {
        const Elves elves {{1,1}, {5,1}, {1, 5}, {5,5}};
        const Point position {3,3};
        expect(proposeMove(elves, position, dirNeighbours, Dir::North) == Move {position, position});
    };

    "When North direction is not blocked, go North"_test = []{
        const Elves elves {{2,2}};
        const Point p {2,1};
        expect(proposeMove(elves, p, dirNeighbours, Dir::North) == Move{p, North(p)});
    };

    "When South direction is blocked, go South"_test = []{
        const Elves elves {{2,2}};
        const Point p {2,3};
        expect(proposeMove(elves, p, dirNeighbours, Dir::South) == Move{p, South(p)});
    };

    "When West direction is blocked, go West"_test = []{
        const Elves elves {{2,2}};
        const Point p {1,2};
        expect(proposeMove(elves, p, dirNeighbours, Dir::West) == Move{p, West(p)});
    };

    "When East direction is blocked, go East"_test = []{
        const Elves elves {{2,2}};
        const Point p {3,2};
        expect(proposeMove(elves, p, dirNeighbours, Dir::East) == Move{p, East(p)});
    };

    "When a direction is blocked, try next direction"_test = [] {
        const Point p {2,2};
        const Elves eastExit {       {2,1},
                              {1,2},
                                     {2,3}};
        expect(proposeMove(eastExit, p, dirNeighbours, Dir::North) == Move{p, East(p)});

        const Elves northExit {{1,2},      {3,2},
                                     {2,3}};
        expect(proposeMove(northExit, p, dirNeighbours, Dir::South) == Move{p, North(p)});

        const Elves southExit {      {2,1},
                               {1,2},      {3,2}};
        expect(proposeMove(southExit, p, dirNeighbours, Dir::West) == Move{p, South(p)});

        const Elves westExit {{2,1},
                                    {3,2},
                              {2,3}};
        expect(proposeMove(westExit, p, dirNeighbours, Dir::East) == Move{p, West(p)});
    };
    
    "When all directions are blocked, do nothing"_test = []{
        // North
        const Elves elves {       {2,1},
                           {1,2},        {3,2},
                                  {2,3}       };
        const Point p {2,2};
        expect(proposeMove(elves, p, dirNeighbours, Dir::North) == Move{p, p});
    };

    "Find neighbours"_test = []{
        // North
        const Elves elves {{1,1}, {2,1}, {3,1},
                           {1,2},        {3,2},
                           {1,3}, {2,3}, {3,3}};
    
        expect(findNeighbours(elves, {2,4}, dirNeighbours[Dir::North]) == Elves{{1,3}, {2,3}, {3,3}});
        expect(findNeighbours(elves, {0,4}, dirNeighbours[Dir::North]) == Elves{{1,3}});
        expect(findNeighbours(elves, {4,4}, dirNeighbours[Dir::North]) == Elves{{3,3}});

        expect(findNeighbours(elves, {2,0}, dirNeighbours[Dir::South]) == Elves{{1,1}, {2,1}, {3,1}});
        expect(findNeighbours(elves, {0,0}, dirNeighbours[Dir::South]) == Elves{{1,1}});
        expect(findNeighbours(elves, {4,0}, dirNeighbours[Dir::South]) == Elves{{3,1}});

        expect(findNeighbours(elves, {4,2}, dirNeighbours[Dir::West]) == Elves{{3,1}, {3,2}, {3,3}});
        expect(findNeighbours(elves, {4,0}, dirNeighbours[Dir::West]) == Elves{{3,1}});
        expect(findNeighbours(elves, {4,4}, dirNeighbours[Dir::West]) == Elves{{3,3}});

        expect(findNeighbours(elves, {0,2}, dirNeighbours[Dir::East]) == Elves{{1,1}, {1,2}, {1,3}});
        expect(findNeighbours(elves, {0,0}, dirNeighbours[Dir::East]) == Elves{{1,1}});
        expect(findNeighbours(elves, {0,4}, dirNeighbours[Dir::East]) == Elves{{1,3}});

        expect(findNeighbours(elves, {2,2}, allNeighbours) == elves);

        const Elves singleElf {{2,2}};
        expect(findNeighbours(singleElf, {2, 3}, dirNeighbours[Dir::North]) == singleElf);
        expect(findNeighbours(singleElf, {2, 1}, dirNeighbours[Dir::South]) == singleElf);
        expect(findNeighbours(singleElf, {3, 2}, dirNeighbours[Dir::West]) == singleElf);
        expect(findNeighbours(singleElf, {1, 2}, dirNeighbours[Dir::East]) == singleElf);
    };

    "When the square is empty, elves can go there"_test = []{
        expect(executeMoves({{{1,2},{2,2}}, {{0,0}, {1,0}}}) == Elves{{2,2}, {1,0}});
    };

    "When the square will be empty, elves can go there"_test = []{
        expect(executeMoves({{{0,0},{1,0}}, {{1,0}, {2,0}}}) == Elves{{2,0}, {1,0}});
    };

    "Only moves that takes empty space are allowed to move"_test = []{
        // moves that take square 2,2
        Moves moves {{{1,2},{2,2}}, {{2,1},{2,2}}, {{3,2},{2,2}}, {{1,3},{2,2}}};

        // moves that take unique squared
        moves.push_back({{0,0}, {0,1}});

        expect(executeMoves(moves) == Elves{{1,2},{2,1},{3,2},{1,3},{0,1}});
    };

    "When elves don't move, they block the current square"_test = []{
        // moves that take square 2,2
        Moves moves {{{1,2}, {2,2}}, {{2,1},{2,2}}};

        // moves that want to take currently taken square, that move will be also blocked
        moves.push_back({{1,1},{1,2}});

        expect(executeMoves(moves) == Elves{{1,2},{2,1},{1,1}});
    };

}

int main(int argc, char* argv[])
{
    test();

    const char* filename = argc > 1 ? argv[1] : "day23.sample";
    solve(filename);
}