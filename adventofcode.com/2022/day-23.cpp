#include "point.h"
#include "utils.h"
#include "ut.hpp"
#include <array>
#include <unordered_map>

const Point neighbors[8] = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};
const Point scan[4][3] = {
    {{-1, -1}, {0, -1}, {1, -1}}, {{-1, 1}, {0, 1}, {1, 1}}, {{-1, -1}, {-1, 0}, {-1, 1}}, {{1, -1}, {1, 0}, {1, 1}}};

using Elves = std::unordered_map<int, Point>;

struct ProposedMove {
    int elf;
    Point dest;
};

ProposedMove proposeMove(const Elves& elves, int elf, const Point& position) {

}

void solve(const char* filename)
{
    const Lines& lines = readFile(filename);
}

void test() {
    using namespace boost::ut;

    const Elves elves;
    const int elf;
    const Point position {5,5};

    expect(proposeMove(elves, elf, position) == ProposedMove{1, {0,0}});
}

int main(int argc, char* argv[])
{
    test();

    const char* filename = argc > 1 ? argv[1] : "day-23.sample";
    solve(filename);
}