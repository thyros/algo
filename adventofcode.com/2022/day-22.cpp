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
using Wrap = std::function<Loc(const Maze&, Loc)>;
const std::vector<Point> offsets = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
const std::vector<char> dirs = {'R', 'D', 'L', 'U'};

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


Loc getNextPosition2D(const Maze& maze, Loc l) {
    const Point o = offsets[static_cast<int>(l.d)];
    do {
        l.p.x = (l.p.x + o.x + maze[0].size()) % maze[0].size();
        l.p.y = (l.p.y + o.y + maze.size()) % maze.size();
    } while (maze[l.p.y][l.p.x] == ' ');

    return l;
}

Loc getNextPosition3D(const Maze& maze, Loc l) {
    const Point o = offsets[static_cast<int>(l.d)];
    l.p.x += o.x;
    l.p.y += o.y;

    //      0   50  100 150
    // 0        1---2---3
    //          |   |   |
    //          |   |   |
    //          |   |   |
    //          4---O---5
    // 50       |   |
    //          |   |
    //          |   |
    //      4---O---5
    // 100  |   |   |
    //      |   |   |
    //      |   |   |
    //      1---O---3
    // 150  |   |
    //      |   |
    //      |   |
    //      2---3
    // 200

    printf("\t pos:(%i,%i), d:%c\n", l.p.x, l.p.y, dirs[static_cast<int>(l.d)]);
    if (50 <= l.p.x && l.p.x < 100 && l.p.y < 0 && l.d == Direction::Up) { // 1-2 edge
        l.d = Direction::Right;
        l.p.y = l.p.x + 100;
        l.p.x = 0;
        printf("1-2 edge pos:(%i,%i), d:%c\n", l.p.x, l.p.y, dirs[static_cast<int>(l.d)]);
    } else if (l.p.x < 0 && 150 <= l.p.y && l.p.y < 200 && l.d == Direction::Left) { // 1-2 edge
        l.d = Direction::Down;
        l.p.x = l.p.y - 100;
        l.p.y = 0;
        printf("1-2 edge pos:(%i,%i), d:%c\n", l.p.x, l.p.y, dirs[static_cast<int>(l.d)]);
    } else if (100 <= l.p.x && l.p.x < 150 && l.p.y < 0 && l.d == Direction::Up) { // 2-3 edge
        l.p.y = 199;
        l.p.x = l.p.x - 100;
        printf("2-3 edge pos:(%i,%i), d:%c\n", l.p.x, l.p.y, dirs[static_cast<int>(l.d)]);
    } else if (0 <= l.p.x && l.p.x < 50 && l.p.y >= 200 && l.d == Direction::Down) { // 2-3 edge
        l.p.y = 0;
        l.p.x = l.p.x + 100;
        printf("2-3 edge pos:(%i,%i), d:%c\n", l.p.x, l.p.y, dirs[static_cast<int>(l.d)]);
    } else if (l.p.x == 150 && 0 <= l.p.y && l.p.y < 50 && l.d == Direction::Right) { // 3-5 edge
        l.d = Direction::Left;
        l.p.y = 149 - l.p.y;
        l.p.x = 99;
        printf("3-5 edge pos:(%i,%i), d:%c\n", l.p.x, l.p.y, dirs[static_cast<int>(l.d)]);
    } else if (l.p.x == 100 && 100 <= l.p.y && l.p.y < 150 && l.d == Direction::Right) { // 3-5 edge
        l.d = Direction::Left;
        l.p.y = 149 - l.p.y;
        l.p.x = 149;
        printf("3-5 edge pos:(%i,%i), d:%c\n", l.p.x, l.p.y, dirs[static_cast<int>(l.d)]);
    } else if (100 <= l.p.x && l.p.x < 150 && l.p.y == 50 && l.d == Direction::Down) { // O-5 edge
        l.d = Direction::Left;
        l.p.y = l.p.x - 50;
        l.p.x = 99;
        printf("O-5 edge pos:(%i,%i), d:%c\n", l.p.x, l.p.y, dirs[static_cast<int>(l.d)]);
    } else if (l.p.x == 100 && 50 <= l.p.y && l.p.y < 100 && l.d == Direction::Right) { // O-5 edge
        l.d = Direction::Up;
        l.p.x = l.p.y + 50;
        l.p.y = 49;
        printf("O-5 edge pos:(%i,%i), d:%c\n", l.p.x, l.p.y, dirs[static_cast<int>(l.d)]);
    } else if (50 <= l.p.x && l.p.x < 100 && l.p.y == 150 && l.d == Direction::Down) { // O-3 edge
        l.d = Direction::Left;
        l.p.y = l.p.x + 100;
        l.p.x = 49;
        printf("O-3 edge pos:(%i,%i), d:%c\n", l.p.x, l.p.y, dirs[static_cast<int>(l.d)]);
    } else if (l.p.x == 50 && 150 <= l.p.y && l.p.y < 200 && l.d == Direction::Right) { // O-3 edge
        l.d = Direction::Up;
        l.p.x = l.p.y - 100;
        l.p.y = 149;
        printf("O-3 edge pos:(%i,%i), d:%c\n", l.p.x, l.p.y, dirs[static_cast<int>(l.d)]);
    } else if (0 <= l.p.x && l.p.x < 50 && l.p.y == 99 && l.d == Direction::Up) { // 4-O edge
        l.d = Direction::Right;
        l.p.y = l.p.x + 50;
        l.p.x = 50;
        printf("4-O edge pos:(%i,%i), d:%c\n", l.p.x, l.p.y, dirs[static_cast<int>(l.d)]);
    } else if (l.p.x == 49 && 50 <= l.p.y && l.p.y < 100 && l.d == Direction::Left) { // 4-0 edge
        l.d = Direction::Down;
        l.p.x = l.p.y - 50;
        l.p.y = 100;
        printf("4-O edge pos:(%i,%i), d:%c\n", l.p.x, l.p.y, dirs[static_cast<int>(l.d)]);
    } else if (l.p.x == 49 && 0 <= l.p.y && l.p.y < 50 && l.d == Direction::Left) { // 1-4 edge
        l.d = Direction::Right;
        l.p.y = 149 - l.p.y;
        l.p.x = 0;
        printf("1-4 edge pos:(%i,%i), d:%c\n", l.p.x, l.p.y, dirs[static_cast<int>(l.d)]);
    } else if (l.p.x < 0 && 100 <= l.p.y && l.p.y < 150 && l.d == Direction::Left) { // 1-4 edge
        l.d = Direction::Right;
        l.p.y = 149 - l.p.y;
        l.p.x = 50;
        printf("1-4 edge pos:(%i,%i), d:%c\n", l.p.x, l.p.y, dirs[static_cast<int>(l.d)]);
    }
    return l;
}

