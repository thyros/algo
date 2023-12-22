#include "utils.h"
#include "ut.hpp"
#include <format>
#include <unordered_map>

using T = unsigned long long;
using Numbers = std::vector<T>;
using Key = std::pair<std::string_view, T>;
template<>
struct std::hash<Key>
{
    std::size_t operator()(const Key& key) const noexcept
    {
        std::size_t h1 = std::hash<std::string_view>{}(key.first);
        return h1 ^ (key.second << 1); // or use boost::hash_combine
    }
};

using Cache = std::unordered_map<Key, T>;

void parse(const std::string& line, std::string& conditionReport, Numbers& numbers) {
    const Tokens tokens = tokenize(line);
    conditionReport = tokens[0];
    const Tokens numberTokens = tokenize2(tokens[1], 0, std::string_view::npos, ",");
    numbers = tokensToNumbers<T>(numberTokens);
}

bool willRangeFit(const std::string_view conditionReport, size_t pos, size_t length) {
    const size_t end = pos + length;
    if (end > conditionReport.size()) {
        return false;
    }

    while (pos < end) {
        if (conditionReport[pos++] == '.') {
            return false;
        }
    }

    return end == conditionReport.size() || conditionReport[end] != '#';
}

T countArrangements(const std::string_view conditionReport, const Numbers& numbers, T index = 0){
    if (index == numbers.size()) {
        const bool hasMoreSprings = conditionReport.find('#') != std::string_view::npos;
        return hasMoreSprings ? 0 : 1;
    }

    T sum = 0;
    const T length = numbers[index];
    for (size_t i = 0; i < conditionReport.size(); ++i) {
        const std::string_view substr = conditionReport.substr(i);
        if (willRangeFit(conditionReport, i, length)) {
            const T from = std::min(i + length + 1, conditionReport.size());
            sum += countArrangements(conditionReport.substr(from), numbers, index + 1);
        }
        if (conditionReport[i] == '#') {
            break;
        }
    }
    return sum;
}

T countArrangementsWithCache(Cache& cache, const std::string_view conditionReport, const Numbers& numbers, T index = 0){
    if (index == numbers.size()) {
        const bool hasMoreSprings = conditionReport.find('#') != std::string_view::npos;
        return hasMoreSprings ? 0 : 1;
    }

    if (auto it = cache.find(std::make_pair(conditionReport, index)); it != cache.end()) {
        return it->second;
    }

    T sum = 0;
    const T length = numbers[index];
    for (size_t i = 0; i < conditionReport.size(); ++i) {
        const std::string_view substr = conditionReport.substr(i);
        if (willRangeFit(conditionReport, i, length)) {
            const T from = std::min(i + length + 1, conditionReport.size());
            sum += countArrangementsWithCache(cache, conditionReport.substr(from), numbers, index + 1);
        }
        if (conditionReport[i] == '#') {
            break;
        }
    }

    cache[std::make_pair(conditionReport, index)] = sum;
    return sum;
}

void solvePart1(const Lines& lines) {
    T sum = 0;
    for (const std::string& line: lines) {
        std::string conditionReport;
        Numbers springs;
        parse(line, conditionReport, springs);
        const T n = countArrangements(conditionReport, springs);

        sum += n;

        // std::cout << std::format("\t{:4} : {}\n", n, line);
    }

    std::cout << std::format("\nThere are {} possible arrangements for part 1\n", sum);
}

void solvePart2(const Lines& lines) {
    T sum = 0;
    for (const std::string& line: lines) {
        std::string conditionReport;
        Numbers springs;
        parse(line, conditionReport, springs);

        std::string unfoldedReport = conditionReport + "?" + conditionReport + "?" + conditionReport + "?" + conditionReport + "?" + conditionReport;
        Numbers unfoldedSprings;
        for (int i = 0; i < 5; ++i) {
            std::copy(begin(springs), end(springs), std::back_inserter(unfoldedSprings));
        }

        Cache cache;
        const T n = countArrangementsWithCache(cache, unfoldedReport, unfoldedSprings);

        sum += n;

        // std::cout << std::format("\t{:8} : {}\n", n, line);
    }

    std::cout << std::format("\nThere are {} possible arrangements for part 2\n", sum);    
}


void solve(const char* filename) {
    const Lines& lines = readFile(filename);
    solvePart1(lines);

    solvePart2(lines);
}

void test() {
    using namespace boost::ut;

    "range will not fit if it's too long"_test = [] {
        expect(false == willRangeFit("???", 0, 4));
    };

    "range will not fit if there is a ."_test = [] {
        expect(false == willRangeFit("?.?", 0, 2));
        expect(false == willRangeFit(".??", 0, 1));
    };

    "range will fit if there is enough ?"_test = [] {
        expect(true == willRangeFit("???..", 0, 3));
    };

    "range will fit if there is enough #"_test = [] {
        expect(true == willRangeFit("###..", 0, 3));
    };

    "range will fit if report contains #"_test = [] {
        expect(true == willRangeFit("?#..", 0, 2));
        expect(true == willRangeFit("?#?#..", 0, 4));
    };

    "range will not fit if the next character is #"_test = []{
        expect(false == willRangeFit("?#..", 0, 1));
        expect(false == willRangeFit("?#?#..", 0, 3));
    };

    "range will fit if the next character is ?"_test = []{
        expect(true == willRangeFit("?#?.", 0, 2));
        expect(true == willRangeFit("?#?#?.", 0, 4));
    };

    "arrangement cannot start if the previous character is #"_test = []{
        expect(1_i == countArrangements("##?", {2}, 0));   
    };

    "arrangement cannot skip #"_test = []{
        expect(2_i == countArrangements("#.?.?", {1, 1}));
    };

    "arrangement should not succeed if there are unused #"_test = []{
        expect(6_i == countArrangements("???.??#??.?#??????", {3,2}));
    };

    "testing arrangements count on the sample data"_test = [] {
        expect(1_i == countArrangements("???.###", {1,1,3}, 0));
        expect(4_i == countArrangements(".??..??...?##.", {1,1,3}, 0));
        expect(1_i == countArrangements("?#?#?#?#?#?#?#?", {1,3,1,6}, 0));
        expect(1_i == countArrangements("????.#...#...", {4,1,1}, 0));
        expect(4_i == countArrangements("????.######..#####.", {1,6,5}, 0));
        expect(10_i == countArrangements("???????", {2, 1}, 0));
        expect(10_i == countArrangements("?###????????", {3,2,1}, 0));
    };

    "testing arrangements count on input data"_test = [] {
        expect(2_i == countArrangements("..?????#??", {4,1}));
        expect(12_i == countArrangements("#?..??.???##?.??", {2,1,1,3,1}));
    };

}

int main(int argc, const char** argv) {
    test();
    const char* filename = argc > 1 ? argv[1] : "day12.sample";
    solve(filename);
}

// ???????
// ## #
// ##  #
// ##   #
// ##    #
//  ## #
//  ##  #
//  ##   #
//   ## #
//   ##  #
//    ## #