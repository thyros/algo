#include "utils.h"
#include "point.h"
#include <array>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

// move left-right valid ? move : do nothing
// move down valid ? move : settle down && spawn next piece

// piece - 4x4 matrix
// board 7xinf

constexpr int pieceSize = 4;
constexpr int piecesCount = 5;
using Piece = std::array<std::array<bool, pieceSize>, pieceSize>;
using Pieces = std::array<Piece, piecesCount>;

void print(const Pieces& pieces) {
    for (int i = 0; i < pieces.size(); ++i) {
        printf("next piece %i out of %zi\n", i, pieces.size());
        const Piece& piece = pieces[i];
        for (int y = 0; y < pieceSize; ++y) {
            printf("%i ", y);
            for (int x = 0; x < pieceSize; ++x) {
                const char 
                c = pieces[i][y][x] ? '#' : ' ';
                printf("%c", c);
            }
            printf("\n");
        }
        printf("\n");
    }
}


Pieces createPieces() {
    const std::array<const char*, 20> templ {
    "####",
    "    ",
    "    ",
    "    ",
    " #  ",
    "### ",
    " #  ",
    "    ",
    "  # ",
    "  # ",
    "### ",
    "    ",
    "#   ",
    "#   ",
    "#   ",
    "#   ",
    "##  ",
    "##  ",
    "    ",
    "    "};


    Pieces pieces;
    for (int gy = 0; gy < templ.size(); ++gy) {
        const char* l = templ[gy];   

        const int pi = gy / pieceSize;
        const int y = gy % pieceSize;
        Piece& piece = pieces[pi];
        for (int x = 0; x < pieceSize; ++x) {
            piece[y][x] = l[x] == '#';
        }
    }

    return pieces;
}

std::vector<int> gatherHeights(const Pieces& pieces) {
    std::vector<int> heights;
    for(const Piece& p: pieces) {
        for (int y = 0; y <= p.size(); ++y) {
            if (y == p.size() || std::none_of(begin(p[y]), end(p[y]), [](bool b) { return b; })) {
                heights.push_back(y);
                break;
            }
        }
    }

    return heights;
}

struct Board {
    bool at(int x, int y) const { 
        return board.count(Point {x, y}) > 0;
    }

    void add(const Piece& piece, const Point& position) {
        for (int y = 0; y < pieceSize; ++y) {
            for (int x = 0; x < pieceSize; ++x) {                
                if ( piece[y][x]) {
                    const int bx = position.x + x;
                    const int by = position.y - y;
                    board.insert(Point {bx, by});
                }
            }
        }

        height = std::max(height, position.y);        
    }

    std::unordered_set<Point> board;
    int width = 7;
    int height = -1;
};

int collapse(const Board& board) {
    for (int y = board.height; y >= 0; --y) {
        for (int x = 0; x < board.width; ++x) {
            if (board.at(x, y) == true) {
                return y;
            }
        }
    }
    return -1;
}

// 7
// 6  o 
// 5 ooo
// 4  o
// 3
// 2
//  0123456

// position = (1,6)

void print(const Board& board, const Piece& piece, const Point& position) {
    const auto hasPiece = [&piece, &position] (const int x, const int y) -> bool {
        if (x >= position.x && x < position.x + 4 && y <= position.y && y > position.y - 4) {
            return piece[position.y - y][x - position.x];
        }
        return false;
    };
    for (int y = board.height; y >= 0; --y) {
        printf("%3i |", y);
        for (int x = 0; x < board.width; ++x) {
            const char c = board.at(x, y) ? '%' : hasPiece(x,y) ? 'o' : '.';
            printf("%c", c);
        }
        printf("|\n");
    }
    printf("    +-------+\n");
}

bool willFit(const Board& board, const Piece& piece, const Point& position) {
    for (int y = 0; y < pieceSize; ++y) {
        for (int x = 0; x < pieceSize; ++x) {
            const int bx = position.x + x;
            const int by = position.y - y;
            if (piece[y][x] && (bx < 0 || bx >= board.width || by < 0 || board.at(bx, by))) {
                return false;
            }
        }
    }
    return true;
}

int fall(Board& board, const Piece& piece, Point position) {
    do {
        if (willFit(board, piece, Point{ position.x, position.y - 1})) {
            position.y -= 1;
        } else {
            board.add(piece, position);
            return position.y;
        }
    } while (true);
    return -1;
}

int simulate(const Pieces& pieces, const std::vector<int>& heights, const std::string& input, int count) {
    Board board;

    int ii = 0;
    for (int i = 0; i < count; ++i) {
        board.height += 3 + heights[i % heights.size()];

        const Piece& piece = pieces[i % pieces.size()];
        Point position {2, board.height};

        if (i % 100000 == 0)
        printf("%i\n", i);

        do {
            const int offset = input[ii++ % input.size()] == '<' ? -1 : 1;
            if (willFit(board, piece, Point {position.x + offset, position.y})) {
                position.x += offset;
            }

            if (willFit(board, piece, Point{ position.x, position.y - 1})) {
                position.y -= 1;
            } else {
                board.add(piece, position);
                break;
            }
        } while (true);

        board.height = collapse(board);
    }
    return board.height + 1;    // I count from 0
}

void part1(const Pieces& pieces, const std::vector<int>& heights, const std::string& input) {
    const int height = simulate(pieces, heights, input, 2022);
    printf("Part 1: %i\n", height);
}
void part2(const Pieces& pieces, const std::vector<int>& heights, const std::string& input) {
    const int height = simulate(pieces, heights, input, 100000);
    printf("Part 2: %i\n", height);
}

void solve(const char* filename) {
    const std::string input = readFile(filename)[0];
    const Pieces pieces = createPieces();
    const std::vector<int> heights = gatherHeights(pieces);

    part1(pieces, heights, input);
    // part2(pieces, heights, input);
}

void test() {

}

int main(int argc, char* argv[])
{
    test();

    const char* filename = argc > 1 ? argv[1] : "day-17.sample";
    solve(filename);
}