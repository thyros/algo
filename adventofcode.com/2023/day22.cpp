#include "utils.h"
#include "ut.hpp"
#include <algorithm>
#include <deque>
#include <format>
#include <functional>
#include <unordered_set>

using T = long long;

struct Brick {
    int x1, y1, z1;
    int x2, y2, z2;
};
using Bricks = std::vector<Brick>;
using BrickToOthers = std::unordered_map<int, std::unordered_set<int>>;

bool overlaps(const Brick& b1, const Brick& b2) {
    return std::max(b1.x1, b2.x1) <= std::min(b1.x2, b2.x2) &&
           std::max(b1.y1, b2.y1) <= std::min(b1.y2, b2.y2); 
}

Brick parse(const std::string& line) {
    int x1, y1, z1, x2, y2, z2;
    sscanf(line.c_str(), "%d,%d,%d~%d,%d,%d", &x1, &y1, &z1, &x2, &y2, &z2);
    return Brick{ x1, y1, z1, x2, y2, z2};
}

Bricks parse(const Lines& lines) {
    Bricks bricks;

    for (const std::string& line: lines) {
        bricks.push_back(parse(line));
    }

    return bricks;
}

Bricks settle(Bricks bricks) {
    const auto sort = [&bricks] {std::sort(begin(bricks), end(bricks), [](const Brick& lhs, const Brick& rhs){ return lhs.z1 < rhs.z1; });};

    sort();
    for (int i = 0; i < bricks.size(); ++i) {
        Brick& brick = bricks[i];
        int maxZ = 1;

        for (int y = 0; y < i; ++y) {
            Brick& previous = bricks[y];

            if (overlaps(brick, previous)) {
                maxZ = std::max(maxZ, previous.z2 + 1);
            }
        }
        brick.z2 -= brick.z1 - maxZ;
        brick.z1 = maxZ;
    }
    sort();

    return bricks;
}

void solvePart1(const Bricks& bricks, BrickToOthers supports, BrickToOthers supported) {
    T total = 0;
    for (int i = 0; i < bricks.size(); ++i) {
        const auto& s = supports[i];
        if (std::all_of(begin(s), end(s), [&supported](int s){ return supported[s].size() >= 2; })) {
            ++total;
        }
    }

    std::cout << std::format("Part 1: Bricks to desintegrate {}\n", total);
}

void solvePart2(const Bricks& bricks, BrickToOthers supports, BrickToOthers supported) {
    T total = 0;

    for (int i = 0; i < bricks.size(); ++i) {
        std::deque<int> chain;
        for (int j: supports[i]) {
            if (supported[j].size() == 1) {
                chain.push_back(j);
            }
        }

        std::unordered_set<int> falling {begin(chain), end(chain)};
        while (!chain.empty()) {
            const int j = chain.front();
            chain.pop_front();

            for (int k: supports[j]) {
                if (falling.contains(k)) {
                    continue;
                }
                const auto& s = supported[k];
                if (std::all_of(begin(s), end(s), [&falling](int l){ return falling.contains(l); })) {
                    chain.push_back(k);
                    falling.insert(k);
                }
            }
        }

        total += falling.size();
    }

    std::cout << std::format("Part 2: Total falling bricks {}\n", total);
}
void solve(const char* filename) {
    const Lines& lines = readFile(filename);
    
    const Bricks bricks = parse(lines);
    const Bricks settled = settle(bricks);

    BrickToOthers supports;
    BrickToOthers supported;

    for (int j = 0; j < settled.size(); ++j) {
        const Brick& upper = settled[j];
        for (int i = 0; i < j; ++i) {
            const Brick& lower = settled[i];

            if (overlaps(upper, lower) && upper.z1 == lower.z2 + 1) {
                supports[i].insert(j);
                supported[j].insert(i);
            }
        }
    }
    solvePart1(settled, supports, supported);
    solvePart2(settled, supports, supported);
}

void test() {
    using namespace boost::ut;

    ""_test = [] {
    };
}

int main(int argc, const char** argv) {
    test();
    const char* filename = argc > 1 ? argv[1] : "day22.sample";
    solve(filename);
}