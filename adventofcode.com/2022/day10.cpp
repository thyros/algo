#include "utils.h"
#include <cassert>
#include <cstdio>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>


void part1(const Lines& lines) {
    std::vector<int> specialCycles {20, 60, 100, 140, 180, 220};

    int result = 0;
    int reg = 1;
    int cycle = 0;
    for (const std::string& command: lines) {
        int value {0};

        if (command == "noop") {
            cycle += 1;
        } else if (sscanf(command.c_str(), "addx %i", &value)) {
            cycle += 2;
        }

        // printf("\t%s %i %i\n", command.c_str(), cycle, reg);

        if (cycle >= specialCycles[0]) {
            int cycleValue = specialCycles[0] * reg;
            result += cycleValue;
            // printf("%i,%i -> %i = %i\n", specialCycles[0], reg, cycleValue, result);
            specialCycles.erase(begin(specialCycles));
        }

        reg += value;

        if (specialCycles.empty()) {
            break;
        }
    }

    printf("Part 1: %i\n", result);

}

void part2(const Lines& lines) {
    const auto drawPixel = [](int spritePosition, int pixel) {
        // sprite position is 1 based
        // pixel is 0 based
        ++pixel;

        const char c = (pixel >= spritePosition && pixel <= spritePosition + 2) ? 'X' : ' ';
        printf("%c", c);
        if (pixel == 40) {
            printf("\n");
        }
    };

    int reg = 1;
    int cycle = 0;

    for (const std::string& command: lines) {
        int value {0};
        if (command == "noop") {
            drawPixel(reg, cycle % 40);
            cycle += 1;
        } else if (sscanf(command.c_str(), "addx %i", &value)) {
            drawPixel(reg, cycle % 40);
            cycle += 1;
            drawPixel(reg, cycle % 40);
            cycle += 1;
            reg += value;
        }
    }
}

void solve(const char* filename) {
    const Lines lines = readFile(filename);

    part1(lines);
    part2(lines);
}

int main() {
    constexpr char filename[] { "day10.input" };
    solve(filename);
}