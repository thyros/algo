#include "utils.h"
#include "ut.hpp"
#include <format>
#include <functional>
#include <unordered_map>

const char Ball = 'O';
const char Stone = '#';
using T = long long;
enum class Tile {
    empty,
    ball,
    rock
};
struct PuzzleInput {
    T width;
    T height;
    std::vector<std::vector<Tile>> tiles;
};

T count(const Lines& lines) {
    T sum = 0;
    const T height = lines.size();

    for (T y = 0; y < height; ++y) {
        for (const char c: lines[y]) {
            if (c == Ball) {
                sum += height - y;
            }
        }
    }

    return sum;
}

void tiltNorth(Lines& lines) {
    const T width = lines[0].size();
    const T height = lines.size();
    for(T x = 0; x < width; ++x) {        
        T maxY = 0;
        for (T y = 0; y < height; ++y) {
            char& from = lines[y][x];
            char& to = lines[maxY][x];            
            if (from == Ball) {
                std::swap(from, to);
                ++maxY;
            } else if (from == Stone) {
                maxY = y + 1;
            }
        }
    }
}

void tiltWest(Lines& lines) {
    const T width = lines[0].size();
    const T height = lines.size();
    for(T y = 0; y < height; ++y) {        
        T maxX = 0;
        for (T x = 0; x < width; ++x) {
            char& from = lines[y][x];
            char& to = lines[y][maxX];            
            if (from == Ball) {
                std::swap(from, to);
                ++maxX;
            } else if (from == Stone) {
                maxX = x + 1;
            }
        }
    }
}

void tiltSouth(Lines& lines) {
    const T width = lines[0].size();
    const T height = lines.size();
    for(T x = 0; x < width; ++x) {        
        T maxY = height - 1;
        for (T y = height - 1; y >= 0; --y) {
            char& from = lines[y][x];
            char& to = lines[maxY][x];            
            if (from == Ball) {
                std::swap(from, to);
                --maxY;
            } else if (from == Stone) {
                maxY = y - 1;
            }
        }
    }
}

void tiltEast(Lines& lines) {
    const T width = lines[0].size();
    const T height = lines.size();
    for(T y = 0; y < height; ++y) {        
        T maxX = width - 1;
        for (T x = width - 1; x >= 0 ; --x) {
            char& from = lines[y][x];
            char& to = lines[y][maxX];            
            if (from == Ball) {
                std::swap(from, to);
                --maxX;
            } else if (from == Stone) {
                maxX = x - 1;
            }
        }
    }
}

class LinesHasher {
public:
  std::size_t operator()(const Lines& lines) const {
    std::size_t ret = 0;
    for (const std::string& i : lines) {
        std::hash<std::string> hasher;   
        ret = rotl(ret,11)^hasher(i);
    }
    return ret;
  }

private:
    // if c++20 rotl is not available:
    template <typename T,typename S>
    typename std::enable_if<std::is_unsigned<T>::value,T>::type
    constexpr rotl(const T n, const S i) const{
        const T m = (std::numeric_limits<T>::digits-1);
        const T c = i&m;
        return (n<<c)|(n>>((T(0)-c)&m)); // this is usually recognized by the compiler to mean rotation, also c++20 now gives us rotl directly
    }
};

void print(const Lines& lines) {
    std::copy(begin(lines), end(lines), std::ostream_iterator<std::string>(std::cout, "\n"));
    std::cout << "\n";
}

void solvePart1(Lines lines) {
    tiltNorth(lines);

    const T sum = count(lines);
    std::cout << std::format("Total load is {}\n", sum);
}
void solvePart2(Lines lines, T reps) {
    const LinesHasher hasher;

    std::unordered_map<size_t, T> seenHashes;
    bool ffed = false;
    for (T i = 0; i < reps; ++i) {
        tiltNorth(lines);
        tiltWest(lines);
        tiltSouth(lines);
        tiltEast(lines);

        const size_t hash = hasher(lines);
        const auto r = seenHashes.emplace(std::make_pair(hash, i));
        const bool seen = r.second == false;
        const T index = seen ? r.first->second : -1;

        std::cout << std::format("Board after {} cycle: hash {}, seen {} at {}\n", i, hasher(lines), seen, index);
        if (seen && !ffed) {
            const T cycleLength = i - index;
            const T ff = (reps - index) / cycleLength;
            const T nextI = i + (ff-1) * cycleLength;

            std::cout << std::format("cycle length {}, ff by {} cycles, {} -> {}\n", cycleLength, ff, i, nextI);

            i = nextI;
            ffed = true;
        }
    }

    const T sum = count(lines);
    std::cout << std::format("Total load is {}\n", sum);
}

void solve(const char* filename) {
    const Lines& lines = readFile(filename);
    solvePart1(lines);

    solvePart2(lines, 1000000000);
}

void test() {
    using namespace boost::ut;

    ""_test = [] {
    };
}

int main(int argc, const char** argv) {
    test();
    const char* filename = argc > 1 ? argv[1] : "day14.sample";
    solve(filename);
}