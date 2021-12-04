#include "utils.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <optional>
#include <string_view>

enum Tile {
    Blank,
    Free,
    Taken
};
using Line = std::vector<Tile>;
using Board = std::vector<Line>;

Tile ParseTile(const char c) {
    if (c == '.') return Tile::Blank;
    if (c == 'L') return Tile::Free;
    return Tile::Taken;
}

Line ParseLine(std::string_view text) {
    Line line;

    for (char c: text) {
        line.push_back(ParseTile(c));
    }

    return line;
}

std::ostream& operator<<(std::ostream& ostr, Tile tile) {
    if (tile == Tile::Blank) ostr << ".";
    else if (tile == Tile::Free) ostr << "L";
    else ostr << "#";
    return ostr;
}
std::ostream& operator<<(std::ostream& ostr, const Line& ts) {
    std::copy(begin(ts), end(ts), std::ostream_iterator<Tile>(ostr, ""));
    return ostr;
}
std::ostream& operator<<(std::ostream& ostr, const Board& board) {
    std::copy(begin(board), end(board), std::ostream_iterator<Line>(ostr, "\n"));
    return ostr;
}

std::optional<Tile> GetTile(const Board& board, int x, int y) {
    if (x < 0 || y < 0 || x >= static_cast<int>(board[0].size()) || y >= static_cast<int>(board.size())) {
        return {};
    }

    return std::make_optional(board[y][x]);
}

int CountSeatsTakenAround(const Board& board, int x, int y) {
    return 
        (GetTile(board, x - 1, y + 1).value_or(Tile::Blank) == Tile::Taken ? 1 : 0) +
        (GetTile(board, x    , y + 1).value_or(Tile::Blank) == Tile::Taken ? 1 : 0) +
        (GetTile(board, x + 1, y + 1).value_or(Tile::Blank) == Tile::Taken ? 1 : 0) +
        (GetTile(board, x - 1, y    ).value_or(Tile::Blank) == Tile::Taken ? 1 : 0) +
        (GetTile(board, x + 1, y    ).value_or(Tile::Blank) == Tile::Taken ? 1 : 0) +
        (GetTile(board, x - 1, y - 1).value_or(Tile::Blank) == Tile::Taken ? 1 : 0) +
        (GetTile(board, x    , y - 1).value_or(Tile::Blank) == Tile::Taken ? 1 : 0) +
        (GetTile(board, x + 1, y - 1).value_or(Tile::Blank) == Tile::Taken ? 1 : 0);
}

Tile CheckSeatsAround(const Board& board, int x, int y) {
    const int takenSeatsAround = CountSeatsTakenAround(board, x, y);
    const Tile currentTile = board[y][x];
    const Tile tile = [takenSeatsAround, currentTile]() -> Tile {
        if (takenSeatsAround == 0) return Tile::Taken;
        if (takenSeatsAround >= 4) return Tile::Free;
        return currentTile;
    }();
    return tile;
}

int CountSeatsTakenInLineOfSight(const Board& board, int x, int y) {
    const auto isSeatTakenInDirection = [&board, x, y](int dx, int dy) -> bool {
        std::optional<Tile> tile;
        int cx = x;
        int cy = y;
        do {
            cx += dx;
            cy += dy;
            tile = GetTile(board, cx, cy);
        } while ( );
        return tile.value_or(Tile::Blank) == Tile::Taken;
    };

    return isSeatTakenInDirection(-1,  1) ? 1 : 0 +
           isSeatTakenInDirection( 0,  1) ? 1 : 0 +
           isSeatTakenInDirection( 1,  1) ? 1 : 0 +
           isSeatTakenInDirection(-1,  0) ? 1 : 0 +
           isSeatTakenInDirection( 1,  0) ? 1 : 0 +
           isSeatTakenInDirection(-1, -1) ? 1 : 0 +
           isSeatTakenInDirection( 0, -1) ? 1 : 0 +
           isSeatTakenInDirection( 1, -1) ? 1 : 0;
}

Tile CheckSeatsInLineOfSight(const Board& board, int x, int y) {
    const int takenSeatsAround = CountSeatsTakenInLineOfSight(board, x, y);
    const Tile currentTile = board[y][x];
    const Tile tile = [takenSeatsAround, currentTile]() -> Tile {
        if (takenSeatsAround == 0) return Tile::Taken;
        if (takenSeatsAround >= 5) return Tile::Free;
        return currentTile;
    }();
    return tile;
}

Board Simulate(const Board& board, std::function<Tile(Board, int, int)> transformFunction) {
    Board result;
    for (size_t y = 0; y < board.size(); ++y) {
        result.emplace_back();
        for (size_t x = 0; x < board[0].size(); ++x) {
            Tile currentTile = board[y][x];
            if (currentTile == Tile::Blank) {
                result.back().push_back(Tile::Blank);
            }
            else {
                const Tile tile = transformFunction(board, x, y);
                result.back().push_back(tile);
            }
        }
    }
    return result;
}

int CountSeatsTaken(const Board& board) {
    int count = 0;
    for (const auto& line: board) {
        count += std::count(begin(line), end(line), Tile::Taken);
    }
    return count;
}

int SolvePartOne(Board newBoard) {
    Board board;
    int step = 0;
    do
    {
        std::cout << "Step: " << step++ << std::endl 
                  << newBoard << std::endl << std::endl;

        board = std::move(newBoard);
        newBoard = Simulate(board, CheckSeatsAround);

    } while(board != newBoard);

    std::cout << "Step: " << step++ << std::endl 
                << newBoard << std::endl << std::endl;
                
    return CountSeatsTaken(newBoard);
}

int SolvePartTwo(Board newBoard) {
    Board board;
    int step = 0;
    do
    {
        std::cout << "Step: " << step++ << std::endl 
                  << newBoard << std::endl << std::endl;

        board = std::move(newBoard);
        newBoard = Simulate(board, CheckSeatsInLineOfSight);

    } while(board != newBoard);

    std::cout << "Step: " << step++ << std::endl 
                << newBoard << std::endl << std::endl;
                
    return CountSeatsTaken(newBoard);
}

void Solve(std::string_view filename){
    std::cout << "Solving: " << filename << std::endl << std::endl;

	std::ifstream ifstr(filename.data());
	const std::vector<std::string> text = Utils::ParseInput(ifstr);

    const auto board = [&text]() -> Board
    {
        std::vector<Line> line;
        line.reserve(text.size());
        std::transform(begin(text), end(text), std::back_inserter(line), [](const std::string& t) { return ParseLine(t); });
        return line;
    }();

    // std::cout << "Seats Taken (part one): " << SolvePartOne(board) << std::endl;
    std::cout << "Seats Taken (part two): " << SolvePartTwo(board) << std::endl;
}

int main() {
    Solve("day-11.sample");
}