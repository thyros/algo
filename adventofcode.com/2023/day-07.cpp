#include "utils.h"
#include "ut.hpp"
#include <algorithm>
#include <format>
#include <iostream>
#include <unordered_map>

using T = long long;

struct Hand {
    std::string cards;
    T value;
    T bid;
};
using Hands = std::vector<Hand>;

int cardToValue(const char c) {
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

bool handsDescending(const Hand& l, const Hand& r) {
    if (l.value < r.value) {
        return true;
    }
    if (l.value == r.value) {
        for (size_t i = 0; i < l.cards.size(); ++i) {
            const int lv = cardToValue(l.cards[i]);
            const int rv = cardToValue(r.cards[i]);
            if (lv < rv) {
                return true;
            } else if (lv > rv) {
                return false;
            }
        }
    }

    return false;
}

std::vector<Hand> parse(const Lines& lines) {
    const auto parseHand = [](const std::string& line) {
        std::istringstream istr(line);

        Hand hand;
        istr >> hand.cards;
        istr >> hand.bid;
        return hand;
    };

    std::vector<Hand> hands;
    for (const std::string& line: lines) {
        hands.push_back(parseHand(line));
    }

    return hands;
}

int getHighestCard(std::string cards) {
    std::sort(begin(cards), end(cards), [](const char l, const char r) { return cardToValue(r) < cardToValue(l); });
    const char c = cards[0];
    return cardToValue(c);
}

bool anyCardHasValue(const std::unordered_map<char, int>& cardsCount, int value) {
    return std::any_of(begin(cardsCount), end(cardsCount), [value](const auto& cardCount) { return cardCount.second == value; });
}

int rateCards(const std::string& cards) {
    std::unordered_map<char, int> cardsCount;

    for (char c: cards) { 
        ++cardsCount[c];
    }

    // 1000 - (1) - Five of a kind, where all five cards have the same label: AAAAA
    // 900  - (2) - Four of a kind, where four cards have the same label and one card has a different label: AA8AA
    // 800  - (2) - Full house, where three cards have the same label, and the remaining two cards share a different label: 23332
    // 700  - (3) - Three of a kind, where three cards have the same label, and the remaining two cards are each different from any other card in the hand: TTT98
    // 600  - (3) - Two pair, where two cards share one label, two other cards share a second label, and the remaining card has a third label: 23432
    // 500  - (4) - One pair, where two cards share one label, and the other three cards have a different label from the pair and each other: A23A4
    // x    - (5) - High card, where all cards' labels are distinct: 23456

    if (cardsCount.size() == 1) {           // Five of a kind
        return 1000;
    } else if (cardsCount.size() == 2) {    // Four of a kind or Full house
        const bool isFourOfKind = anyCardHasValue(cardsCount, 4);
        return isFourOfKind ? 900 : 800;
    } else if (cardsCount.size() == 3) {    // Three of a kind or Two pair
        const bool isTreeOfKind = anyCardHasValue(cardsCount, 3);
        return isTreeOfKind ? 700 : 600;
    } else if (cardsCount.size() == 4) {    // One Pair
        return 500;
    }
    // High card
    return getHighestCard(cards);
}

void solvePart1(Hands hands) {
    for (Hand& hand: hands) {
        hand.value = rateCards(hand.cards);
    }
    std::sort(begin(hands), end(hands), handsDescending);

    T result = 0;
    for (size_t i = 0; i < hands.size(); ++i) {
        size_t rank = i + 1;
        std::cout << std::format("\t {}: {} ({}) {}\n", rank, hands[i].cards, hands[i].value, hands[i].bid);
        result += rank * hands[i].bid;
    }
    std::cout << std::format("Part 1: {}\n", result);
}

void solve(const char* filename) {
    const Lines& lines = readFile(filename);
    const Hands hands = parse(lines);

    solvePart1(hands);
}

void test() {
    using namespace boost::ut;

    "Parsing sample input"_test = [] {
        const Lines lines = {{"32T3K 765"},{"T55J5 684"},{"KK677 28"},{"KTJJT 220"},{"QQQJA 483"}};
        const Hands hands = parse(lines);

        expect(5_i == hands.size());
        expect(765_i == hands[0].bid);
        expect(684_i == hands[1].bid);
        expect(28_i == hands[2].bid);
        expect(220_i == hands[3].bid);
        expect(483_i == hands[4].bid);

        expect("32T3K" == hands[0].cards);
        expect("T55J5" == hands[1].cards);
        expect("KK677" == hands[2].cards);
        expect("KTJJT" == hands[3].cards);
        expect("QQQJA" == hands[4].cards);
    };

    "Type of the hand is calculated properly"_test = [] {
        expect(1000_i == rateCards("55555"));
        expect(900_i == rateCards("55553"));
        expect(800_i == rateCards("55533"));
        expect(700_i == rateCards("55543"));
        expect(600_i == rateCards("55443"));
        expect(500_i == rateCards("55432"));
        expect(6_i == rateCards("45632"));        
        expect(14_i == rateCards("65A32"));
    };

    "Cards are compared properly"_test = [] {
        const Hand max {.cards = "AAJAA", .value = 900, .bid = 0};
        const Hand mid {.cards = "TTQTT", .value = 900, .bid = 0};
        const Hand small {.cards = "TTTJT", .value = 900, .bid = 0};
        expect(handsDescending(small, mid));
        expect(handsDescending(mid, max));
        expect(handsDescending(small, max));
    };
}

int main(int argc, const char** argv) {
    test();
    const char* filename = argc > 1 ? argv[1] : "day-07.sample";
    solve(filename);
}