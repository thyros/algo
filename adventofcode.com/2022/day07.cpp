#include "utils.h"
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <limits>
#include <vector>

struct Dir {
    Dir* parent = nullptr;
    int filesSize = 0;

    std::vector<Dir*> dirs;
};

struct ScanResult {
    int total;
    std::vector<int> dirsSizes;
};

Dir* parse(const Lines& lines) {
    Dir* currentDir = nullptr;

    for (const std::string& line: lines) {
        int size = 0;
        char name[20] = {};

        if (sscanf(line.c_str(), "%i %s", &size, &name) == 2) {         // 14848514 b.txt
            // add the file size to the total
            currentDir->filesSize += size;
        } else if (line == "$ cd ..") {                                 // cd ..
            // go to parent directory
            currentDir = currentDir->parent;
        }
        else if (sscanf(line.c_str(), "$ cd %s", &name) == 1) {         // cd a
            // create a new directory and link it with the parent
            Dir* newDir = new Dir();
            newDir->parent = currentDir;
            if (currentDir) {
                currentDir->dirs.push_back(newDir);
            }
            currentDir = newDir;
        }
    }

    // travese back to find root
    while (currentDir->parent != nullptr) {
        currentDir = currentDir->parent;
    }

    // return root
    return currentDir;
}

ScanResult Scan(const Dir* const currentDir) {
    int total = currentDir->filesSize;
    std::vector<int> dirsSizes;

    for (const Dir* const dir: currentDir->dirs) {
        auto [subTotal, subDirsSizes] = Scan(dir);

        total += subTotal;
        std::copy(begin(subDirsSizes), end(subDirsSizes), std::back_inserter(dirsSizes));
    }

    dirsSizes.push_back(total);
    return {.total = total, .dirsSizes = dirsSizes};
}

int sum(const std::vector<int>& dirsSizes, int maxSize) {
    int result = 0;
    for (int size: dirsSizes) {
        if (size <= maxSize) {
            result += size;
        }
    }
    return result;
}

int findBestDir(const std::vector<int>& dirsSizes, int requiredSize) {
    int bestDirSize = std::numeric_limits<int>::max();

    for (int size: dirsSizes) {
        if (size >= requiredSize && size < bestDirSize) {
            bestDirSize = size;
        }
    }
    return bestDirSize;
}

void solve(const char* filename) {
    const Lines lines = readFile(filename);
    
    Dir* dir = parse(lines);
    const auto [subTotal, subDirsSizes] = Scan(dir);

    std::cout << "Part 1: " << sum(subDirsSizes, 100000) << std::endl;

    const int requiredSize = 30000000;
    const int remainingDiskSize = 70000000 - subTotal;
    const int neededDiskSize = requiredSize - remainingDiskSize;

    std::cout << "Part 2: " << findBestDir(subDirsSizes, neededDiskSize) << std::endl;
}

int main() {
    constexpr char filename[] { "day07.input" };
    solve(filename);
}