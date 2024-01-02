#include "utils.h"
#include "ut.hpp"
#include <format>
#include <unordered_map>

using T = long long;
using Lens = std::pair<std::string, T>;
using Box = std::vector<Lens>;
using HashMap = std::vector<Box>;

std::ostream & operator<< (std::ostream& out, const Lens& l) {
    out << std::format("[{}={}]", l.first, l.second);
    return out;
}

T hash(const std::string_view input) {
    T result = 0;

    for (const char c: input) {
        const int v = static_cast<int>(c);
        result += v;
        result *= 17;
        result = result % 256;
    }
    return result;
}

void remove(Box& box, std::string_view label) {
    Box::const_iterator it = std::find_if(begin(box), end(box), [label](const Lens l){ return l.first == label; });
    if (it != end(box)) {
        box.erase(it);
    }
}

void update(Box& box, Lens lens) {
    Box::iterator it = std::find_if(begin(box), end(box), [lens](const Lens l){ return l.first == lens.first; });
    if (it != end(box)) {
        *it = lens;
    } else {
        box.push_back(lens);
    }
}

T calculateFocusingPower(const HashMap& hashmap) {
    T total = 0;
    for (size_t i = 0; i < hashmap.size(); ++i) {
        const Box& box = hashmap[i];
        for (size_t j = 0; j < box.size(); ++j) {
            const Lens& lens = box[j];
            const T lensFocalPower = (i+1)*(j+1)*lens.second;
            total += lensFocalPower;
        }
    }
    return total;
}

void solvePart1(const Tokens& tokens) {
    T result = 0;

    for (const std::string_view token: tokens) {
        result += hash(token);
    }

    std::cout << std::format("Hash value for part 1: {}\n", result);
}

void solvePart2(const Tokens& tokens) {
    HashMap hashmap(256, Box {});

    for (const std::string_view token: tokens) {
        const Tokens subtokens = tokenize2(token, 0, std::string_view::npos, "=");

        if (subtokens.size() == 1) {
            const std::string_view label = subtokens[0].substr(0, subtokens[0].size() - 1);
            const T h = hash(label);
            Box& box = hashmap[h];
            remove(box, label);
        } else {
            const std::string_view label = subtokens[0];
            const T value = std::stoll(toString(subtokens[1]));
            const T h = hash(label);
            Box& box = hashmap[h];

            update(box, Lens {label, value});
        }
    }

    const T totalFocusingPower = calculateFocusingPower(hashmap);
    std::cout << std::format("Total focusing power: {}\n", totalFocusingPower);
}



void solve(const char* filename) {
    const Lines& lines = readFile(filename);
    const Tokens tokens = tokenize2(lines[0], 0, std::string_view::npos, ",");

    solvePart1(tokens);
    solvePart2(tokens);
}

void test() {
    using namespace boost::ut;

    "removing nonexisting label should not change the box"_test = [] {
        Box box {{"rn", 1},{"cm",4},{"qp",3}};
        Box expectedBox = box;
        remove(box, "qq");

        expect(expectedBox == box);
    };

    "removing existing label should remove it from the box"_test = [] {
        Box boxWithRnRemoved {{"rn", 1},{"cm",4},{"qp",3}};
        remove(boxWithRnRemoved, "rn");
        expect(Box {{"cm",4},{"qp",3}} == boxWithRnRemoved);

        Box boxWithCmRemoved {{"rn", 1},{"cm",4},{"qp",3}};
        remove(boxWithCmRemoved, "cm");
        expect(Box {{"rn",1},{"qp",3}} == boxWithCmRemoved);

        Box boxWithQpRemoved {{"rn", 1},{"cm",4},{"qp",3}};
        remove(boxWithQpRemoved, "qp");
        expect(Box {{"rn",1},{"cm",4}} == boxWithQpRemoved);    
    };

    "updating nonexisting label should add it at the end of the box"_test = [] {
        Box box {{"rn", 1},{"cm",4},{"qp",3}};
        update(box, {"ab",3});

        expect(Box {{"rn", 1},{"cm",4},{"qp",3},{"ab",3}} == box);
    };

    "updating existing label should update its value"_test = [] {
        Box boxWithRnUpdated {{"rn", 1},{"cm",4},{"qp",3}};
        update(boxWithRnUpdated, {"rn",6});
        expect(Box {{"rn", 6},{"cm",4},{"qp",3}} == boxWithRnUpdated);

        Box boxWithCmUpdated {{"rn", 1},{"cm",4},{"qp",3}};
        update(boxWithCmUpdated, {"cm",6});
        expect(Box {{"rn", 1},{"cm",6},{"qp",3}} == boxWithCmUpdated);

        Box boxWithQpUpdated {{"rn", 1},{"cm",4},{"qp",3}};
        update(boxWithQpUpdated, {"qp",6});
        expect(Box {{"rn", 1},{"cm",4},{"qp",6}} == boxWithQpUpdated);    
    };

    "testing total focusing power"_test = [] {
        HashMap hashmap {{{"rn", 1}, {"cm", 2}}, {}, {}, {{"ot", 7}, {"ab",5}, {"pc",6}}};
        expect(145_i == calculateFocusingPower(hashmap));
    };
}

int main(int argc, const char** argv) {
    test();
    const char* filename = argc > 1 ? argv[1] : "day15.sample";
    solve(filename);
}