#include "utils.h"
#include <cassert>
#include <cstdio>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>


struct Monkey {
    std::vector<int> items;

    std::function<int(int)> op;
    int div;

    int trueMonkey;
    int falseMonkey;
};
using Monkeys = std::vector<Monkey>;

Monkeys toMonkeys(const Lines& lines) {
    Monkeys result;

    return result;
}

void solve(const char* filename) {
    const Lines lines = readFile(filename);


}

void test() {

}

int main() {
    test();

    constexpr char filename[] { "day-11.sample" };
    solve(filename);
}