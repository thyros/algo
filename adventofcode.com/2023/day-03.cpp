#include "utils.h"
#include "ut.hpp"

bool hasSymbol(const Lines& lines, int y, int x) {
    if (y < 0 || x < 0 || y >= lines.size() || x >= lines[y].size()) {
        return false;
    }
    const char c = lines[y][x];
    return !isdigit(c) && c != '.';
}

bool hasSymbolAround(const Lines& lines, int y, int x1, int x2) {
    for (int x = x1-1; x < x2+1; ++x) {
        if (hasSymbol(lines, y-1, x) || hasSymbol(lines, y+1, x)) {
            return true;
        }
    }
    if (hasSymbol(lines, y, x1-1) || hasSymbol(lines, y, x2)) {
        return true;
    }

    return false;
}

long long to_number(std::string_view text, size_t begin, size_t end) {
    long long number = 0;
    while(begin < end) {
        number *= 10;
        number += text[begin++] - '0';
    }
    return number;
}

// end is one-passed the last digit
size_t findNumberEnd(std::string_view row, size_t start) {
    while (isdigit(row[start])) {
        ++start;
    }
    return start;
}

void solve_part1(const Lines& lines) {
    long long sum = 0;

    for (size_t y = 0; y < lines.size(); ++y) {
        std::string_view row = lines[y];

        size_t x = 0;
        do {
            if (isdigit(row[x])) {
                const size_t end = findNumberEnd(row, x);
                const size_t count = end - x;
                const bool symbolAround = hasSymbolAround(lines, y, x, end);
                const long long number = to_number(row, x, end);

                std::cout << "Found: " << row.substr(x, count) << " value: " << number << " " << symbolAround << std::endl;
                if (symbolAround) {
                    sum += number;
                }

                x = end;
            } else {
                ++x;
            }

        } while (x < row.size());        
    }

    std::cout << "Total: " << sum << std::endl;
}

void solve_part2(const Lines& lines) {
    
}

void solve(const char* filename) {
    const Lines lines = readFile(filename);

    solve_part1(lines);
    solve_part2(lines);
}

void test() {
    using namespace boost::ut;

    // const Lines exampleBoard = {"1.$",
    //                             "#4.",
    //                             ".@2"};

    "hasSymbol should return false if possition is outside the board"_test = [] {
        const Lines exampleBoard = {"1.",
                                    "#4"};

        expect(false == hasSymbol(exampleBoard, -1, 1));
        expect(false == hasSymbol(exampleBoard, 1, -1));
        expect(false == hasSymbol(exampleBoard, 2, 1));
        expect(false == hasSymbol(exampleBoard, 1, 2));
        expect(false == hasSymbol(exampleBoard, -1, -1));
        expect(false == hasSymbol(exampleBoard, 2, 2));        
    };

    "hasSymbol should return false for non-symbols"_test = [] {
        const Lines exampleBoard = {"1.",
                                    "#4"};

        expect(false == hasSymbol(exampleBoard, 0, 0));
        expect(false == hasSymbol(exampleBoard, 0, 1));
        expect(false == hasSymbol(exampleBoard, 1, 1));
    };    

    "hasSymbol should return true for symbols"_test = [] {
        const Lines exampleBoard = {"@+",
                                    "#&"};

        expect(true == hasSymbol(exampleBoard, 0, 0));
        expect(true == hasSymbol(exampleBoard, 0, 1));
        expect(true == hasSymbol(exampleBoard, 1, 0));
        expect(true == hasSymbol(exampleBoard, 1, 1));
    };

    "hasSymbolAround should return false in case number is at the edge of the board"_test = [] {
        const Lines exampleBoard = {"123"};
        expect(false == hasSymbolAround(exampleBoard, 0, 0, 3));
    };

    "hasSymbolAround should return false in case there are no symbols around the number"_test = [] {
        const Lines exampleBoard = {"#..%..?",
                                    ".......",
                                    "&.123.!",
                                    ".......",
                                    "&.^!#.!"};
        expect(false == hasSymbolAround(exampleBoard, 2, 2, 5));
    };

    "hasSymbolAround should return true in case there is symbol around the number"_test = [] {
        const Lines topLeft = {"#..",
                               ".1.",
                               "..."};
        expect(true == hasSymbolAround(topLeft, 1, 1, 2));

        const Lines topRight = {"..#",
                                ".1.",
                                "..."};
        expect(true == hasSymbolAround(topRight, 1, 1, 2));

        const Lines bottomLeft = {"...",
                                  ".1.",
                                  "#.."};
        expect(true == hasSymbolAround(bottomLeft, 1, 1, 2));

        const Lines bottomRight = {"...",
                                   ".1.",
                                   "..#"};
        expect(true == hasSymbolAround(bottomRight, 1, 1, 2));

        const Lines top = {".#.",
                           ".1.",
                           "..."};
        expect(true == hasSymbolAround(bottomRight, 1, 1, 2));

        const Lines bottom = {"...",
                              ".1.",
                              ".#."};
        expect(true == hasSymbolAround(bottomRight, 1, 1, 2));

        const Lines left = {"...",
                            "$1.",
                            "..."};
        expect(true == hasSymbolAround(bottomRight, 1, 1, 2));

        const Lines right = {"...",
                             ".1@",
                             "..."};
        expect(true == hasSymbolAround(bottomRight, 1, 1, 2));

    };
}

int main(int argc, const char** argv) {
    test();
    const char* filename = argc > 1 ? argv[1] : "day-03.sample";
    solve(filename);
}