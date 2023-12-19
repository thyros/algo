#include "utils.h"
#include <array>
#include <cstdio>
#include <iostream>
#include <string>
#include <string_view>
#include <stack>

struct Command {
    int count;
    int from;
    int to;
};

using Stack = std::stack<char>;
using Stacks = std::array<Stack, 9>;
using Commands = std::vector<Command>;

struct Task {
    Stacks stacks;
    Commands commands;
};

Command toCommand(const std::string& line) {
    int count = 0;
    int from = 0;
    int to = 0;

    sscanf(line.c_str(), "move %d from %d to %d", &count, &from, &to);
    return {.count = count, .from = from, .to = to};
}

Task parseInput(const Lines& lines) {
    Stacks stacks;
    Commands commands;
    for (const std::string& line: lines) {
        const std::string_view sv { line };

        if (sv.starts_with("move")) {
            commands.push_back(toCommand(line));
        }
        else {
            for (size_t i = 0; i*4+1 < line.length(); ++i) {
                const size_t offset = i * 4 + 1;
                const char c = line[offset];

                if (c >= 'A' && c <= 'Z') {
                    stacks[i].push(c);
                }
            }
        }
    }
    Stacks reversed;
    for (size_t i = 0; i < stacks.size(); ++i) {
        while (!stacks[i].empty()) {
            reversed[i].push(stacks[i].top());
            stacks[i].pop();
        }
    }

    return {.stacks = std::move(reversed), .commands = std::move(commands)};
}

void move9000(Stack& from, Stack& to, int count) {
    for (size_t i = 0; i < count; ++i) {
        to.push(from.top());
        from.pop();
    }
}

void move9001(Stack& from, Stack& to, int count) {
    Stack temp;
    for (size_t i = 0; i < count; ++i) {
        temp.push(from.top());
        from.pop();
    }
    for (size_t i = 0; i < count; ++i) {
        to.push(temp.top());
        temp.pop();
    }
}

template<typename Move>
std::string solve(const char* filename, Move&& move) {
    const Lines lines = readFile(filename);
    auto [stacks, commands] = parseInput(lines);

    for (const Command& command: commands) {
        move(stacks[command.from - 1], stacks[command.to - 1], command.count);
    }

    std::string result;
    result.reserve(stacks.size());
    for (auto stack: stacks) {
        if (!stack.empty()) {
            result.push_back(stack.top());
        }
    };

    return result;
}

int main() {
    std::cout << "Result: " << solve("day05.input", move9001) << std::endl;
}