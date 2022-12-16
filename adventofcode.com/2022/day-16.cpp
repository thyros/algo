#include "utils.h"
#include "ut.hpp"
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <unordered_map>
#include <unordered_set>

constexpr int INF = 10000;

using ValveId = std::string;
struct Valve {
    ValveId id;
    int flowRate;
    std::vector<ValveId> tunnels;
};

template<>
struct std::hash<Valve> {
    std::size_t operator()(const Valve& v) const noexcept
    {
        return std::hash<std::string>()(v.id);
    }
};

using Valves = std::unordered_map<std::string, Valve>;
using OpenValves = std::unordered_set<std::string>;

Valve toValve(std::string_view line) {
    char id[2] {};
    int flowRate {0};
    sscanf(line.data(), "Valve %s has flow rate=%i", id, &flowRate);

    const auto tunnelsIndex = line.find(' ', line.find("valve")) + 1;
    const Tokens tokens = tokenize2(line, tunnelsIndex, std::string_view::npos, ", ");
    std::vector<std::string> tunnels;
    for (const std::string_view tunnel: tokens) {
        tunnels.emplace_back(tunnel);
    }
    return {.id = id, .flowRate = flowRate, .tunnels = tunnels};
}

Valves toValves(const Lines& lines) {
    Valves result;
    for (const auto& line: lines) {
        Valve v = toValve(line);
        result[v.id] = v;
    }
    return result;
}

void print(const Valves& valves) {
    const auto printKeyValue = [](const auto& key, const auto& value) {
        printf("Valve: %s: %s %i %zi:[%s...\n", key.c_str(), value.id.c_str(), value.flowRate, value.tunnels.size(), value.tunnels[0].c_str()); 
    };

    for (const auto& [key, value] : valves) {
        printKeyValue(key, value);
    }
}

int rateValve(int flowRate, int movesLeft) {
    return flowRate * movesLeft;
}

int traverse(const Valves& valves, OpenValves openValves, ValveId currentValve, ValveId previousValve, int movesLeft, bool include, int& count) {
    ++count;
    if (movesLeft <= 0 || openValves.size() == valves.size()) {
        return 0;
    }

    const Valve& v = valves.at(currentValve);

    int result = 0;
    for (const std::string& nextValve: v.tunnels) {
        if (!openValves.contains(currentValve)) {
            OpenValves copy { openValves};
            copy.insert(currentValve);

            const int value = rateValve(v.flowRate, movesLeft);
            result = std::max(result + value, traverse(valves, copy, currentValve, nextValve, movesLeft - 2, include, count));
        }
        if (nextValve != previousValve || include) {
            result = std::max(result, traverse(valves, openValves, nextValve, currentValve, movesLeft - 1, include, count));
        }
    }

    return result;
}

template<int Size>
using Dist = std::array<std::array<int, Size>, Size>;

template<int Size>
void floydWarshall(Dist<Size>& dist) {
    const int size = dist.size();

    for (int k = 0; k < size; k++) {
        // Pick all vertices as source one by one
        for (int i = 0; i < size; i++) {
            // Pick all vertices as destination for the
            // above picked source
            for (int j = 0; j < size; j++) {
                // If vertex k is on the shortest path from
                // i to j, then update the value of
                // dist[i][j]
                if (dist[i][j] > (dist[i][k] + dist[k][j])
                    && (dist[k][j] != INF
                        && dist[i][k] != INF))
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
        }
    }
}

template <int Size>
void printDist(Dist<Size> dist) {
    const int size = dist.size();
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            if (dist[y][x] != INF) {
                printf("%3i", dist[y][x]);
            } else {
                printf("---");
            }
        }
        printf("\n");
    }
}

void part1(const Valves& valves) {
    const std::string firstValve {"AA"};
    const int movesLeft {3};

    for (int moves = 3; moves < 30; ++moves) {
        int count = 0;
        // const int result1 = traverse(valves, {}, firstValve, "", moves, true, count);
        // printf("Part1: include %i %i %i\n", moves, count, result1);

        count = 0;
        const int result2 = traverse(valves, {}, firstValve, "", moves, false, count);
        printf("Part1: dont    %i %i %i\n", moves, count, result2);
    }

}

template<int SIZE>
int t(const Dist<SIZE>& dist, const std::vector<int>& values, int current, std::unordered_set<int> openValves, int timeLeft) {
    if (timeLeft <= 0) {
        return 0;
    }

    int result = 0;
    for (int i = 0; i < SIZE; ++i) {
        if (openValves.contains(i) || values[i] == 0) {
            continue;
        }

        auto copy = openValves;
        copy.insert(i);

        const int timeTaken = dist[current][i] + 1;
        const int valveGenerated = values[i] * (timeLeft-timeTaken);
        const int restValue = t(dist, values, i, copy, timeLeft - timeTaken);
        result = std::max(result, valveGenerated + restValue);
    }

    return result;
}

template<int SIZE>
void p1(const Dist<SIZE>& dist, const std::vector<int>& values) {
    const int result = t(dist, values, 0, {}, 30);
    printf("Part 1: %i\n", result);
}

void solve(const char* filename) {
    const Lines lines = readFile(filename);
    Valves valves = toValves(lines);

    // print(valves);

    constexpr int SIZE = 10;
    Dist<SIZE> dist {};
    for (int y = 0; y < SIZE; ++y) {
        for (int x = 0; x < SIZE; ++x) {
            dist[y][x] = INF;
        }
    };

    std::vector<int> values(SIZE, 0);
    for (const auto& [key, value] : valves) {
        const int from = key[0] - 'A';
        for (const std::string& t: value.tunnels) {
            const int to = t[0] - 'A';
            dist[from][to] = 1;
        }

        dist[from][from] = 0;
        values[from] = value.flowRate;
    }

    printDist(dist);
    floydWarshall(dist);
    printf("After: \n");
    printDist(dist);

    // part1(valves);

    p1(dist, values);
}

void test() {
    using namespace boost::ut;

    "toValve"_test = [] {
        constexpr const char* singleTunnel {"Valve HH has flow rate=22; tunnel leads to valve GG"};
        const Valve singleTunnelValve = toValve(singleTunnel);
        expect("HH" == singleTunnelValve.id);
        expect(22_i == singleTunnelValve.flowRate);
        expect(1_i == singleTunnelValve.tunnels.size());
        expect("GG" == singleTunnelValve.tunnels[0]);
      
        constexpr const char* input {"Valve DD has flow rate=20; tunnels lead to valves CC, AA, EE"};
        const Valve valve = toValve(input);
        expect("DD" == valve.id);
        expect(20_i == valve.flowRate);
        expect(3_i == valve.tunnels.size());
        expect("CC" == valve.tunnels[0]);
        expect("AA" == valve.tunnels[1]);
        expect("EE" == valve.tunnels[2]);
    };
}

int main() {
    test();

    constexpr char filename[] { "day-16.sample" };
    solve(filename);

    printf("Done\n");
}