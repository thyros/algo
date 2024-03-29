#include "utils.h"
#include "ut.hpp"
#include <algorithm>
#include <numeric>
#include <vector>

std::vector<int> fetchNumbers(std::string_view text, size_t begin, size_t end) {
    Tokens tokens = tokenize2(text, begin, end, " ");

    std::vector<int> numbers = tokensToNumbers(tokens);
    std::sort(numbers.begin(), numbers.end());
    return numbers;
}

std::vector<int> getMatchedNumbers(const std::vector<int>& winningNumbers, const std::vector<int>& actualNumbers) {
    std::vector<int> matchedNumbers;
    std::set_intersection(begin(winningNumbers), end(winningNumbers), begin(actualNumbers), end(actualNumbers), std::back_inserter(matchedNumbers));
    return matchedNumbers;
}

std::vector<int> getMatchedNumbers(std::string_view card) {
    const size_t colon = card.find(':');
    const size_t pipe = card.find('|', colon);

    const std::vector<int> winningNumbers = fetchNumbers(card, colon + 2, pipe - 1);
    const std::vector<int> actualNumbers = fetchNumbers(card, pipe + 2, card.npos); 
    const std::vector<int> matchedNumbers = getMatchedNumbers(winningNumbers, actualNumbers);

    std::cout << card << std::endl;
    std::cout << "Winning numbers: ";
    printAll(winningNumbers);
    std::cout << std::endl << "Actual numbers: ";
    printAll(actualNumbers);
    std::cout << std::endl << "Matched numbers: ";
    printAll(matchedNumbers);
    std::cout << std::endl;

    return matchedNumbers;
}

int evaluateCard(std::string_view card) {
    const std::vector<int> matchedNumbers = getMatchedNumbers(card);

    if (matchedNumbers.empty()) {
        return 0;
    }
    return pow(2, matchedNumbers.size() - 1);
}

void solvePart1(const Lines& lines) {
    long long sum = 0;
    for (std::string_view line: lines) {
        sum += evaluateCard(line);
    }
    std::cout << "Total part 1: " << sum << std::endl;
}

void solvePart2(const Lines& lines) {
    std::vector<int> cardsCount(lines.size(), 1);

    for (size_t i = 0; i < cardsCount.size(); ++i) {
        if (const int matchedNumbers = getMatchedNumbers(lines[i]).size(); matchedNumbers > 0) {
            for (int a = 1; a <= matchedNumbers; ++a) {
                cardsCount[i + a] += cardsCount[i];
            }
        }
    }

    std::cout << "Processed cards: " << std::endl;
    printAll(cardsCount);
    std::cout << std::endl;
    const int sum = std::accumulate(std::begin(cardsCount), std::end(cardsCount), 0, [](const auto l, const auto r) {return l+r;});
    std::cout << "Total part 2: " << sum << std::endl;
}

void solve(const char* filename) {
    const Lines lines = readFile(filename);
    solvePart1(lines);
    solvePart2(lines);
}

void test() {
    using namespace boost::ut;

    ""_test = [] {
        
    };
}

int main(int argc, const char** argv) {
    test();
    const char* filename = argc > 1 ? argv[1] : "day04.sample";
    solve(filename);
}