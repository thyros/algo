#include "utils.h"
#include "ut.hpp"

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
    const char* filename = argc > 1 ? argv[1] : "day11.sample";
    solve(filename);
}