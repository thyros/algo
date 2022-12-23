#include "point.h"
#include "utils.h"
#include "ut.hpp"
#include <string>
#include <vector>

enum class Direction
{
    Right = 0,
    Down = 1,
    Left = 2,
    Up = 3
};

struct Loc {
    Point p;
    Direction d;
    auto operator<=>(const Loc&) const = default;
};

struct Command {
    int steps;
    char r;
};
using Commands = std::vector<Command>;

using Maze = std::vector<std::string>;
const std::vector<Point> offsets = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

struct ParseResult {
    Maze maze;
    Commands commands;
};

ParseResult parse(const Lines& lines) {
    using namespace std::string_literals;
    const size_t width = lines[0].size();

    Maze maze;
    int i = 0;
    for (; i < lines.size(); ++i) {
        if (lines[i].empty())
            break;
        printf("%i: %zi\n", i, lines[i].size());

        std::string line(width, ' ');
        std::copy(begin(lines[i]), end(lines[i]), begin(line));
        maze.push_back(std::move(line));
    }

    const std::string& commandLine = lines[++i];
    Commands commands;
    size_t lhs = 0;
    while (lhs != std::string::npos) {
        const size_t rhs = commandLine.find_first_of("LR"s, lhs);
        
        const std::string number = commandLine.substr(lhs, rhs-lhs);
        const int steps = std::stoi(number);

        if (rhs != std::string::npos) {
            const char r = commandLine[rhs];
            commands.push_back({.steps = steps, .r = r});
            lhs = rhs + 1;
        } else {
            commands.push_back({.steps = steps, .r = 'X'});
            lhs = rhs;
        }
    }
    return {.maze = std::move(maze), .commands = std::move(commands)};
}


Point getNextPosition(const Maze& maze, Point location, Direction d) {
    const Point o = offsets[static_cast<int>(d)];
    Point p = location;

    do {
        p.x = (p.x + o.x + maze[0].size()) % maze[0].size();
        p.y = (p.y + o.y + maze.size()) % maze.size();
    } while (maze[p.y][p.x] == ' ');

    return p;
}

Point move(const Maze& maze, Point location, Direction d, int steps)
{
    printf("move %i %i %i %i\n", location.x, location.y, static_cast<int>(d), steps);
    Point result = location;
    for (int i = 0; i < steps; ++i) {
        const Point next = getNextPosition(maze, result, d);

        if (maze[next.y][next.x] != '.') {
            break;
        }
        result = next;
    }
    return result;
}

Loc processCommand(const Maze& maze, const Loc& current, const Command& c) {
    const Point p = move(maze, current.p, current.d, c.steps);
    const int delta = c.r == 'R' ? 1 : c.r == 'L' ? -1 : 0;
    const int directionsCount = 4;
    const auto d = static_cast<Direction>((static_cast<int>(current.d) + directionsCount + delta) % directionsCount);

    return {.p = p, .d = d};
}

Loc processCommands(const Maze& maze, Loc current, const Commands& commands) {
    for (const Command& command: commands) {
        current = processCommand(maze, current, command);
    }
    return current;
}

void part1(const Maze& maze, const Loc& start, const Commands& commands) {
    const Loc finish = processCommands(maze, start, commands);
    const int value = 1000 * (finish.p.y+1) + 4 * (finish.p.x+1) + static_cast<int>(finish.d);
    
    printf("Part 1: %i\n", value);
}

void solve(const char* filename)
{
    const Lines& lines = readFile(filename);
    const auto [maze, commands] = parse(lines);

    const int x = maze[0].find('.');
    const Loc start = {.p = {x, 0}, .d = Direction::Right};

    printf("%zi %zi %zi %i\n", maze[0].size(), maze.size(), commands.size(), start.p.x);


    part1(maze, start, commands);

}

