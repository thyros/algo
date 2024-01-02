#include "utils.h"
#include "ut.hpp"
#include <format>
#include <unordered_map>

using T = long long;

void solve(const char* filename) {
    const Lines& lines = readFile(filename);
}

void test() {
    using namespace boost::ut;

    ""_test = [] {
    };
}

int main(int argc, const char** argv) {
    test();
    const char* filename = argc > 1 ? argv[1] : "day17.sample";
    solve(filename);
}