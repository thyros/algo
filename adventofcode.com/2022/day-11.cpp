#include "utils.h"
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <functional>
#include <iostream>
#include <vector>

using ItemType = unsigned long long;
struct Monkey {
    std::vector<ItemType> items;

    std::function<ItemType(ItemType)> op;
    int div;

    int trueMonkey;
    int falseMonkey;
};
using Monkeys = std::vector<Monkey>;

Monkeys toMonkeys(const Lines& lines) {
    Monkeys result;
    for (size_t i = 0; i < lines.size(); i += 7) {
        int monkeyId, trueMonkey, falseMonkey, divisableBy;
        int mIds[8], count;
        int opValue;
        std::function<ItemType(ItemType)> op;
        sscanf(lines[i].c_str(), "Monkey %i:", &monkeyId);
        count = sscanf(lines[i+1].c_str(), "  Starting items: %i, %i, %i, %i, %i, %i, %i, %i", &mIds[0], &mIds[1], &mIds[2], &mIds[3], &mIds[4], &mIds[5], &mIds[6], &mIds[7]);
        if (lines[i+2] == "  Operation: new = old * old") {
            op = [](ItemType v) -> ItemType {return v*v;};
            printf("sqrt\n");
        } else if (sscanf(lines[i+2].c_str(), "  Operation: new = old + %i:", &opValue) == 1) {
            op = [opValue](ItemType v) -> ItemType {return v+opValue;};
        } else if (sscanf(lines[i+2].c_str(), "  Operation: new = old * %i:", &opValue) == 1) {
            op = [opValue](ItemType v) -> ItemType {return v*opValue;};
        }
        sscanf(lines[i+3].c_str(), "  Test: divisible by %i", &divisableBy);
        sscanf(lines[i+4].c_str(), "    If true: throw to monkey %i", &trueMonkey);
        sscanf(lines[i+5].c_str(), "    If false: throw to monkey %i", &falseMonkey);

        Monkey monkey {.op = std::move(op), .div = divisableBy, .trueMonkey = trueMonkey, .falseMonkey = falseMonkey};
        for (int i = 0; i < count; ++i) {
            monkey.items.push_back(mIds[i]);
        }
        result.push_back(std::move(monkey));
    }

    return result;
}

void print(const Monkeys& monkeys) {
    for (const Monkey& m: monkeys) {
        printf("items: %zi\n", m.items.size());
        printf("div: %i\n", m.div);
        printf("true: %i false: %i\n", m.trueMonkey, m.falseMonkey);
        printf("\n");
    }
}

void part1(Monkeys monkeys) {
    const int roundsCount = 20;

    std::vector<int> inspected(monkeys.size(), 0);
    for (int i = 0; i < roundsCount; ++i) {
        for (size_t i = 0; i < monkeys.size(); ++i) {
            Monkey& m = monkeys[i];
            while (!m.items.empty()) {
                ++inspected[i];

                const ItemType item = m.items[0];
                m.items.erase(m.items.begin());

                const ItemType newItem = m.op(item) / 3;
                const int nextMonkey = newItem % m.div == 0 ? m.trueMonkey : m.falseMonkey;
                monkeys[nextMonkey].items.push_back(newItem);

                printf("checking %llu, %llu -> %i\n", item, newItem, nextMonkey);
            }
        }
    }

    std::nth_element(begin(inspected), begin(inspected) + 1, end(inspected), std::greater{});
    printf("Part 1: %i\n", inspected[0] * inspected[1]);
}

void part2(Monkeys monkeys) {
    const int roundsCount = 10000;

    int lcm = 1;
    for (const Monkey& m: monkeys) {
        lcm *= m.div;
    }
    printf("lcm: %i\n", lcm);

    std::vector<ItemType> inspected(monkeys.size(), 0);
    for (int i = 0; i < roundsCount; ++i) {
        for (size_t i = 0; i < monkeys.size(); ++i) {
            Monkey& m = monkeys[i];
            while (!m.items.empty()) {
                ++inspected[i];

                const ItemType item = m.items[0];
                m.items.erase(m.items.begin());

                const ItemType newItem = m.op(item) % lcm;
                const int nextMonkey = newItem % m.div == 0 ? m.trueMonkey : m.falseMonkey;
                monkeys[nextMonkey].items.push_back(newItem);

                // printf("%zi checking %llu, %llu -> %i\n", i, item, newItem, nextMonkey);
            }
        }
    }

    for (ItemType i: inspected) {
        printf("%llu\n", i);
    }

    std::nth_element(begin(inspected), begin(inspected) + 1, end(inspected), std::greater{});
    printf("Part 2: %llu\n", inspected[0] * inspected[1]);
}

void solve(const char* filename) {
    const Lines lines = readFile(filename);

    Monkeys monkeys = toMonkeys(lines);
    // part1(monkeys);
    part2(monkeys);
}

void test() {

}

int main() {
    test();

    constexpr char filename[] { "day-11.input" };
    solve(filename);
}