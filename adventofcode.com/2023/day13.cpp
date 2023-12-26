#include "utils.h"
#include "ut.hpp"
#include <format>
#include <unordered_map>
#include <utility>

using T = long long;

bool isPalindrome(std::string_view line, int i1) {
    int i2 = i1 + 1;
    while (i1 >=0 && i2 < line.size()) {
        if (line[i1--] != line[i2++]) {
            return false;
        }
    }

    return true;
}

T countDifferences(std::string_view line, int i1) {
    int i2 = i1 + 1;
    T count = 0;
    while (i1 >= 0 && i2 < line.size()) {
        if (line[i1--] != line[i2++]) {
            ++count;
            if (count > 1) {
                return -1;
            }
        }
    }
    return count;
}

T findMirror(std::string_view line, int start = 0) {
    for (size_t i = start; i < line.size() - 1; ++i) {
        if (line[i] == line[i+1]) {
            if (isPalindrome(line, i)) {
                return i;
            }
        }
    }

    return -1;
}

std::vector<std::string> transpose(std::vector<std::string>::const_iterator b, std::vector<std::string>::const_iterator e) {
    const auto length = b->size();
    std::vector<std::string> result = std::vector<std::string>(length, "");

    while (true) {
        for (size_t i = 0; i < length; ++i) {
            result[i].push_back(b->at(i));
        }
        if (++b == e) {
            break;
        }
        
    }
    return result;
}


T countDifferences(const std::vector<std::string_view>& region, size_t x) {
    T sum = 0;
    for (size_t y = 0; y < region.size(); ++y) {
        const T diff = countDifferences(region[y], x);
        if (diff < 0) {
            return -1;
        } 
        sum += diff;
        if (sum > 1) {
            return -1;
        }
    }
    return sum;
}

T findPart2(const std::vector<std::string_view>& region) {
    for (size_t x = 0; x < region[0].size(); ++x) {
        const T count = countDifferences(region, x);
        if (count == 1) {
            return x;
        }
    }
    return -1;
}

T findPart1(const std::vector<std::string_view>& region) {
    T start = 0;
    while (true) {
        if (const T pos = findMirror(region[0], start); pos != -1) {
            const bool allMirrors = std::all_of(begin(region) + 1, end(region), [pos](std::string_view line) { return isPalindrome(line, pos); });
            if (allMirrors) {
                return pos;
            } 
            start = pos + 1;
        } else {
            break;
        }
    }
    return -1;
}


void solvePart1(const Lines& lines) {
    auto b = std::begin(lines);
    auto e = std::find_if(begin(lines), end(lines), [](std::string_view line) { return line.empty(); });
    T sum = 0;
    while (b != lines.end()) {
        T count = findPart1({b, e});
        if (count != -1) {
            sum += count + 1;
            std::cout << std::format("Horizontal mirror found at {}. Have {} so far.\n", count, sum);
        }
        else {
            std::vector<std::string> transposed = transpose(b, e);
            count = findPart1({begin(transposed), end(transposed)});

            sum += 100*(count + 1);
            std::cout << std::format("Vertical mirror found at {}. Have {} so far.\n", count, sum);
        }

        b = e == end(lines) ? e : e + 1;
        e = std::find_if(b, end(lines), [](std::string_view line) { return line.empty(); });
    }

    std::cout << std::format("Total sum for part 1: {}\n", sum);
}

void solvePart2(const Lines& lines) {
    auto b = std::begin(lines);
    auto e = std::find_if(begin(lines), end(lines), [](std::string_view line) { return line.empty(); });
    T sum = 0;
    while (b != lines.end()) {
        T count = findPart2({b, e});
        if (count != -1) {
            sum += count + 1;
            std::cout << std::format("Horizontal mirror with a smudge found at {}. Have {} so far.\n", count, sum);
        }
        else {
            std::vector<std::string> transposed = transpose(b, e);
            count = findPart2({begin(transposed), end(transposed)});

            sum += 100*(count + 1);
            std::cout << std::format("Vertical mirror found with a smudge at {}. Have {} so far.\n", count, sum);
        }

        b = e == end(lines) ? e : e + 1;
        e = std::find_if(b, end(lines), [](std::string_view line) { return line.empty(); });
    }

    std::cout << std::format("Total sum for part 2: {}\n", sum);
}

void solve(const char* filename) {
    const Lines lines = readFile(filename);

    solvePart1(lines);
    solvePart2(lines);
}

void test() {
    using namespace boost::ut;

    "when any character does not match there is no palindrome"_test = [] {
        expect(false == isPalindrome("..##.#", 2));
        expect(false == isPalindrome("...#..", 2));
    };

    "when all characters match there is a plaindrome"_test = [] {
        expect(true == isPalindrome("##", 0)) << "##";
        expect(true == isPalindrome("..##..", 2)) << "..##..";
    };

    "when there is an mismatch on extra character it is still a palindrome"_test = [] {
        expect(true == isPalindrome("#..##..", 3)) << "#..##..";
        expect(true == isPalindrome("..##...", 2)) << "..##...";
    };

    "palindrome with an odd number of characters are ignored"_test = []{
        expect(false == isPalindrome(".#.", 1));
    };

    "finds the middle of the palindrome"_test = [] {
        expect(3_i == findMirror(".#.##.#."));
    };

    "when there are extra characters there are ignored and the middle of the palindrome is found"_test = [] {
        expect(3_i == findMirror(".#.##.#.#"));
        expect(4_i == findMirror("#.#.##.#."));
    };

    "when there is no palindrome with even number of characters"_test = []{
        expect(-1_i == findMirror(".###."));
    };
}

int main(int argc, const char** argv) {
    test();
    const char* filename = argc > 1 ? argv[1] : "day13.sample";
    solve(filename);
}