#include "hash.h"
#include "utils.h"
#include "pointL.h"
#include <array>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

constexpr long long pieceSize = 4;
constexpr long long piecesCount = 5;
using Piece = std::array<std::array<bool, pieceSize>, pieceSize>;
using Pieces = std::array<Piece, piecesCount>;

constexpr long long stateLength = 5;
using State = std::array<long long, stateLength + 2>; // for jet index and piece index
struct CacheValue {
    long long i, h;
};
using Cache = std::unordered_map<State, CacheValue, ArrayHash<long long, stateLength + 2>>;

void print(const Pieces& pieces) {
    for (long long i = 0; i < pieces.size(); ++i) {
        printf("next piece %lli out of %zi\n", i, pieces.size());
        const Piece& piece = pieces[i];
        for (long long y = 0; y < pieceSize; ++y) {
            printf("%lli ", y);
            for (long long x = 0; x < pieceSize; ++x) {
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
    for (long long gy = 0; gy < templ.size(); ++gy) {
        const char* l = templ[gy];   

        const long long pi = gy / pieceSize;
        const long long y = gy % pieceSize;
        Piece& piece = pieces[pi];
        for (long long x = 0; x < pieceSize; ++x) {
            piece[y][x] = l[x] == '#';
        }
    }

    return pieces;
}

std::vector<long long> gatherHeights(const Pieces& pieces) {
    std::vector<long long> heights;
    for(const Piece& p: pieces) {
        for (long long y = 0; y <= p.size(); ++y) {
            if (y == p.size() || std::none_of(begin(p[y]), end(p[y]), [](bool b) { return b; })) {
                heights.push_back(y);
                break;
            }
        }
    }

    return heights;
}

struct Board {
    bool at(long long x, long long y) const { 
        return board.count(PointL {x, y}) > 0;
    }

    void add(const Piece& piece, const PointL& position) {
        for (long long y = 0; y < pieceSize; ++y) {
            for (long long x = 0; x < pieceSize; ++x) {                
                if ( piece[y][x]) {
                    const long long bx = position.x + x;
                    const long long by = position.y - y;
                    board.insert(PointL {bx, by});
                }
            }
        }

        height = std::max(height, position.y);        
    }

    std::unordered_set<PointL> board;
    long long width = 7;
    long long height = -1;
};

long long collapse(const Board& board) {
    for (long long y = board.height; y >= 0; --y) {
        for (long long x = 0; x < board.width; ++x) {
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

void print(const Board& board, const Piece& piece, const PointL& position) {
    const auto hasPiece = [&piece, &position] (const long long x, const long long y) -> bool {
        if (x >= position.x && x < position.x + 4 && y <= position.y && y > position.y - 4) {
            return piece[position.y - y][x - position.x];
        }
        return false;
    };
    for (long long y = board.height; y >= 0; --y) {
        printf("%3lli |", y);
        for (long long x = 0; x < board.width; ++x) {
            const char c = board.at(x, y) ? '%' : hasPiece(x,y) ? 'o' : '.';
            printf("%c", c);
        }
        printf("|\n");
    }
    printf("    +-------+\n");
}

bool willFit(const Board& board, const Piece& piece, const PointL& position) {
    for (long long y = 0; y < pieceSize; ++y) {
        for (long long x = 0; x < pieceSize; ++x) {
            const long long bx = position.x + x;
            const long long by = position.y - y;
            if (piece[y][x] && (bx < 0 || bx >= board.width || by < 0 || board.at(bx, by))) {
                return false;
            }
        }
    }
    return true;
}

long long fall(Board& board, const Piece& piece, PointL position) {
    do {
        if (willFit(board, piece, PointL{ position.x, position.y - 1})) {
            position.y -= 1;
        } else {
            board.add(piece, position);
            return position.y;
        }
    } while (true);
    return -1;
}

State calculateState(long long inputIndex, long long pieceIndex, const Board& board) {
    State state;

    state[0] = inputIndex;
    state[1] = pieceIndex;

    long long i = 2;
    for (long long y = board.height; y > board.height - stateLength; --y) {
        state[i] = board.at(0, y) << 6 | board.at(1, y)  << 5 | board.at(2, y) << 4 | board.at(3, y) << 3 | board.at(4, y) << 2 | board.at(5, y) << 1 | board.at(6, y) << 0;
        ++i;
    }

    return state;
}

long long simulate(const Pieces& pieces, const std::vector<long long>& heights, const std::string& input, long long count) {
    Board board;

    Cache cache;

    long long offset = 0;
    long long inputIndex = 0;
    for (long long i = 0; i < count; ++i) {
        const State state = calculateState(inputIndex % input.size(), i % pieces.size(), board);
        const auto it = cache.find(state);
        if (it != end(cache)) {
            printf("Cycle detected %lli %lli -> %lli %lli\n", i, board.height, it->second.i, it->second.h);
            const long long deltaI = i - it->second.i;
            const long long deltaH = board.height - it->second.h;
            const long long rem = count - i;
            const long long rep = rem / deltaI;
            offset = rep * deltaH;
            i += rep * deltaI;
            printf("deltaI %lli delta H %lli rem %lli rep %lli offset %lli new i %lli\n",deltaI, deltaH, rem, rep, offset, i + rep*deltaI);
            cache.clear();
        }
        printf("Normal spawn %lli %lli\n", i, board.height);
        cache[state] = {.i = i, .h = board.height};


        board.height += 3 + heights[i % heights.size()];

        const Piece& piece = pieces[i % pieces.size()];
        PointL position {2, board.height};

        if (i % 100000 == 0)
        printf("%lli\n", i);

        do {
            const long long horizontalMove = input[inputIndex++ % input.size()] == '<' ? -1 : 1;
            if (willFit(board, piece, PointL {position.x + horizontalMove, position.y})) {
                position.x += horizontalMove;
            }

            if (willFit(board, piece, PointL{ position.x, position.y - 1})) {
                position.y -= 1;
            } else {
                board.add(piece, position);
                break;
            }
        } while (true);
        board.height = collapse(board);
        // print(board, piece, position);
    }
    return board.height + offset + 1;    // I count from 0
}

void part1(const Pieces& pieces, const std::vector<long long>& heights, const std::string& input) {
    const auto height = simulate(pieces, heights, input, 2022);
    printf("Part 1: %lli\n", height);
}
void part2(const Pieces& pieces, const std::vector<long long>& heights, const std::string& input) {
    const auto height = simulate(pieces, heights, input, 1000000000000);
    printf("Part 2: %lli\n", height);
}

void solve(const char* filename) {
    const std::string input = readFile(filename)[0];
    const Pieces pieces = createPieces();
    const std::vector<long long> heights = gatherHeights(pieces);

    part1(pieces, heights, input);
    part2(pieces, heights, input);
}

void test() {

}

int main(int argc, char* argv[])
{
    test();

    const char* filename = argc > 1 ? argv[1] : "day-17.sample";
    solve(filename);
}