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
    }

    std::unordered_set<Point> board;
    int width = 7;
    int height = 5;
};

void print(const Board& board) {
    for (int y = board.height; y >= 0; --y) {
        printf("%3i |", y);
        for (int x = 0; x < board.width; ++x) {
            const char c = board.at(x, y) ? '%' : ' ';
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





void solve(const char* filename) {
    const Pieces pieces = createPieces();
    Board board;

    for (int i = 0; i < 5; ++i) {
        board.height = fall(board, pieces[i], Point{3, board.height}) + 3;
        print(board);    
    }
}

void test() {

}

int main() {
    test();

    constexpr char filename[] { "day-17.sample" };
    solve(filename);
    printf("Done\n");
}