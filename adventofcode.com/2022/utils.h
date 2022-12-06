#pragma once

#include <fstream>
#include <string>
#include <vector>

using Lines = std::vector<std::string>;
Lines readLines(std::istream& istr) {
    Lines lines;

    for (std::string line; std::getline(istr, line); ) {
        lines.emplace_back(line);
    }
    return lines;
}

Lines readFile(const char* filename) {
    std::ifstream file(filename);
    return readLines(file);
}