Loc move(const Maze& maze, Loc current, int steps, const Wrap& wrap)
{
    printf("move pos:(%i,%i) dir:%c step:%i\n", current.p.x, current.p.y, dirs[static_cast<int>(current.d)], steps);
    for (int i = 0; i < steps; ++i) {
        const Loc next = wrap(maze, current);

        if (maze[next.p.y][next.p.x] != '.') {
            break;
        }
        current = next;
    }
    return current;
}

Loc processCommand(const Maze& maze, const Loc& current, const Command& c, const Wrap& wrap) {
    const Loc next = move(maze, current, c.steps, wrap);
    const int delta = c.r == 'R' ? 1 : c.r == 'L' ? -1 : 0;
    const int directionsCount = 4;
    const auto d = static_cast<Direction>((static_cast<int>(next.d) + directionsCount + delta) % directionsCount);

    return {.p = next.p, .d = d};
}

Loc processCommands(const Maze& maze, Loc current, const Commands& commands, const Wrap& wrap) {
    for (const Command& command: commands) {
        current = processCommand(maze, current, command, wrap);
    }
    return current;
}

void partX(const Maze& maze, const Loc& start, const Commands& commands, int i, const Wrap& wrap) {
    const Loc finish = processCommands(maze, start, commands, wrap);
    const int value = 1000 * (finish.p.y+1) + 4 * (finish.p.x+1) + static_cast<int>(finish.d);
    
    printf("Part %i: %i\n", i, value);
}

