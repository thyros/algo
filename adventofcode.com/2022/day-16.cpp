#include "utils.h"
#include "ut.hpp"
#include <cassert>
#include <cstdio>
#include <unordered_map>

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

using AllValves = std::unordered_map<std::string, Valve>;
using NonZeroValvesBitMask = long long;
using ValveIndices = std::unordered_map<std::string, int>;

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

AllValves toValves(const Lines& lines)
{
    AllValves result;
    for (const auto& line : lines)
    {
        Valve v = toValve(line);
        result[v.id] = v;
    }
    return result;
}

void print(const AllValves& valves)
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

int dsf(const Dist& dist, const std::vector<int>& flowRates, const std::vector<int>& nonZeroIndices, int currentValve, NonZeroValvesBitMask bitmask, int timeLeft)
{
    if (timeLeft <= 0)
    {
        return 0;
    }

    int result = 0;
    for (int a = 0; a < nonZeroIndices.size(); ++a)
    {
        const int nextValve = nonZeroIndices[a];
        const int flowRate = flowRates[nextValve];

        NonZeroValvesBitMask bit = 1 << a;
        if (currentValve == nextValve || bitmask & bit || flowRate == 0)
        {
            continue;
        }

        const int timeTaken = dist[currentValve][nextValve] + 1; // traverse + open
        const int valveGenerated = flowRate * (timeLeft - timeTaken);
        if (valveGenerated <= 0)
        {
            continue;
        }

        const int restValue = dsf(dist, flowRates, nonZeroIndices, nextValve, bitmask | bit, timeLeft - timeTaken);

        // printf("%i %i %i %i\n", i, timeTaken, valveGenerated, restValue);
        result = std::max(result, valveGenerated + restValue);
    }

    return result;
}

void part1(const Dist& dist, const std::vector<int>& flowRates, const std::vector<int>& nonZeroIndices, int currentValve)
{
    const int result = dsf(dist, flowRates, nonZeroIndices, currentValve, 0, 30);
    printf("Part 1: %i\n", result);
}

void part2(const Dist& dist, const std::vector<int>& flowRates, const std::vector<int>& nonZeroIndices, int currentValve)
{
    int result = 0;
    const NonZeroValvesBitMask total = (1ll << nonZeroIndices.size()) - 1;
    printf("nonZeroIndices: %zi total: %zi\n", nonZeroIndices.size(), total);
    for (NonZeroValvesBitMask i = 0; i <= total / 2; ++i) {
        const int pr = dsf(dist, flowRates, nonZeroIndices, currentValve, i, 26);
        const int er = dsf(dist, flowRates, nonZeroIndices, currentValve, total ^ i, 26);

        if (i % 2500 == 0)
            printf("%zi: %i = %i + %i\n", i, pr+er, pr, er);

        result = std::max(result, pr + er);

    }
    printf("Part 2: %i\n", result);
}

void solve(const char* filename)
{
    const Lines lines = readFile(filename);
    AllValves valves = toValves(lines);

    // generate map of valve id -> index
    ValveIndices valveIndices;
    for (const auto& [key, value] : valves)
    {
        const int index = valveIndices.size();
        valveIndices[key] = index;
    }

    const int size = valves.size();
    Dist dist(size, std::vector<int>(size, INF));
    std::vector<int> flowRates(size, 0);
    std::vector<int> nonZeroIndices;
    for (const auto& [key, value] : valves)
    {
        const int from = valveIndices[key];
        for (const std::string& t : value.tunnels)
        {
            const int to = valveIndices[t];
            dist[from][to] = 1;
        }

        dist[from][from] = 0;
        flowRates[from] = value.flowRate;
        if (value.flowRate != 0) {
            nonZeroIndices.push_back(from);
        }
    }

    floydWarshall(dist);

    part1(dist, flowRates, nonZeroIndices, valveIndices["AA"]);
    part2(dist, flowRates, nonZeroIndices, valveIndices["AA"]);
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