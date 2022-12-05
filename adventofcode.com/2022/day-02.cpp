#include "utils.h"
#include <algorithm>
#include <cassert>
#include <iostream>

enum class Shape {
    Rock = 1,
    Paper = 2,
    Scissors = 3
};

struct Round {
    Shape opponent;
    Shape you;
};

Round makeRound(std::string_view line) {
    char opponent = line[0];
    char you = line[2];

    return {static_cast<Shape>(opponent - 'A' + 1), static_cast<Shape>(you - 'X' + 1)};
}

int rateRound(Shape opponent, Shape you) {
    // win
    if ((you == Shape::Rock && opponent == Shape::Scissors) ||
        (you == Shape::Scissors && opponent == Shape::Paper) ||
        (you == Shape::Paper && opponent == Shape::Rock)) {
            return 6;
        }
    // draw
    if (opponent == you) {
        return 3;
    }
    return 0;
}

int rateShape(Shape you) {
    return static_cast<int>(you);
}

using Rounds = std::vector<Round>;
Rounds toRounds(const Lines& lines) {
    Rounds rounds;
    rounds.reserve(lines.size());

    std::transform(begin(lines), end(lines), std::back_inserter(rounds), makeRound);

    return rounds;
}


long solve(const char* filename) {
    std::ifstream file(filename);
    const Lines lines = readLines(file);
    const Rounds rounds = toRounds(lines);

    long result = 0;
    for (const auto &[opponent, you]: rounds) {
        int roundValue = rateRound(opponent, you);
        int shapeValue = rateShape(you);

        result += roundValue + shapeValue;
    }

    return result;
}

void test() {
    // lost
    assert(0 == rateRound(Shape::Rock, Shape::Scissors));
    assert(0 == rateRound(Shape::Paper, Shape::Rock));
    assert(0 == rateRound(Shape::Scissors, Shape::Paper));
    // draw
    assert(3 == rateRound(Shape::Rock, Shape::Rock));
    assert(3 == rateRound(Shape::Paper, Shape::Paper));
    assert(3 == rateRound(Shape::Scissors, Shape::Scissors));
    // win
    assert(6 == rateRound(Shape::Rock, Shape::Paper));
    assert(6 == rateRound(Shape::Paper, Shape::Scissors));
    assert(6 == rateRound(Shape::Scissors, Shape::Rock));
}

int main() {
    test();
    std::cout << "Result: " << solve("day-02.input");
}