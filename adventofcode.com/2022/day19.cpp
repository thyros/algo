#include "utils.h"
#include "hash.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <cstdio>
#include <deque>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

// since there is only one factory, we can produce only one robot per turn
using MaxSpend = std::array<int, 3>;
using MaxSpends = std::vector<MaxSpend>;
using Blueprint = std::array<int, 7>;
using Blueprints = std::vector<Blueprint>;

Blueprints parse(const Lines& lines) {
    constexpr const char* format = "Blueprint %i: Each ore robot costs %i ore. "
                                 "Each clay robot costs %i ore. "
                                 "Each obsidian robot costs %i ore and %i clay. "
                                 "Each geode robot costs %i ore and %i obsidian.";

    Blueprints result;
    int id, r1c1, r2c1, r3c1, r3c2, r4c1, r4c2;
    for (const std::string& line: lines) {
        const int count = sscanf(line.c_str(), format, &id, &r1c1, &r2c1, &r3c1, &r3c2, &r4c1, &r4c2);
        assert(count == 7);

        result.push_back(Blueprint {id, r1c1, r2c1, r3c1, r3c2, r4c1, r4c2});
    }

    return result;
}

MaxSpends toMaxSpends(const Blueprints& blueprints) {
    MaxSpends result;
    std::transform(begin(blueprints), end(blueprints), std::back_inserter(result), 
    [](const Blueprint& bp) -> MaxSpend {
        return { max(bp[1], bp[2], bp[3], bp[5]), bp[4], bp[6]};
    });
    return result;
}


// [0-3] resources
// [4-7] bots
// [8] time left
using State = std::array<int, 9>;
using Seen = std::unordered_set<State, ArrayHash<9>>;
using Cache = std::unordered_map<State, int, ArrayHash<9>, ArrayEqual<9>>;


int dsf(int co, int cc, int cob1, int cob2, int cg1, int cg2, int t) {

    std::deque<State> deq;
    Seen seen;

    deq.push_back({0, 0, 0, 0, 1, 0, 0, 0, t});
    int best = 0;
    while (!deq.empty()) {
        auto [o,c,ob,g,r1,r2,r3,r4,t] = deq.front();
        deq.pop_front();

        best = std::max(best, g);
        if (t == 0) {
            continue;
        }

        // how much ore can we spend per turn
        const int maxOreNeeded = max(co, cc, cob1, cg1);

        // don't build more bots than we can spend
        // todo: why excess doesn't count
        r1 = std::min(r1, maxOreNeeded);
        r2 = std::min(r2, cob2);
        r3 = std::min(r3, cg2);

        // don't mine more minerals than is needed to produce next bot
        // if we have more minerals than can spend, the excess doesn't matter
        o = std::min(o, t*maxOreNeeded - r1*(t-1));
        c = std::min(c, t*cob2 - r2*(t-1));
        ob = std::min(ob, t*cg2 - r3*(t-1));

        State state {o, c, ob, g, r1, r2, r3, r4, t};
        if (seen.contains(state)) {
            continue;
        }
        seen.insert(state);

        if (seen.size() % 1000000 == 0)
            printf("%i %i %zi\n", t, best, seen.size());

        // do nothing
        deq.push_back({o+r1, c+r2, ob+r3, g+r4, r1, r2, r3, r4, t-1});
        // buy ore bot
        if (o >= co) {
            deq.push_back({ o+r1-co, c+r2, ob+r3, g+r4, r1+1,r2,r3,r4,t-1});
        }
        // buy clay bot
        if (o >= cc) {
            deq.push_back({o+r1-cc, c+r2, ob+r3, g+r4, r1,r2+1,r3,r4,t-1});
        }
        // buy obsidian bot
        if (o >= cob1 && c >= cob2) {
            deq.push_back({o+r1-cob1, c+r2-cob2, ob+r3, g+r4, r1,r2,r3+1,r4,t-1});
        }
        // buy geode bot
        if (o >= cg1 && ob >= cg2) {
            deq.push_back({o+r1-cg1, c+r2, ob+r3-cg2, g+r4, r1, r2, r3, r4+1, t-1});
        }
    }
    return best;
}

void part1(const Blueprints& blueprints, const MaxSpends& maxSpends, int time) {
    int result = 0;
    for (const Blueprint& bp: blueprints) {
        const int geodes = dsf(bp[1], bp[2], bp[3], bp[4], bp[5], bp[6], time);
        result += bp[0] * geodes;
    }
    printf("Part 1: %i\n", result);
}

void part2(const Blueprints& blueprints, const MaxSpends& maxSpends, int time) {
    int result = 1;
    for (int i = 0; i < 3; ++i) {
        const Blueprint& bp = blueprints[i]; 
        const int geodes = dsf(bp[1], bp[2], bp[3], bp[4], bp[5], bp[6], time);
        result *= geodes;
    }
    printf("Part 2: %i\n", result);
}

void solve(const char* filename) {
    const Lines lines = readFile(filename);
    const Blueprints blueprints = parse(lines);
    const MaxSpends maxSpends = toMaxSpends(blueprints);

    for (const Blueprint& bp: blueprints) {
        printf("%2i %2i %2i %2i %2i %2i %2i\n", bp[0], bp[1], bp[2], bp[3], bp[4], bp[5], bp[6]);
    }

    for (const MaxSpend& ms: maxSpends) {
        printf("%2i %2i %2i\n", ms[0], ms[1], ms[2]);
    }

    part1(blueprints, maxSpends, 24);
    part2(blueprints, maxSpends, 32);
}

void test() {

}

int main() {
    test();

    constexpr char filename[] { "day19.input" };
    solve(filename);
}