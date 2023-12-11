#include "utils.h"
#include "ut.hpp"
#include <format>

using Seeds = std::vector<long long>;

struct Mapping {
    long long to;
    long long from;
    long long length;
};

struct Section {
    std::vector<Mapping> mappings;
};

struct PuzzleInput {
    Seeds seeds;
    std::vector<Section> sections;
};

long long map(const Mapping& mapping, long long v) {
    const long long offset = v - mapping.from;
    if (offset >=0 && offset < mapping.length) {
        return mapping.to + offset;
    }
    return v;
}

Seeds readSeeds(std::string_view text) {
    const size_t colon = text.find(':');
    const Tokens tokens = tokenize(text.substr(colon+ 2));
        
    return tokensToNumbers<long long>(tokens);
}

Mapping readMapping(std::string_view text) {
    const std::vector<long long> numbers = tokensToNumbers<long long>(tokenize(text));

    return Mapping {.to = numbers[0], .from = numbers[1], .length = numbers[2]};
}

PuzzleInput readPuzzleInput(const Lines& lines) {
    Seeds seeds = readSeeds(lines[0]);

    std::cout << std::format("Read #{} seeds\n", seeds.size());

    std::vector<Section> sections {7, Section{}};


    std::cout << std::format("Reading section: 0\n");

    size_t currentSection = 0;
    const size_t topLinesToSkip = 3;
    for (size_t y = topLinesToSkip; y < lines.size(); /* do nothing */) {
        if (!isdigit(lines[y][0])) {
            // current section is done, move to the next one
            y += 2;
            ++currentSection;
            std::cout << std::format("Reading section: {}\n", currentSection);
        } else {
            std::cout << std::format("\tReading line: {}\n", y);
            sections[currentSection].mappings.push_back(readMapping(lines[y]));
            ++y;
        }
    }
    return PuzzleInput {.seeds = std::move(seeds), .sections = std::move(sections)};
}

void solve(const char* filename) {
    const Lines& lines = readFile(filename);
    std::cout << std::format("Read {} lines\n", lines.size());

    const PuzzleInput puzzleInput = readPuzzleInput(lines);
    std::cout << std::format("Seeds: #{} sections: #{}\n", puzzleInput.seeds.size(), puzzleInput.sections.size());

    long long min = -1;
    for (long long seed: puzzleInput.seeds) {
        std::cout << std::format("Seed: {}\n", seed);
        for (const Section& section: puzzleInput.sections) {
            for (const Mapping& mapping: section.mappings) {
                const long long previousSeed = seed;
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
    const char* filename = argc > 1 ? argv[1] : "day-05.input";
    solve(filename);
}