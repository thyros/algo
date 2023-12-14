#include "utils.h"
#include "ut.hpp"
#include <algorithm>
#include <array>
#include <format>
#include <iostream>
#include <unordered_map>

using T = long long;

struct Play {
    std::string hand;
    T bid;
};
using Plays = std::vector<Play>;

int cardToValue(const char c) {
    // if (c == 'J')
    //     return 1;
    if (c == 'T')
        return 10;
    if (c == 'J')
        return 11;
    if (c == 'Q')
        return 12;
    if (c == 'K')
        return 13;
    if (c == 'A')
        return 14;
    return c - '0';
}

int handType(const std::string& hand) {
    const std::unordered_map<char, int> cardsCount = [&hand] {
        std::unordered_map<char, int> cardsCount;
        for (char c: hand) {
            ++cardsCount[c];
        }
        return cardsCount;
    }();

    const auto countValues = [&cardsCount](int value) {
        return std::count_if(begin(cardsCount), end(cardsCount), [value](const auto& cardCount) { return cardCount.second == value; }); 
    };

    if (countValues(5) == 1) {           
        return 6;                       // Five of a kind, where all five cards have the same label: AAAAA
    } else if (countValues(4) == 1) {    
        return 5;                       // Four of a kind, where four cards have the same label and one card has a different label: AA8AA
    } else if (countValues(3) == 1) {
        if (countValues(2) == 1) {
            return 4;                   // Full house, where three cards have the same label, and the remaining two cards share a different label: 23332
        }
        return 3;                       // Three of a kind, where three cards have the same label, and the remaining two cards are each different from any other card in the hand: TTT98
    } else if (countValues(2) == 2) {
        return 2;                       // Two pair, where two cards share one label, two other cards share a second label, and the remaining card has a third label: 23432
    } else if (countValues(2) == 1) {
        return 1;                       // One pair, where two cards share one label, and the other three cards have a different label from the pair and each other: A23A4
    }
    return 0;                           // High card, where all cards' labels are distinct: 23456
}

int promoteType(int type, int times) {
    
    for (int i = 0; i < times; ++i) {
        if (type == 5) type = 6;
        else if (type == 3) type = 5;
        else if (type == 2) type = 4;
        else if (type == 1) type = 3;
        else if (type == 0) type = 1;
    }

    return type;
}

bool greater(const Play& l, const Play& r) {
    int lv = handType(l.hand);
    int rv = handType(r.hand);

    if (lv < rv) {
        return true;
    }
    if (lv == rv) {
        for (size_t i = 0; i < l.hand.size(); ++i) {
            const int lv = cardToValue(l.hand[i]);
            const int rv = cardToValue(r.hand[i]);
            if (lv < rv) {
                return true;
            } else if (lv > rv) {
                return false;
            }
        }
    }

    return false;
}

std::vector<Play> parse(const Lines& lines) {
    const auto parseHand = [](const std::string& line) {
        std::istringstream istr(line);

        Play hand;
        istr >> hand.hand;
        istr >> hand.bid;
        return hand;
    };

    std::vector<Play> plays;
    for (const std::string& line: lines) {
        plays.push_back(parseHand(line));
    }

    return plays;
}

void solvePart1(Plays plays) {
    std::sort(begin(plays), end(plays), greater);

    T result = 0;
    for (size_t i = 0; i < plays.size(); ++i) {
        size_t rank = i + 1;
        std::cout << std::format("\t {}: {} ({}) {}\n", rank, plays[i].hand, handType(plays[i].hand), plays[i].bid);
        result += rank * plays[i].bid;
    }
    std::cout << std::format("Part 1: {}\n", result);
}

void solve(const char* filename) {
    const Lines& lines = readFile(filename);
    const Plays plays = parse(lines);

    solvePart1(plays);
}

void test() {
    using namespace boost::ut;

    "Parsing sample input"_test = [] {
        const Lines lines = {{"32T3K 765"},{"T55J5 684"},{"KK677 28"},{"KTJJT 220"},{"QQQJA 483"}};
        const Plays plays = parse(lines);

        expect(5_i == plays.size());
        expect(765_i == plays[0].bid);
        expect(684_i == plays[1].bid);
        expect(28_i == plays[2].bid);
        expect(220_i == plays[3].bid);
        expect(483_i == plays[4].bid);

        expect("32T3K" == plays[0].hand);
        expect("T55J5" == plays[1].hand);
        expect("KK677" == plays[2].hand);
        expect("KTJJT" == plays[3].hand);
        expect("QQQJA" == plays[4].hand);
    };

    "Type of the hand is calculated properly"_test = [] {
        expect(6_i == handType("55555"));
        expect(5_i == handType("55553"));
        expect(4_i == handType("55533"));
        expect(3_i == handType("55543"));
        expect(2_i == handType("55443"));
        expect(1_i == handType("55432"));
        expect(0_i == handType("45632"));        
        expect(0_i == handType("65A32"));
    };

    "Cards are compared properly"_test = [] {
        const Play max {.hand = "AAJAA", .bid = 0};
        const Play mid {.hand = "TTQTT", .bid = 0};
        const Play small {.hand = "TTTJT", .bid = 0};
        const Play smallest {.hand = "2AAAA", .bid = 0};
        expect(greater(small, mid));
        expect(greater(mid, max));
        expect(greater(small, max));

        expect(greater(smallest, small));
        expect(greater(smallest, mid));
        expect(greater(smallest, max));
    };

    "Card value"_test = [] {
        expect(cardToValue('A') > cardToValue('K'));
        expect(cardToValue('K') > cardToValue('Q'));
        expect(cardToValue('Q') > cardToValue('J'));
        expect(cardToValue('J') > cardToValue('T'));
        expect(cardToValue('T') > cardToValue('9'));
        expect(cardToValue('9') > cardToValue('8'));
        expect(cardToValue('8') > cardToValue('7'));
        expect(cardToValue('7') > cardToValue('6'));
        expect(cardToValue('6') > cardToValue('5'));
        expect(cardToValue('5') > cardToValue('4'));
        expect(cardToValue('4') > cardToValue('3'));
        expect(cardToValue('3') > cardToValue('2'));
    };
}

int main(int argc, const char** argv) {
    test();
    const char* filename = argc > 1 ? argv[1] : "day-07.sample";
    solve(filename);
}