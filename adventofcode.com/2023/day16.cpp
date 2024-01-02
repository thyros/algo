#include "utils.h"
#include "ut.hpp"
#include "point.h"
#include <format>
#include <iterator>
#include <unordered_set>

using T = long long;
using Grid = Lines;

enum class Dir {
    U, D, R, L
};
static const std::unordered_map<Dir, char> dirC {std::make_pair(Dir::U, 'U'), std::make_pair(Dir::D, 'D'), std::make_pair(Dir::L, 'L'), std::make_pair(Dir::R, 'R')};

struct Beam {
    Point p;
    Dir d;

    auto operator<=>(const Beam& rhs) const = default;
};
using Beams = std::vector<Beam>;

std::ostream & operator<< (std::ostream& out, const Beam& b) {
    out << std::format("({},{}:{})", b.p.x, b.p.y, dirC.at(b.d));
    return out;
}

std::ostream & operator<< (std::ostream& out, const Beams& bs) {
    out << "(";
    for (const Beam& b: bs) {
        out << b;
    }
    out << ")";
    return out;
}

class BeamHasher {
public:
  std::size_t operator()(const Beam& beam) const {
    std::size_t ret = 0;
    std::hash<long long> hasher;
    ret = rotl(ret, 11)^hasher(beam.p.x);
    ret = rotl(ret, 11)^hasher(beam.p.y);
    ret = rotl(ret, 11)^hasher(static_cast<int>(beam.d));
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


Point newPosition(Point p, Dir d) {
    if (d == Dir::U) {
        return {p.x, p.y - 1};
    }
    if (d == Dir::D) {
        return {p.x, p.y + 1};
    }
    if (d == Dir::L) {
        return {p.x - 1, p.y};
    }
    return {p.x + 1, p.y};
}

bool isOnGrid(const Grid& g, Point p) {
    return p.x >= 0 && p.y >= 0 && p.x < g[0].size() && p.y < g.size();
}

Beams simulateBeam(const Grid& g, Beam b) {
    if (!isOnGrid(g, b.p)) {
        return {};
    }

    const auto move = [p = b.p](Dir d){ return Beam {.p = newPosition(p, d), .d = d}; };
    
    const char c = g[b.p.y][b.p.x];
    if (c == '|') {
        if (b.d == Dir::R || b.d == Dir::L) {
            return {move(Dir::U), move(Dir::D)};
        }
    } else if (c == '-') {
        if (b.d == Dir::U || b.d == Dir::D) {
            return {move(Dir::L), move(Dir::R)};
        }
    } else if (c == '\\') {
        static const std::unordered_map<Dir, Dir> reflected = {{Dir::D, Dir::R}, {Dir::U, Dir::L}, {Dir::L, Dir::U}, {Dir::R, Dir::D}};
        return {move(reflected.at(b.d))};
    } else if (c == '/') {
        static const std::unordered_map<Dir, Dir> reflected = {{Dir::D, Dir::L}, {Dir::U, Dir::R}, {Dir::L, Dir::D}, {Dir::R, Dir::U}};
        return {move(reflected.at(b.d))};
    }

    return {move(b.d)};
}

T simulate(const Grid& g, Beam startingBeam) {
    std::unordered_set<Point> visitedCells;
    std::unordered_set<Beam, BeamHasher> loopDetector;

    visitedCells.insert(startingBeam.p);

    Beams beams {startingBeam};

    while (!beams.empty()) {
        const Beam b = beams[0];
        beams.erase(begin(beams));

        const Beams newBeams = simulateBeam(g, b);

        for (const Beam newBeam: newBeams) {
            if (!isOnGrid(g, newBeam.p)) {
                continue;
            }

            if (loopDetector.contains(newBeam)) {
                continue;
            }
            visitedCells.insert(newBeam.p);
            loopDetector.insert(newBeam);
            beams.push_back(newBeam);
        }        
    }

    return visitedCells.size();
}


void solvePart1(const Grid& g) {
    const Beam startingBeam {.p = {0, 0}, .d = Dir::R};
    T visitedCells = simulate(g, startingBeam);
    std::cout << std::format("Part 1: total visited cells: {}\n", visitedCells);
}

void solvePart2(const Grid& g) {
    T max = 0;

    for (int x = 0; x < g[0].size(); ++x) {
        const int y = g.size() - 1;
        const T v1 = simulate(g, Beam{.p = {x, 0}, .d = Dir::D});
        const T v2 = simulate(g, Beam{.p = {x, y}, .d = Dir::U});
        max = std::max({max, v1, v2});
    }

    for (int y = 0; y < g.size(); ++y) {
        const int x = g[0].size() - 1;
        const T v1 = simulate(g, Beam{.p = {0, y}, .d = Dir::R});
        const T v2 = simulate(g, Beam{.p = {x, y}, .d = Dir::L});
        max = std::max({max, v1, v2});
    }

    std::cout << std::format("Part 2: max: {}\n", max);
}

void solve(const char* filename) {
    const Lines& lines = readFile(filename);

    solvePart1(lines);
    solvePart2(lines);
}

void test() {
    using namespace boost::ut;

    "Moving through the empty space does not affect the beam"_test = [] {
        const Grid g {"..", 
                      ".."};

        const std::vector<Beam> r = simulateBeam(g, Beam {.p = Point{0, 0}, .d = Dir::R });
        expect(1_i == r.size());
        expect(Beams {{.p = {1, 0}, .d = Dir::R}} == r);

        const std::vector<Beam> l = simulateBeam(g, Beam {.p = Point{1, 0}, .d = Dir::L });
        expect(1_i == l.size());
        expect(Beams {{.p = {0, 0}, .d = Dir::L}} == l);

        const std::vector<Beam> u = simulateBeam(g, Beam {.p = Point{1, 1}, .d = Dir::U });
        expect(1_i == u.size());
        expect(Beams {{.p = {1, 0}, .d = Dir::U}} == u);

        const std::vector<Beam> d = simulateBeam(g, Beam {.p = Point{1, 0}, .d = Dir::D });
        expect(1_i == d.size());
        expect(Beams {{.p = {1, 1}, .d = Dir::D}} == d);
    };

    "Moving outside the grid stops the beam"_test = [] {
        const Grid g {"..", 
                      ".."};

        expect(Beams {} == simulateBeam(g, Beam {.p = Point(-1, 0), .d = Dir::R }));
        expect(Beams {} == simulateBeam(g, Beam {.p = Point( 2, 1), .d = Dir::L }));
        expect(Beams {} == simulateBeam(g, Beam {.p = Point( 1,-1), .d = Dir::D }));
        expect(Beams {} == simulateBeam(g, Beam {.p = Point( 1, 2), .d = Dir::U }));
    };

    "Moving horizontally through the horizontal mirror does not affect the beam"_test = [] {
        const Grid g {"-.", 
                      ".-"};

        const std::vector<Beam> r = simulateBeam(g, Beam {.p = Point{0, 0}, .d = Dir::R });
        expect(Beams {{.p = {1, 0}, .d = Dir::R}} == r);

        const std::vector<Beam> l = simulateBeam(g, Beam {.p = Point{1, 1}, .d = Dir::L });
        expect(Beams {{.p = {0, 1}, .d = Dir::L}} == l);
    };

    "Moving horizontally through the vertical mirror splits it"_test = [] {
        const Grid g {"|.", 
                      ".|"};

        const std::vector<Beam> r = simulateBeam(g, Beam {.p = Point{0, 0}, .d = Dir::R });
        expect(Beams {{.p = {0, -1}, .d = Dir::U}, {.p = {0, 1}, .d = Dir::D}} == r);

        const std::vector<Beam> l = simulateBeam(g, Beam {.p = Point{1, 1}, .d = Dir::L });
        expect(Beams {{.p = {1, 0}, .d = Dir::U}, {.p = {1, 2}, .d = Dir::D}} == l);
    };


    "Moving vertically through the vertical mirror does not affect the beam"_test = [] {
        const Grid g {"|.", 
                      ".|"};

        const std::vector<Beam> d = simulateBeam(g, Beam {.p = Point{0, 0}, .d = Dir::D });
        expect(Beams {{.p = {0, 1}, .d = Dir::D}} == d);

        const std::vector<Beam> u = simulateBeam(g, Beam {.p = Point{1, 1}, .d = Dir::U });
        expect(Beams {{.p = {1, 0}, .d = Dir::U}} == u);
    };

    "Moving vertically through the horizontal mirror splits it"_test = [] {
        const Grid g {"-.", 
                      ".-"};

        const std::vector<Beam> d = simulateBeam(g, Beam {.p = Point{1, 1}, .d = Dir::D });
        expect(Beams {{.p = {0, 1}, .d = Dir::L}, {.p = {2, 1}, .d = Dir::R}} == d);

        const std::vector<Beam> u = simulateBeam(g, Beam {.p = Point{0, 0}, .d = Dir::U });
        expect(Beams {{.p = {-1, 0}, .d = Dir::L}, {.p = {1, 0}, .d = Dir::R}} == u);
    };

    "Moving through diagonal mirror reflects the beam"_test = [] {
        const Grid g {"/-\\",
                      "...",
                      "\\./" };

        // going up
        expect(Beams {{.p = {1, 0}, .d = Dir::R}} == simulateBeam(g, Beam {.p = Point{0, 0}, .d = Dir::U }));
        expect(Beams {{.p = {1, 0}, .d = Dir::L}} == simulateBeam(g, Beam {.p = Point{2, 0}, .d = Dir::U }));
        // going down
        expect(Beams {{.p = {1, 2}, .d = Dir::R}} == simulateBeam(g, Beam {.p = Point{0, 2}, .d = Dir::D }));
        expect(Beams {{.p = {1, 2}, .d = Dir::L}} == simulateBeam(g, Beam {.p = Point{2, 2}, .d = Dir::D }));
        // going left
        expect(Beams {{.p = {0, 1}, .d = Dir::D}} == simulateBeam(g, Beam {.p = Point{0, 0}, .d = Dir::L }));
        expect(Beams {{.p = {0, 1}, .d = Dir::U}} == simulateBeam(g, Beam {.p = Point{0, 2}, .d = Dir::L }));
        // going right
        expect(Beams {{.p = {2, 1}, .d = Dir::D}} == simulateBeam(g, Beam {.p = Point{2, 0}, .d = Dir::R }));
        expect(Beams {{.p = {2, 1}, .d = Dir::U}} == simulateBeam(g, Beam {.p = Point{2, 2}, .d = Dir::R }));
    };
}

int main(int argc, const char** argv) {
    test();
    const char* filename = argc > 1 ? argv[1] : "day16.sample";
    solve(filename);
}