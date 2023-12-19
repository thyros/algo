#include "utils.h"
#include "ut.hpp"
#include <format>

using T = long long;

using Seeds = std::vector<T>;
struct Mapping {
    T to;
    T from;
    T length;
};

struct Range {
    T from;
    T to;
};
using Ranges = std::vector<Range>;

struct Section {
    std::vector<Mapping> mappings;
};

struct PuzzleInput {
    Seeds seeds;
    std::vector<Section> sections;
};

T map(const Mapping& mapping, T v) {
    const T offset = v - mapping.from;
    if (offset >=0 && offset < mapping.length) {
        return mapping.to + offset;
    }
    return v;
}

Seeds readSeeds(std::string_view text) {
    const size_t colon = text.find(':');
    const Tokens tokens = tokenize(text.substr(colon+ 2));
        
    return tokensToNumbers<T>(tokens);
}

Mapping readMapping(std::string_view text) {
    const std::vector<T> numbers = tokensToNumbers<T>(tokenize(text));

    return Mapping {.to = numbers[0], .from = numbers[1], .length = numbers[2]};
}

PuzzleInput readPuzzleInput(const Lines& lines) {
    Seeds seeds = readSeeds(lines[0]);
    std::vector<Section> sections {7, Section{}};

    size_t currentSection = 0;
    const size_t topLinesToSkip = 3;
    for (size_t y = topLinesToSkip; y < lines.size(); /* do nothing */) {
        if (!isdigit(lines[y][0])) {
            y += 2;
            ++currentSection;
        } else {
            sections[currentSection].mappings.push_back(readMapping(lines[y]));
            ++y;
        }
    }
    return PuzzleInput {.seeds = std::move(seeds), .sections = std::move(sections)};
}

void solvePart1(const PuzzleInput& puzzleInput) {
    T min = -1;
    for (T seed: puzzleInput.seeds) {
        std::cout << std::format("Seed: {}\n", seed);
        for (const Section& section: puzzleInput.sections) {
            for (const Mapping& mapping: section.mappings) {
                const T previousSeed = seed;
                seed = map(mapping, seed);
                if (previousSeed != seed) { // seed was mapped in this section, skipping to the next one
                    std::cout << std::format("\t{}->{}\n", previousSeed, seed);
                    break;
                }
            }            
        }

        if (min == -1 || seed < min) {
            std::cout << std::format("Updating min: {}->{}\n", min, seed);
            min = seed;
        }
    }

    std::cout << "Result: " << min << std::endl;
}

Ranges fromSeeds(const Seeds& seeds) {
    Ranges ranges;

    for (size_t i = 0; i < seeds.size(); i+=2) {
        ranges.push_back(Range {.from = seeds[i], .to = seeds[i] + seeds[i+1]});
    }
    return ranges;
}

void solvePart2(const PuzzleInput& puzzleInput) {
    Ranges currentRanges = fromSeeds(puzzleInput.seeds);

    for (size_t i = 0; i < puzzleInput.sections.size(); ++i) {
        Ranges nextRanges;

        while (!currentRanges.empty()) {
            Range r = currentRanges.back();
            currentRanges.pop_back();

            const Section& section = puzzleInput.sections[i];
            bool mapped = false;
            for (const Mapping& m: section.mappings) {

                const T of = std::max(r.from, m.from);          // overlap from
                const T ot = std::min(r.to, m.from + m.length); // overlap to

                if (of < ot) {
                    nextRanges.push_back(Range {.from = of - m.from + m.to, .to = ot - m.from + m.to});

                    if (of > r.from) {
                        currentRanges.push_back(Range {.from = r.from, .to = of});
                    }
                    if (r.to > ot) {
                        currentRanges.push_back(Range {.from = ot, .to = r.to});
                    }

                    mapped = true;
                    break;
                }
            }

            if (!mapped) {
                nextRanges.push_back(r);
            }
        }

        currentRanges = nextRanges;
    }

    std::sort(begin(currentRanges), end(currentRanges), [](const Range& l, const Range& r) { return l.from < r.from; });

    std::cout << std::format("Part 2: {}\n", currentRanges[0].from);
    for (const Range& r: currentRanges) {
        std::cout << std::format("({},{})\n", r.from, r.to);
    }
}

void solve(const char* filename) {
    const Lines& lines = readFile(filename);
    std::cout << std::format("Read {} lines\n", lines.size());

    const PuzzleInput puzzleInput = readPuzzleInput(lines);
    std::cout << std::format("Seeds: #{} sections: #{}\n", puzzleInput.seeds.size(), puzzleInput.sections.size());

    solvePart1(puzzleInput);
    solvePart2(puzzleInput);
}

void test() {
    using namespace boost::ut;

    "When value is in the range, it is mapped"_test = [] {
        const Mapping m {.to = 10, .from = 5, .length = 5};
        
        expect(10_i == map(m, 5));
        expect(11_i == map(m, 6));
        expect(12_i == map(m, 7));
        expect(13_i == map(m, 8));
        expect(14_i == map(m, 9));
    };

    "When value is outside the range, it is not mapped"_test = [] {
        const Mapping m {.to = 10, .from = 5, .length = 5};

        expect(4_i == map(m, 4));
        expect(10_i == map(m, 10));
    };

    "Seeds are read correctly"_test = [] {
        const Seeds seeds = readSeeds("seeds: 79 14 55 13");
        expect(Seeds {79, 14, 55, 13} == seeds);
    };

    "Mapping is read correctly"_test = [] {
        const Mapping m = readMapping("52 50 48");
        expect(52_i == m.to);
        expect(50_i == m.from);
        expect(48_i == m.length);
    };

    "Puzzle input is read correctly"_test = [] {
        const PuzzleInput puzzleInput = readPuzzleInput({   "seeds: 79 14 55 13",
                                                            "",
                                                            "seed-to-soil map:",
                                                            "50 98 2",
                                                            "52 50 48",
                                                            "",
                                                            "soil-to-fertilizer map:",
                                                            "0 15 37",
                                                            "37 52 2",
                                                            "39 0 15",
                                                            "",
                                                            "fertilizer-to-water map:",
                                                            "49 53 8",
                                                            "0 11 42",
                                                            "42 0 7",
                                                            "57 7 4",
                                                            "",
                                                            "water-to-light map:",
                                                            "88 18 7",
                                                            "18 25 70",
                                                            "",
                                                            "light-to-temperature map:",
                                                            "45 77 23",
                                                            "81 45 19",
                                                            "68 64 13",
                                                            "",
                                                            "temperature-to-humidity map:",
                                                            "0 69 1",
                                                            "1 0 69",
                                                            "",
                                                            "humidity-to-location map:",
                                                            "60 56 37",
                                                            "56 93 4"});
        expect(Seeds {79, 14, 55, 13} == puzzleInput.seeds);
        expect(7_i == puzzleInput.sections.size());

        expect(2_i == puzzleInput.sections[6].mappings.size());
        expect(60_i == puzzleInput.sections[6].mappings[0].to);
        expect(56_i == puzzleInput.sections[6].mappings[0].from);
        expect(37_i == puzzleInput.sections[6].mappings[0].length);

        expect(56_i == puzzleInput.sections[6].mappings[1].to);
        expect(93_i == puzzleInput.sections[6].mappings[1].from);
        expect(4_i == puzzleInput.sections[6].mappings[1].length);
    };
}

int main(int argc, const char** argv) {
    test();
    const char* filename = argc > 1 ? argv[1] : "day05.input";
    solve(filename);
}