void test()
{
    using namespace boost::ut;

    const Maze maze = {{"....."}, 
                       {"....."}, 
                       {"..#.."}, 
                       {"....."}, 
                       {"....."}};

    "single step"_test = [&maze] {
        expect(move(maze, Point{2, 2}, Direction::Right, 1) == Point{3, 2});
        expect(move(maze, Point{2, 2}, Direction::Down, 1) == Point{2, 3});
        expect(move(maze, Point{2, 2}, Direction::Left, 1) == Point{1, 2});
        expect(move(maze, Point{2, 2}, Direction::Up, 1) == Point{2, 1});
    };

    "long step"_test = [&maze] {
        const int s = maze.size() - 1;
        expect(move(maze, Point{0, 0}, Direction::Right, s) == Point{s, 0});
        expect(move(maze, Point{0, 0}, Direction::Down, s) == Point{0, s});
        expect(move(maze, Point{s, s}, Direction::Left, s) == Point{0, s});
        expect(move(maze, Point{s, s}, Direction::Up, s) == Point{s, 0});
    };

    "wall stops single step"_test = [&maze] {
        expect(move(maze, Point{1, 2}, Direction::Right, 1) == Point{1, 2});
        expect(move(maze, Point{2, 1}, Direction::Down, 1) == Point{2, 1});
        expect(move(maze, Point{3, 2}, Direction::Left, 1) == Point{3, 2});
        expect(move(maze, Point{2, 3}, Direction::Up, 1) == Point{2, 3});
    };

    "wall stops long step"_test = [&maze] {
        expect(move(maze, Point{0, 2}, Direction::Right, 3) == Point{1, 2});
        expect(move(maze, Point{2, 0}, Direction::Down, 3) == Point{2, 1});
        expect(move(maze, Point{4, 2}, Direction::Left, 3) == Point{3, 2});
        expect(move(maze, Point{2, 4}, Direction::Up, 3) == Point{2, 3});
    };

    "wrapping around edges single step"_test = [&maze] {
        const int s = maze.size() - 1;
        expect(move(maze, Point{0, 0}, Direction::Left, 1) == Point{s, 0});
        expect(move(maze, Point{0, 0}, Direction::Up, 1) == Point{0, s});
        expect(move(maze, Point{s, s}, Direction::Right, 1) == Point{0, s});
        expect(move(maze, Point{s, s}, Direction::Down, 1) == Point{s, 0});
    };

    "wrapping around edges long step"_test = [&maze] {
        const int s = maze.size() - 1;
        expect(move(maze, Point{1, 1}, Direction::Left, 3) == Point{s-1, 1});
        expect(move(maze, Point{1, 1}, Direction::Up, 3) == Point{1, s-1});
        expect(move(maze, Point{s-1, s-1}, Direction::Right, 3) == Point{1, s-1});
        expect(move(maze, Point{s-1, s-1}, Direction::Down, 3) == Point{s-1, 1});
    };

    const Maze wrappingMaze = {{"..#"}, 
                               {"..."}, 
                               {"#.."}};

    "wall stops wrapping"_test = [&m = wrappingMaze] {
        expect(move(m, Point{1, 0}, Direction::Left, 3) == Point{0, 0});
        expect(move(m, Point{0, 1}, Direction::Up, 3) == Point{0, 0});
        expect(move(m, Point{1, 2}, Direction::Right, 3) == Point{2, 2});
        expect(move(m, Point{2, 1}, Direction::Down, 3) == Point{2, 2});

        expect(move(m, Point{0,0}, Direction::Left, 1) == Point{0,0});
        expect(move(m, Point{0,0}, Direction::Up, 1) == Point{0,0});
        expect(move(m, Point{2,2}, Direction::Right, 1) == Point{2,2});
        expect(move(m, Point{2,2}, Direction::Down, 1) == Point{2,2});
    };

    const Maze gapsMaze = {{"  ..."},
                           {"  ..."},
                           {"....."},
                           {"...  "},
                           {"...  "}};

    "wrapping around gaps long step"_test = [&m = gapsMaze] {
        expect(move(m, Point{1, 3}, Direction::Right, 3) == Point{1, 3});
        expect(move(m, Point{1, 3}, Direction::Down, 3) == Point{1, 3});
        expect(move(m, Point{1, 3}, Direction::Left, 3) == Point{1, 3});
        expect(move(m, Point{1, 3}, Direction::Up, 3) == Point{1, 3});
    };

    const Maze gapsWallsMaze = {{"  .#."},
                                {"  ..#"},
                                {"....."},
                                {"#..  "},
                                {".#.  "}};

    "walls stop wrapping around gaps long step"_test = [&m = gapsWallsMaze] {
        expect(move(m, Point{3, 1}, Direction::Left, 3) == Point{2, 1});
        expect(move(m, Point{3, 1}, Direction::Down, 3) == Point{3, 2});
        expect(move(m, Point{1, 3}, Direction::Right, 3) == Point{2, 3});
        expect(move(m, Point{1, 3}, Direction::Up, 3) == Point{1, 2});
    };

    const Maze bigEmptyMaze = {{"........."},
                               {"........."},
                               {"........."},
                               {"........."},
                               {"........."},
                               {"........."},
                               {"........."},
                               {"........."},
                               {"........."}};

    "processing single move"_test = [&m = bigEmptyMaze] {
        const Loc locR {.p = {5, 5}, .d = Direction::Right };
        expect(processCommand(m, locR, {.steps = 3, .r = 'R'}) == Loc {.p = {8, 5}, .d = Direction::Down});
        expect(processCommand(m, locR, {.steps = 3, .r = 'L'}) == Loc {.p = {8, 5}, .d = Direction::Up});

        const Loc locD {.p = {5, 5}, .d = Direction::Down };
        expect(processCommand(m, locD, {.steps = 3, .r = 'R'}) == Loc {.p = {5, 8}, .d = Direction::Left});
        expect(processCommand(m, locD, {.steps = 3, .r = 'L'}) == Loc {.p = {5, 8}, .d = Direction::Right});

        const Loc locL {.p = {5, 5}, .d = Direction::Left };
        expect(processCommand(m, locL, {.steps = 3, .r = 'R'}) == Loc {.p = {2, 5}, .d = Direction::Up});
        expect(processCommand(m, locL, {.steps = 3, .r = 'L'}) == Loc {.p = {2, 5}, .d = Direction::Down});

        const Loc locU {.p = {5, 5}, .d = Direction::Up };
        expect(processCommand(m, locU, {.steps = 3, .r = 'R'}) == Loc {.p = {5, 2}, .d = Direction::Right});
        expect(processCommand(m, locU, {.steps = 3, .r = 'L'}) == Loc {.p = {5, 2}, .d = Direction::Left});
    };

    "processing multiple moves"_test = [&m = bigEmptyMaze] {
        const Loc locR {.p = {5, 3}, .d = Direction::Right };
        const Commands commands {{1, 'R'}, {2, 'R'}, {4, 'L'}, {1, 'L'}, {5, 'L'}, {6, 'L'}, {7, 'L'}};
            // (6,3,D) -> (6,5,L) -> (2,5,D) -> (2,6,R) -> (7,6,U) -> (7,0,L) -> (0,0,D)
        expect(processCommands(m, locR, commands) == Loc {.p = {0, 0}, .d = Direction::Down});
    };
}

int main(int argc, char* argv[])
{
    test();

    const char* filename = argc > 1 ? argv[1] : "day-22.input";
    solve(filename);
}