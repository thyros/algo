#include "utils.h"
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>

enum class Shape {
    Rock = 0,
    Paper = 1,
    Scissors = 2
};

enum class Result {
    Lose = 0,
    Draw = 1,
    Win = 2
};

struct Round {
    Shape opponent;
    Shape you;
};

struct SetRound {
    Shape opponent;
    Result result;
};

Round makeRound(std::string_view line) {
    char opponent = line[0];
    char you = line[2];

    return {static_cast<Shape>(opponent - 'A'), static_cast<Shape>(you - 'X')};
}

SetRound makeSetRound(std::string_view line) {
    char opponent = line[0];
    char you = line[2];

    return {static_cast<Shape>(opponent - 'A'), static_cast<Result>(you - 'X')};
}

Shape makeMove(Shape opponent, Result desiredResult) {
    if (desiredResult == Result::Draw) {
        return opponent;
    }
    if (desiredResult == Result::Lose) {
        return static_cast<Shape>((static_cast<int>(opponent) + 2) % 3);
    }
    return static_cast<Shape>((static_cast<int>(opponent) + 1) % 3);
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
    return static_cast<int>(you) + 1;
}

using Rounds = std::vector<Round>;
Rounds toRounds(const Lines& lines) {
    Rounds rounds;
    rounds.reserve(lines.size());

    std::transform(begin(lines), end(lines), std::back_inserter(rounds), makeRound);

    return rounds;
}


using SetRounds = std::vector<SetRound>;
SetRounds toSetRounds(const Lines& lines) {
    SetRounds setRounds;
    setRounds.reserve(lines.size());

    std::transform(begin(lines), end(lines), std::back_inserter(setRounds), makeSetRound);
    return setRounds;
}


long solvePart1(const Lines& lines) {
    const Rounds rounds = toRounds(lines);

    long result = 0;
    for (const auto &[opponent, you]: rounds) {
        int roundValue = rateRound(opponent, you);
        int shapeValue = rateShape(you);
        result += roundValue + shapeValue;
    }

    return result;
}

long solvePart2(const Lines& lines) {
    const SetRounds setRounds = toSetRounds(lines);

    long result = 0;
    for (const auto &[opponent, roundResult]: setRounds) {
        Shape you = makeMove(opponent, roundResult);

        int roundValue = rateRound(opponent, you);
        int shapeValue = rateShape(you);

        result += roundValue + shapeValue;
    }
    return result;    
}

long solve(const char* filename, std::function<long(Lines)> solveFunction) {
    const Lines lines = readFile(filename);

    return solveFunction(lines);
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

    // losing move
    assert(Shape::Scissors == makeMove(Shape::Rock, Result::Lose));
    assert(Shape::Rock == makeMove(Shape::Paper, Result::Lose));
    assert(Shape::Paper == makeMove(Shape::Scissors, Result::Lose));
    // drawing move
    assert(Shape::Rock == makeMove(Shape::Rock, Result::Draw));
    assert(Shape::Paper == makeMove(Shape::Paper, Result::Draw));
    assert(Shape::Scissors == makeMove(Shape::Scissors, Result::Draw));
    // winning move
    assert(Shape::Paper == makeMove(Shape::Rock, Result::Win));
    assert(Shape::Scissors == makeMove(Shape::Paper, Result::Win));
    assert(Shape::Rock == makeMove(Shape::Scissors, Result::Win));
}

int main() {
    test();
    std::cout << "Result: " << solve("day02.input", solvePart2);
}