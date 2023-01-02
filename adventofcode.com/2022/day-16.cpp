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
struct Valve
{
    ValveId id;
    int flowRate;
    std::vector<ValveId> tunnels;
};

template <>
struct std::hash<Valve>
{
    std::size_t operator()(const Valve& v) const noexcept { return std::hash<std::string>()(v.id); }
};

using Valves = std::unordered_map<std::string, Valve>;
using OpenValves = long long;
using ValveIds = std::unordered_map<std::string, int>;

Valve toValve(std::string_view line)
{
    char id[3]{};
    int flowRate{0};
    sscanf(line.data(), "Valve %s has flow rate=%i", id, &flowRate);

    const auto tunnelsIndex = line.find(' ', line.find("valve")) + 1;
    const Tokens tokens = tokenize2(line, tunnelsIndex, std::string_view::npos, ", ");
    std::vector<std::string> tunnels;
    for (const std::string_view tunnel : tokens)
    {
        tunnels.emplace_back(tunnel);
    }
    return {.id = id, .flowRate = flowRate, .tunnels = tunnels};
}

Valves toValves(const Lines& lines)
{
    Valves result;
    for (const auto& line : lines)
    {
        Valve v = toValve(line);
        result[v.id] = v;
    }
    return result;
}

void print(const Valves& valves)
{
    const auto printKeyValue = [](const auto& key, const auto& value)
    {
        printf("Valve: %s: %s %i %zi:[%s...\n",
            key.c_str(),
            value.id.c_str(),
            value.flowRate,
            value.tunnels.size(),
            value.tunnels[0].c_str());
    };

    for (const auto& [key, value] : valves)
    {
        printKeyValue(key, value);
    }
}

using Dist = std::vector<std::vector<int>>;

void floydWarshall(Dist& dist)
{
    const int size = dist.size();

    for (int k = 0; k < size; k++)
    {
        // Pick all vertices as source one by one
        for (int i = 0; i < size; i++)
        {
            // Pick all vertices as destination for the
            // above picked source
            for (int j = 0; j < size; j++)
            {
                // If vertex k is on the shortest path from
                // i to j, then update the value of
                // dist[i][j]
                if (dist[i][j] > (dist[i][k] + dist[k][j]) && (dist[k][j] != INF && dist[i][k] != INF))
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
        }
    }
}

void printDist(const Dist& dist)
{
    const int size = dist.size();
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            if (dist[y][x] != INF)
            {
                printf("%3i", dist[y][x]);
            }
            else
            {
                printf("---");
            }
        }
        printf("\n");
    }
}

int dsf(const Dist& dist, const std::vector<int>& values, int current, OpenValves bitmask, int timeLeft)
{
    if (timeLeft <= 0)
    {
        return 0;
    }

    int result = 0;
    for (int i = 0; i < dist.size(); ++i)
    {
        OpenValves bit = 1 << i;
        if (bitmask & bit || values[i] == 0)
        {
            continue;
        }

        const int timeTaken = dist[current][i] + 1;
        const int valveGenerated = values[i] * (timeLeft - timeTaken);
        if (valveGenerated <= 0)
        {
            continue;
        }

        const int restValue = dsf(dist, values, i, bitmask | bit, timeLeft - timeTaken);

        // printf("%i %i %i %i\n", i, timeTaken, valveGenerated, restValue);
        result = std::max(result, valveGenerated + restValue);
    }

    return result;
}

void part1(const Dist& dist, const std::vector<int>& values, int current)
{
    const int result = dsf(dist, values, current, 0, 30);
    printf("Part 1: %i\n", result);
}

void part2(const Dist& dist, const std::vector<int>& valves, int current)
{
    int result = 0;
    const int total = (1 << valves.size()) - 1;
    for (int i = 0; i <= total / 2; ++i) {
        const int pr = dsf(dist, valves, current, i, 26);
        const int er = dsf(dist, valves, current, total ^ i, 26);

        result = std::max(result, pr + er);

    }
    printf("Part 2: %i\n", result);
}

void solve(const char* filename)
{
    const Lines lines = readFile(filename);
    Valves valves = toValves(lines);

    // print(valves);

    ValveIds valveIds;
    for (const auto& [key, value] : valves)
    {
        // printf("Valve %s = %zi\n", key.c_str(), valveIds.size());
        valveIds[key] = valveIds.size();
    }

    const int size = lines.size();
    Dist dist(size, std::vector<int>(size, INF));
    std::vector<int> values(size, 0);
    for (const auto& [key, value] : valves)
    {
        const int from = valveIds[key];
        for (const std::string& t : value.tunnels)
        {
            const int to = valveIds[t];
            dist[from][to] = 1;
            // printf("%s-%i -> %s-%i = 1\n", key.c_str(), from, t.c_str(), to);
        }

        dist[from][from] = 0;
        values[from] = value.flowRate;
    }

    // printDist(dist);
    floydWarshall(dist);
    // printf("After: \n");
    // printDist(dist);

    part1(dist, values, valveIds["AA"]);
    part2(dist, values, valveIds["AA"]);
}

void test()
{
    using namespace boost::ut;

    "toValve"_test = []
    {
        constexpr const char* singleTunnel{"Valve HH has flow rate=22; tunnel leads to valve GG"};
        const Valve singleTunnelValve = toValve(singleTunnel);
        expect("HH" == singleTunnelValve.id);
        expect(22_i == singleTunnelValve.flowRate);
        expect(1_i == singleTunnelValve.tunnels.size());
        expect("GG" == singleTunnelValve.tunnels[0]);

        constexpr const char* input{"Valve DD has flow rate=20; tunnels lead to valves CC, AA, EE"};
        const Valve valve = toValve(input);
        expect("DD" == valve.id);
        expect(20_i == valve.flowRate);
        expect(3_i == valve.tunnels.size());
        expect("CC" == valve.tunnels[0]);
        expect("AA" == valve.tunnels[1]);
        expect("EE" == valve.tunnels[2]);
    };
}

int main(int argc, char* argv[])
{
    test();

    const char* filename = argc > 1 ? argv[1] : "day-16.sample";
    solve(filename);
}