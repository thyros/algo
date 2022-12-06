#include "utils.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <unordered_set>

int rateLetter(char c) {
    assert((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));

    if (c >= 'a' && c <= 'z') {
        return c - 'a' + 1;
    } else if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 27;
    }
    return 0;
}

char findDuplicates(std::string_view left, std::string_view right) {
    std::unordered_set<char> buffer;

    for (char c: left) {
        buffer.insert(c);
    }

    for (char c: right) {
        if (buffer.count(c) > 0) {
            return c;
        }
    }

    return '\0';
}

char findBadge(std::string_view rucksack1, std::string_view rucksack2, std::string_view rucksack3) {
    std::unordered_set<char> buffer1;
    std::unordered_set<char> buffer2;

    std::for_each(begin(rucksack1), end(rucksack1), [&buffer1](char c){ buffer1.insert(c); });
    std::for_each(begin(rucksack2), end(rucksack2), [&buffer2](char c){ buffer2.insert(c); });

    for (char c: rucksack3) {
        if (buffer1.count(c) > 0 && buffer2.count(c) > 0) {
            return c;
        }
    }

    return '\0';
}

int solvePart1(const char* filename) {
    Lines lines = readFile(filename);

    int sum = 0;
    for (const std::string& line: lines) {
        const std::string_view sv = line;
        const size_t length = sv.length();
        const size_t mid = length / 2;

        const char duplicate = findDuplicates(sv.substr(0, mid), sv.substr(mid));
        sum += rateLetter(duplicate);
    }

    return sum;
}

int solvePart2(const char* filename) {
    Lines lines = readFile(filename);

    int sum = 0;
    for (size_t i = 0; i < lines.size(); i += 3) {
        const char badge = findBadge(lines[i], lines[i+1], lines[i+2]);
        sum += rateLetter(badge);
    }
    return sum;
}

int main() {
    std::cout << "Result: " << solvePart2("day-03.input") << std::endl;
}
