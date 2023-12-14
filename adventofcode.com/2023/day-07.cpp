#include "utils.h"
#include "ut.hpp"

using T = long long;

struct Hand {
    std::string cards;
    T bid;
};
using Hands = std::vector<Hand>;

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

void solvePart1(Hands hands) {
    
}

void solve(const char* filename) {
    const Lines& lines = readFile(filename);
    const Hands hands = parse(lines);
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
}

int main(int argc, const char** argv) {
    test();
    const char* filename = argc > 1 ? argv[1] : "day-07.sample";
    solve(filename);
}