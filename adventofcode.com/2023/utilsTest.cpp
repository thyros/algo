#include "utils.h"
#include <cassert>
#include <vector>
#include <iostream>

void tokenizeTest() {
    const auto tokens = tokenize("498,4 -> 498,6 -> 496,6", " -> ");

    for (auto token: tokens) {
        std::cout << '|' << token << '|' << std::endl;
    }

}

int main() {
    tokenizeTest();
}