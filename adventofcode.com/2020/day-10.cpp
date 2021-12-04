#include "utils.h"
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

using Input = std::vector<long>;

long SolvePartOne(const Input& input) {
    std::array<int, 3> differenceCounts = {0, 0, 0};
    for (int i = 1; i < static_cast<int>(input.size()); ++i) {
        const int difference = input[i] - input[i-1] - 1;
        ++differenceCounts[difference];
    }

    const long answer = differenceCounts[0] * differenceCounts[2];
    return answer;
}

long long SolvePartTwo(const Input& input) {
    const long maxJoltsDifference = 3;

    std::vector<long long> waysToGetHere(input.size(), 0);
    waysToGetHere[0] = 1;

    for (int i = 1; i < static_cast<int>(input.size()); ++i) {
        for (int j = i - 1; j >= 0 && input[i] - input[j] <= maxJoltsDifference; --j) {
            waysToGetHere[i] += waysToGetHere[j];
        }
    }
    return waysToGetHere.back();
}

void Solve(std::string_view filename) {
    std::cout << "Solving: " << filename << std::endl << std::endl;

	std::ifstream ifstr(filename.data());
	const std::vector<std::string> text = Utils::ParseInput(ifstr);

    auto input = [&text]() -> Input
    {
        Input input;
        input.reserve(text.size());
        std::transform(begin(text), end(text), std::back_inserter(input), [](const std::string& t) { return std::atol(t.c_str()); });
        return input;
    }();

    std::sort(begin(input), end(input));
    input.insert(begin(input), 0);
    input.push_back(input.back() + 3);
    std::cout << "Input: " << input << std::endl << std::endl << std::flush;

    std::cout << "Part one: " << SolvePartOne(input) << std::endl;
    std::cout << "Part two: " << SolvePartTwo(input) << std::endl;
}

int main() {
    Solve("day-10.input");
}
