#include "pointL.h"
#include "utils.h"
#include "ut.hpp"
#include <algorithm>
#include <deque>
#include <format>
#include <functional>
#include <unordered_set>

using T = long double;
struct Hailstone {
    Hailstone(T x_, T y_, T z_, T vx_, T vy_, T vz_) :
        x(x_), y(y_), z(z_), vx(vx_), vy(vy_), vz(vz_), a(vy_), b(-vx_), c(vy_ * x_ - vx_ * y) {}
    T x, y, z;
    T vx, vy, vz;
    T a, b, c;
};
using Hailstones = std::vector<Hailstone>;

Hailstone parse(const std::string& line) {
    long long x, y, z;
    long long vx, vy, vz;
    const T parsed = sscanf(line.c_str(), "%I64d, %I64d, %I64d @ %I64d, %I64d, %I64d", &x, &y, &z, &vx, &vy, &vz);
    assert(parsed == 6);
    return Hailstone{static_cast<T>(x),
                     static_cast<T>(y),
                     static_cast<T>(z),
                     static_cast<T>(vx),
                     static_cast<T>(vy),
                     static_cast<T>(vz)};
}

Hailstones parse(const Lines& lines) {
    Hailstones hss;
    std::transform(begin(lines), end(lines), std::back_inserter(hss), [](const std::string& line) { return parse(line); });
    return hss;
}

void solvePart1(const Hailstones& hss, T min, T max) {
    T total = 0;
    for (int i = 0; i < hss.size(); ++i) {
        for (int j = 0; j < i; ++j) {
            const Hailstone& hs1 = hss[i];
            const Hailstone& hs2 = hss[j];

            const T a1 = hs1.a;
            const T b1 = hs1.b;
            const T c1 = hs1.c;
            const T a2 = hs2.a;
            const T b2 = hs2.b;
            const T c2 = hs2.c;

            if (a1 * b2 == a2 * b1) { 
                // parallel
                continue;
            }

            const T x = (c1 * b2 -  c2 * b1) / (a1*b2 - a2*b1);
            const T y = (c2 * a1 -  c1 * a2) / (a1*b2 - a2*b1);

            if (x >= min && x <= max && y >= min && y <= max) {
                if ((x - hs1.x) * hs1.vx >=0 && (y - hs1.y) * hs1.vy >=0 &&
                    (x - hs2.x) * hs2.vx >=0 && (y - hs2.y) * hs2.vy >=0) {
                    ++total;    
                }
            }
        }
    }

    std::cout << std::format("Part 1: colliding hailstones: {}\n", total);

}

void solve(const char* filename, T min, T max) {
    std::cout << std::format("Solving for {} {} {}\n", filename, min, max);
    const Lines& lines = readFile(filename);
    const Hailstones hss = parse(lines);
    solvePart1(hss, min, max);
}

void test() {
    using namespace boost::ut;

    ""_test = [] {
        const Hailstone hs = parse("294075397467910, 234333313677607, 197238255273352 @ 68, -31, 190");
        expect(hs.x == 294075397467910);
        expect(hs.y == 234333313677607);
        expect(hs.z == 197238255273352);
        expect(hs.vx == 68);
        expect(hs.vy == -31);
        expect(hs.vz == 190);        
    };
}

int main(int argc, const char** argv) {

    std::cout << std::numeric_limits<T>::max() << std::endl;
    std::cout << std::numeric_limits<T>::min() << std::endl;

    test();
    const char* filename = argc > 1 ? argv[1] : "day24.input";
    T min = 7;
    T max = 27;
    if (argc == 4) {
        min = std::stoll(argv[2]);
        max = std::stoll(argv[3]);
    }
    solve(filename, min, max);
}