void solve(const char* filename)
{
    printf("Solving...\n");
    const Lines& lines = readFile(filename);
    const auto [maze, commands] = parse(lines);

    const int x = maze[0].find('.');
    const Loc start = {.p = {x, 0}, .d = Direction::Right};

    printf("w:%zi h:%zi commands:%zi start:(%i,0)\n", maze[0].size(), maze.size(), commands.size(), start.p.x);

    partX(maze, start, commands, 1, getNextPosition2D);
    partX(maze, start, commands, 2, getNextPosition3D);
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
        expect(move(maze, {.p=Point{2, 2}, .d=Direction::Right}, 1, getNextPosition2D) == Loc{.p=Point{3, 2}, .d=Direction::Right});
        expect(move(maze, {.p=Point{2, 2}, .d=Direction::Down}, 1, getNextPosition2D) == Loc{.p=Point{2, 3}, .d=Direction::Down});
        expect(move(maze, {.p=Point{2, 2}, .d=Direction::Left}, 1, getNextPosition2D) == Loc{.p=Point{1, 2}, .d=Direction::Left});
        expect(move(maze, {.p=Point{2, 2}, .d=Direction::Up}, 1, getNextPosition2D) == Loc{.p=Point{2, 1}, .d=Direction::Up});
    };

    "long step"_test = [&maze] {
        const int s = maze.size() - 1;
        expect(move(maze, {.p=Point{0, 0}, .d=Direction::Right}, s, getNextPosition2D) == Loc{.p=Point{s, 0}, .d=Direction::Right});
        expect(move(maze, {.p=Point{0, 0}, .d=Direction::Down}, s, getNextPosition2D) == Loc{.p=Point{0, s}, .d=Direction::Down});
        expect(move(maze, {.p=Point{s, s}, .d=Direction::Left}, s, getNextPosition2D) == Loc{.p=Point{0, s}, .d=Direction::Left});
        expect(move(maze, {.p=Point{s, s}, .d=Direction::Up}, s, getNextPosition2D) == Loc{.p=Point{s, 0}, .d=Direction::Up});
    };

    "wall stops single step"_test = [&maze] {
        expect(move(maze, {.p=Point{1, 2}, .d=Direction::Right}, 1, getNextPosition2D) == Loc{.p=Point{1, 2}, .d=Direction::Right});
        expect(move(maze, {.p=Point{2, 1}, .d=Direction::Down}, 1, getNextPosition2D) == Loc{.p=Point{2, 1}, .d=Direction::Down});
        expect(move(maze, {.p=Point{3, 2}, .d=Direction::Left}, 1, getNextPosition2D) == Loc{.p=Point{3, 2}, .d=Direction::Left});
        expect(move(maze, {.p=Point{2, 3}, .d=Direction::Up}, 1, getNextPosition2D) == Loc{.p=Point{2, 3}, .d=Direction::Up});
    };

    "wall stops long step"_test = [&maze] {
        expect(move(maze, {.p=Point{0, 2}, .d=Direction::Right}, 3, getNextPosition2D) == Loc{.p=Point{1, 2}, .d=Direction::Right});
        expect(move(maze, {.p=Point{2, 0}, .d=Direction::Down}, 3, getNextPosition2D) == Loc{.p=Point{2, 1}, .d=Direction::Down});
        expect(move(maze, {.p=Point{4, 2}, .d=Direction::Left}, 3, getNextPosition2D) == Loc{.p=Point{3, 2}, .d=Direction::Left});
        expect(move(maze, {.p=Point{2, 4}, .d=Direction::Up}, 3, getNextPosition2D) == Loc{.p=Point{2, 3}, .d=Direction::Up});
    };

    "wrapping around edges single step"_test = [&maze] {
        const int s = maze.size() - 1;
        expect(move(maze, {.p=Point{0, 0}, .d=Direction::Left}, 1, getNextPosition2D) == Loc{.p=Point{s, 0}, .d=Direction::Left});
        expect(move(maze, {.p=Point{0, 0}, .d=Direction::Up}, 1, getNextPosition2D) == Loc{.p=Point{0, s}, .d=Direction::Up});
        expect(move(maze, {.p=Point{s, s}, .d=Direction::Right}, 1, getNextPosition2D) == Loc{.p=Point{0, s}, .d=Direction::Right});
        expect(move(maze, {.p=Point{s, s}, .d=Direction::Down}, 1, getNextPosition2D) == Loc{.p=Point{s, 0}, .d=Direction::Down});
    };

    "wrapping around edges long step"_test = [&maze] {
        const int s = maze.size() - 1;
        expect(move(maze, {.p=Point{1, 1}, .d=Direction::Left}, 3, getNextPosition2D) == Loc{.p=Point{s-1, 1}, .d=Direction::Left});
        expect(move(maze, {.p=Point{1, 1}, .d=Direction::Up}, 3, getNextPosition2D) == Loc{.p=Point{1, s-1}, .d=Direction::Up});
        expect(move(maze, {.p=Point{s-1, s-1}, .d=Direction::Right}, 3, getNextPosition2D) == Loc{.p=Point{1, s-1}, .d=Direction::Right});
        expect(move(maze, {.p=Point{s-1, s-1}, .d=Direction::Down}, 3, getNextPosition2D) == Loc{.p=Point{s-1, 1}, .d=Direction::Down});
    };

    const Maze wrappingMaze = {{"..#"}, 
                               {"..."}, 
                               {"#.."}};

    "wall stops wrapping"_test = [&m = wrappingMaze] {
        expect(move(m, {.p=Point{1, 0}, .d=Direction::Left}, 3, getNextPosition2D) == Loc{.p=Point{0, 0}, .d=Direction::Left});
        expect(move(m, {.p=Point{0, 1}, .d=Direction::Up}, 3, getNextPosition2D) == Loc{.p=Point{0, 0}, .d=Direction::Up});
        expect(move(m, {.p=Point{1, 2}, .d=Direction::Right}, 3, getNextPosition2D) == Loc{.p=Point{2, 2}, .d=Direction::Right});
        expect(move(m, {.p=Point{2, 1}, .d=Direction::Down}, 3, getNextPosition2D) == Loc{.p=Point{2, 2}, .d=Direction::Down});

        expect(move(m, {.p=Point{0,0}, .d=Direction::Left}, 1, getNextPosition2D) == Loc{.p=Point{0,0}, .d=Direction::Left});
        expect(move(m, {.p=Point{0,0}, .d=Direction::Up}, 1, getNextPosition2D) == Loc{.p=Point{0,0}, .d=Direction::Up});
        expect(move(m, {.p=Point{2,2}, .d=Direction::Right}, 1, getNextPosition2D) == Loc{.p=Point{2,2}, .d=Direction::Right});
        expect(move(m, {.p=Point{2,2}, .d=Direction::Down}, 1, getNextPosition2D) == Loc{.p=Point{2,2}, .d=Direction::Down});
    };

    const Maze gapsMaze = {{"  ..."},
                           {"  ..."},
                           {"....."},
                           {"...  "},
                           {"...  "}};

    "wrapping around gaps long step"_test = [&m = gapsMaze] {
        expect(move(m, {.p=Point{1, 3}, .d=Direction::Right}, 3, getNextPosition2D) == Loc{.p=Point{1, 3}, .d=Direction::Right});
        expect(move(m, {.p=Point{1, 3}, .d=Direction::Down}, 3, getNextPosition2D) == Loc{.p=Point{1, 3}, .d=Direction::Down});
        expect(move(m, {.p=Point{1, 3}, .d=Direction::Left}, 3, getNextPosition2D) == Loc{.p=Point{1, 3}, .d=Direction::Left});
        expect(move(m, {.p=Point{1, 3}, .d=Direction::Up}, 3, getNextPosition2D) == Loc{.p=Point{1, 3}, .d=Direction::Up});
    };

    const Maze gapsWallsMaze = {{"  .#."},
                                {"  ..#"},
                                {"....."},
                                {"#..  "},
                                {".#.  "}};

    "walls stop wrapping around gaps long step"_test = [&m = gapsWallsMaze] {
        expect(move(m, {.p=Point{3, 1}, .d=Direction::Left}, 3, getNextPosition2D) == Loc{.p=Point{2, 1}, .d=Direction::Left});
        expect(move(m, {.p=Point{3, 1}, .d=Direction::Down}, 3, getNextPosition2D) == Loc{.p=Point{3, 2}, .d=Direction::Down});
        expect(move(m, {.p=Point{1, 3}, .d=Direction::Right}, 3, getNextPosition2D) == Loc{.p=Point{2, 3}, .d=Direction::Right});
        expect(move(m, {.p=Point{1, 3}, .d=Direction::Up}, 3, getNextPosition2D) == Loc{.p=Point{1, 2}, .d=Direction::Up});
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
        expect(processCommand(m, locR, {.steps = 3, .r = 'R'}, getNextPosition2D) == Loc {.p = {8, 5}, .d = Direction::Down});
        expect(processCommand(m, locR, {.steps = 3, .r = 'L'}, getNextPosition2D) == Loc {.p = {8, 5}, .d = Direction::Up});

        const Loc locD {.p = {5, 5}, .d = Direction::Down };
        expect(processCommand(m, locD, {.steps = 3, .r = 'R'}, getNextPosition2D) == Loc {.p = {5, 8}, .d = Direction::Left});
        expect(processCommand(m, locD, {.steps = 3, .r = 'L'}, getNextPosition2D) == Loc {.p = {5, 8}, .d = Direction::Right});

        const Loc locL {.p = {5, 5}, .d = Direction::Left };
        expect(processCommand(m, locL, {.steps = 3, .r = 'R'}, getNextPosition2D) == Loc {.p = {2, 5}, .d = Direction::Up});
        expect(processCommand(m, locL, {.steps = 3, .r = 'L'}, getNextPosition2D) == Loc {.p = {2, 5}, .d = Direction::Down});

        const Loc locU {.p = {5, 5}, .d = Direction::Up };
        expect(processCommand(m, locU, {.steps = 3, .r = 'R'}, getNextPosition2D) == Loc {.p = {5, 2}, .d = Direction::Right});
        expect(processCommand(m, locU, {.steps = 3, .r = 'L'}, getNextPosition2D) == Loc {.p = {5, 2}, .d = Direction::Left});
    };

    "processing multiple moves"_test = [&m = bigEmptyMaze] {
        const Loc locR {.p = {5, 3}, .d = Direction::Right };
        const Commands commands {{1, 'R'}, {2, 'R'}, {4, 'L'}, {1, 'L'}, {5, 'L'}, {6, 'L'}, {7, 'L'}};
            // (6,3,D) -> (6,5,L) -> (2,5,D) -> (2,6,R) -> (7,6,U) -> (7,0,L) -> (0,0,D)
        expect(processCommands(m, locR, commands, getNextPosition2D) == Loc {.p = {0, 0}, .d = Direction::Down});
    };
}

int main(int argc, char* argv[])
{
    test();

    const char* filename = argc > 1 ? argv[1] : "day-22.input";
    solve(filename);
}