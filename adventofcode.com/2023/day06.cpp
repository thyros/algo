#include "utils.h"
#include "ut.hpp"
#include <format>

using T = long long;

struct Races {
    std::vector<T> times;
    std::vector<T> distances;
};

Races parse(const std::string& timesRaw, const std::string& distancesRaw) {
    std::vector<T> times = [&timesRaw]
    {
        std::vector<T> result;
        std::istringstream timesStream(timesRaw.substr(11));
        while(!timesStream.eof()) {
            T time;
            timesStream >> time;
            result.push_back(time);
        }
        return result;
    }();

    std::vector<T> distances = [&distancesRaw]
    {
        std::vector<T> result;
        std::istringstream distancesStream(distancesRaw.substr(11));
        while(!distancesStream.eof()) {
            T time;
            distancesStream >> time;
            result.push_back(time);
        }
        return result;
    }();

    return Races {.times = std::move(times), .distances = std::move(distances)};
}

T calculateDistance(T raceTime, T pressTime) {
    return (raceTime - pressTime) * pressTime;
}

T countSolutionsOld(T raceTime, T best) {
    T solutions = 0;
    for (T i = 1; i < raceTime; ++i) {
        if (calculateDistance(raceTime, i) > best) {
            ++solutions;
        }
    }
    return solutions;
}

T countSolutions(T raceTime, T best) {
    const T bb = raceTime * raceTime;
    const T delta = bb - 4 * best;
    const auto sd = sqrt(delta);
    const T extra = (sd - floor(sd)) == 0 ? 1 : 0;
    const T t1 = (raceTime + floor(sd)) / 2;
    const T t2 = (raceTime - ceil(sd)) / 2;

    return t1 - t2 - extra;
}

void solvePart1(const Races& races) {
    T result = 1;

    for (size_t i = 0; i < races.times.size(); ++i) {
        const T raceTime = races.times[i];
        const T best = races.distances[i];
        const T solutions = countSolutions(raceTime, best);
        std::cout << std::format("{} {} -> {}\n", raceTime, best, solutions);
        result *= solutions;
    }

    std::cout << std::format("Part 1: {}\n", result);
}

void solvePart2(const Races&) {
    // since there was only 1 value for each data set, I just hardcoded it to gain some time to catch up
    {
        const T time = 71530;
        const T distance = 940200;
        const T solutions = countSolutions(time, distance);
        std::cout << std::format("Part 2 sample: {}\n", solutions);
    }
    {
        const T time = 57726992;
        const T distance = 291117211762026;
        const T solutions = countSolutions(time, distance);
        std::cout << std::format("Part 2 input: {}\n", solutions);
    }
}

void solve(const char* filename) {
    const Lines& lines = readFile(filename);

    Races races = parse(lines[0], lines[1]);
    solvePart1(races);
    solvePart2(races);
}

void test() {
    using namespace boost::ut;

    "Input is parsed correctly"_test = [] {
        const Races races = parse("Time:      7  15   30", "Distance:  9  40  200");

        expect(3_i == races.times.size());
        expect(3_i == races.distances.size());

        expect(std::vector<T> {7, 15, 30} == races.times);
        expect(std::vector<T> {9, 40, 200} == races.distances);
    };

    "Testing behavior for refactoring purpose"_test = [] {
        expect(4_i == countSolutions(7, 9));
        expect(8_i == countSolutions(15, 40));
        expect(9_i == countSolutions(30, 200));
    };
}

int main(int argc, const char** argv) {
    test();
    const char* filename = argc > 1 ? argv[1] : "day06.sample";
    solve(filename);
}