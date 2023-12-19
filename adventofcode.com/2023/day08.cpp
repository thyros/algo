#include "utils.h"
#include "ut.hpp"
#include <algorithm>
#include <array>
#include <format>
#include <numeric>
#include <iostream>
#include <unordered_map>

using ULL = unsigned long long;

using Network = std::unordered_map<std::string, std::array<std::string, 2>>;
struct PuzzleInput {
    std::string instructions;

    Network network;
};

PuzzleInput parse(const Lines& lines) {

    Network network;
    for (size_t i = 2; i < lines.size(); ++i) {
        const std::string& l = lines[i];
        network[l.substr(0, 3)] = {l.substr(7, 3), l.substr(12, 3)};
    }

    return PuzzleInput {.instructions = lines[0], .network = {std::move(network)}};
}

void solvePart1(const PuzzleInput& puzzleInput) {
    int step = 0;
    std::string currentNode = "AAA";
    while (currentNode != "ZZZ") {
        const char direction = puzzleInput.instructions[step++ % puzzleInput.instructions.size()];
        const int offset = direction == 'L' ? 0 : 1;

        currentNode = puzzleInput.network.at(currentNode)[offset];
    }
    std::cout << std::format("Part 1: It took {} steps to reach ZZZ\n", step);
}

void solvePart2(const PuzzleInput& puzzleInput) {
    std::vector<std::string> currentNodes;
    for (const auto& node: puzzleInput.network) {
        if (node.first[2] == 'A') {
            currentNodes.push_back(node.first);
        }
    }
    std::cout << std::format("there are {} starting nodes\n", currentNodes.size());

    // there are no multiple Z's in a cycle.
    // number to get to the first Z is the same as the length of the cycle
    std::vector<ULL> lengths;
    for (std::string currentNode: currentNodes) {
        int step = 0;
        while (currentNode[2] != 'Z') { 
            const char direction = puzzleInput.instructions[step++ % puzzleInput.instructions.size()];
            const int offset = direction == 'L' ? 0 : 1;            

            currentNode = puzzleInput.network.at(currentNode)[offset];
        }
        std::cout << std::format("\tIt took {} steps to reach {} exit\n", step, currentNode);
        lengths.push_back(step);
    }

    ULL lcm = lengths.back();
    lengths.pop_back();
    for (ULL n: lengths) {
        lcm = lcm * n / std::gcd(lcm, n);
    }

    std::cout << std::format("It took {} for all ghost to reach nodes ending with 'Z'\n", lcm);
}

void solve(const char* filename) {
    const Lines& lines = readFile(filename);
    const PuzzleInput puzzleInput = parse(lines);

    // solvePart1(puzzleInput);
    solvePart2(puzzleInput);
}

void test() {
    using namespace boost::ut;

    ""_test = [] {
        
    };
}

int main(int argc, const char** argv) {
    test();
    const char* filename = argc > 1 ? argv[1] : "day08.input";
    solve(filename);
}