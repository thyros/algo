#include "utils.h"
#include <algorithm>
#include <iostream>
#include <numeric>

using Calories = std::vector<long>;
Calories toCalories(const Lines& lines) {
    Calories calories;
    calories.emplace_back(0);

    for (const std::string& line: lines) {
        if (line.empty()) {
            calories.emplace_back(0);
        } else {
            calories.back() += std::stoi(line.c_str());
        }
    }
    return calories;
}

long findMax(Calories calories, int count) {
    const auto from = begin(calories);
    const auto middle = from + count;
    const auto last = end(calories);
    std::partial_sort(from, middle, last, std::greater{});

    return std::accumulate(from, middle, 0);
}


long solve(const char* filename, int count) {
    std::ifstream file{filename};
    const Lines lines = readLines(file);
    const std::vector<long> calories = toCalories(lines);

    return findMax(calories, count);
}

int main(int argc, char *argv[]) {
    const auto max_calories = solve("day-01.input", 3);
    std::cout << "Result: " << max_calories << std::endl;
}