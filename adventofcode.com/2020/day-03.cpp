#include <fstream>
#include <iostream>
#include <istream>
#include <iterator>
#include <optional>
#include <string>
#include <utility>
#include <vector>

// https://adventofcode.com/2020/day/3

using Map = std::vector<std::string>;

Map ParseInput(std::istream& istr) {
    Map map;

    for (std::string line; std::getline(istr, line);) {
        map.push_back(std::move(line));
    }
    return map;
}

int CountTrees(const Map& map, int dx, int dy) {
    int treesEncountered = 0;
    size_t x = 0;
    size_t y = 0;
    for (; y < map.size(); x = (x + dx) % map[0].size(), y += dy) {
        treesEncountered += map[y][x] == '#';
    }
    return treesEncountered;
}

int main()
{
    std::ifstream ifstr("day-03.input");
    Map map = ParseInput(ifstr);

    std::cout << "Map size: " << map[0].size() << 'x' << map.size() << std::endl;

    const int treesEncountered = CountTrees(map, 1, 1) *
                                 CountTrees(map, 3, 1) *
                                 CountTrees(map, 5, 1) *
                                 CountTrees(map, 7, 1) *
                                 CountTrees(map, 1, 2);
    std::cout << "Trees endountered v2: " << treesEncountered << std::endl;
}