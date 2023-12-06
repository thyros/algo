#include "utils.h"
#include "ut.hpp"
#include <string_view>

int parse(std::string_view text) {

    return 0;
}

void solve(const char* filename) {

}

void test() {
    using namespace boost::ut;

    ""_test = [] {
        int game = parse("Game 13: 1 blue, 16 green, 1 red; 6 red, 2 blue, 5 green; 2 blue, 12 red, 10 green; 3 red, 4 blue, 13 green; 14 red, 4 blue, 12 green; 7 red, 2 green");
        expect(13_i == game);
    };    
}

int main(int argc, const char** argv) {
    test();
    const char* filename = argc > 1 ? argv[1] : "day-02.sample";
    solve(filename);
}