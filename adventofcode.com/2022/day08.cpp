#include "utils.h"
#include <cstdio>
#include <iostream>
#include <utility>

struct Tree {
    int height = -1;
    bool visible = false;
    int scenicScore = 0;
};

using Location = std::pair<int, int>;
using Grid = std::vector<std::vector<Tree>>;

Grid toGrid(const Lines& lines, int width, int height) {
    Grid result;
    for (int y = 0; y < height; ++y) {
        result.emplace_back(width, Tree());
    }

    for (size_t y = 0; y < lines.size(); ++y) {
        const auto& line = lines[y];
        for (size_t x = 0; x < line.size(); ++x) {
            const int height = static_cast<int>(line[x] - '0');
            const Tree tree {.height = height, .visible = false};
            result[y][x] = tree;
        }
    }

    return result;
}

int solvePart1(Grid grid, int width, int height) {
    // from left to right
    int currentHeight = -1;
    for (int y = 0; y < height; ++y) {
        currentHeight = -1;
        for (int x = 0; x < width; ++x) {
            Tree& tree = grid[y][x];
            if (currentHeight < tree.height) {
                tree.visible = true;
                currentHeight = tree.height;
            }
        }
    }

    // from right to left
    for (int y = 0; y < height; ++y) {
        currentHeight = -1;
        for (int x = width - 1; x >= 0; --x) {
            Tree& tree = grid[y][x];
            if (currentHeight < tree.height) {
                tree.visible = true;
                currentHeight = tree.height;
            }
        }
    }

    // from top to bottom
    for (int x = 0; x < width; ++x) {
        currentHeight = -1;
        for (int y = 0; y < height; ++y) {
            Tree& tree = grid[y][x];
            if (currentHeight < tree.height) {
                tree.visible = true;
                currentHeight = tree.height;
            }
        }
    }

    // from bottom to top
    for (int x = 0; x < width; ++x) {
        currentHeight = -1;
        for (int y = height - 1; y >= 0; --y) {
            Tree& tree = grid[y][x];
            if (currentHeight < tree.height) {
                tree.visible = true;
                currentHeight = tree.height;
            }
        }
    }

    int count = 0;
    for (const auto& row: grid) {
        for (const Tree& t: row) {
            if (t.visible) {
                ++count;
            }
        }
    }
    return count;
}


int scenicScore(const Grid& grid, int width, int height, int x, int y) {
    const int currentHeight = grid[y][x].height;

    int leftCount = 0;
    for (int xx = x - 1; xx >=0; --xx) {
        ++leftCount;
        if (grid[y][xx].height >= currentHeight) {
            break;
        }
    }

    int rightCount = 0;
    for (int xx = x + 1; xx < width; ++xx) {
        ++rightCount;
        if (grid[y][xx].height >= currentHeight) {
            break;
        }
    }

    int upCount = 0;
    for (int yy = y - 1; yy >=0; --yy) {
        const Location location = std::make_pair(x, yy);
        ++upCount;
        if (grid[yy][x].height >= currentHeight) {
            break;
        }
    }

    int downCount = 0;
    for (int yy = y + 1; yy < height; ++yy) {
        const Location location = std::make_pair(x, yy);
        ++downCount;
        if (grid[yy][x].height >= currentHeight) {
            break;
        }
    }

    return leftCount * rightCount * upCount * downCount;
}


int solvePart2(Grid grid, int width, int height) {
    for (int y = 0; y < height; ++y) {
        for (int x  = 0; x < width; ++x) {
            Tree& tree = grid[y][x];
            const int score = scenicScore(grid, width, height, x, y);
            tree.scenicScore = score;
        }
    }

    int bestScenicScore = 0;
    for (const auto& row: grid) {
        for (const Tree& t: row) {
            if (t.scenicScore > bestScenicScore) {
               bestScenicScore = t.scenicScore;
            }
        }
    }
    return bestScenicScore;
}

void solve(const char* filename) {
    const Lines lines = readFile(filename);
    const int height = lines.size();
    const int width = lines[0].size();
    const Grid grid = toGrid(lines, width, height);

    std::cout << "Part 1: " << solvePart1(grid, width, height) << std::endl;
    std::cout << "Part 2: " << solvePart2(grid, width, height) << std::endl;
}
    
int main() {
    constexpr char filename[] { "day08.input" };
    solve(filename);
}