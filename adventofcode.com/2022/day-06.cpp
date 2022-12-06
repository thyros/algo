#include "utils.h"
#include <iostream>
#include <unordered_set>
#include <string_view>

bool unique(std::string_view marker) {
    std::unordered_set<char> buffer;
    for (char c: marker) {
        if (buffer.count(c) > 0) {
            return false;
        }
        buffer.insert(c);
    }
    return true;
}

int solve(const char* filename, int markerSize) {
    const Lines lines = readFile(filename);
    std::string_view line = lines[0];

    for (size_t i = 0; i < line.length() - markerSize; ++i) {
        if (unique(line.substr(i, markerSize))) {
            return i + markerSize;
        }
    }

    return 0;
}


int main() {
    std::cout << "Result: " << solve("day-06.input", 14) << std::endl;
}