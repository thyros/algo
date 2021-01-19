#include <algorithm>
#include <fstream>
#include <iostream>
#include <istream>
#include <string>
#include <vector>

// https://adventofcode.com/2020/day/2

struct Row {
    int min;
    int max;
    char c;
    std::string password;
};

// 6-17 v: jvbstgbwxnzvwmbvrm
std::vector<Row> ParseInput(std::istream& istr) {
    std::vector<Row> result;

    for (std::string line; std::getline(istr, line);) {
        int min {};
        int max {};
        char c {};
        char password[64];

        if (sscanf(line.c_str(), "%d-%d %c: %s", &min, &max, &c, password) == 4) {
            result.push_back(Row {min, max, c, password});
        }
    }

    return result;
}

int main()
{
    std::ifstream ifstr("day-02.input");
    std::vector<Row> xs = ParseInput(ifstr);

    std::cout << "Parsed " << xs.size() << " passwords" << std::endl;

    auto isPasswordValid_v1 = [](const Row& row) -> bool {
        const auto count = std::count(begin(row.password), end(row.password), row.c);
        return count >= row.min && count <= row.max;
    };

    auto isPasswordValid_v2 = [](const Row& row) -> bool {
        const int i1 = row.min - 1;
        const int i2 = row.max - 1;
        const auto& password = row.password;
        return (password[i1] == row.c) != (password[i2] == row.c);
    };

    const auto validPasswordsCount = std::count_if(begin(xs), end(xs), isPasswordValid_v2); 
    std::cout << "There are " << validPasswordsCount << " passwords" << std::endl;

    return validPasswordsCount;
}