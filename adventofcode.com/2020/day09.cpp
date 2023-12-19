#include "utils.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <optional>

using Input = std::vector<long long>;

bool AnyPairSumsTo(Input::const_iterator first, Input::const_iterator last, long long value) {
    Input inputCopy(first, last);
    std::sort(begin(inputCopy), end(inputCopy));

    for (auto it = begin(inputCopy); it != end(inputCopy); ++it) {
        auto next = it + 1;

        if (std::binary_search(next, end(inputCopy), value - *it)) {
            return true;
        }
    }

    return false;
}

std::pair<Input::const_iterator, Input::const_iterator> FindRangeThatSumsTo(const Input& input, long long number) {

    Input::const_iterator first = begin(input);
    Input::const_iterator last = first + 1;

    long long sum = *first;
    while(last != end(input)) {
        if (sum < number) {
            sum += *last;
            ++last;
        } else if (sum > number) {
            sum -= *first;
            ++first;
        } else {
            break;
        }
    }

    if (last != end(input)) {
        return std::make_pair(first, last);
    }

    return std::make_pair(end(input), end(input));
}

int main() {
	std::ifstream ifstr("day09.input");
	const std::vector<std::string> text = Utils::ParseInput(ifstr);

    const auto input = [&text]() -> Input
    {
        Input input;
        input.reserve(text.size());
        std::transform(begin(text), end(text), std::back_inserter(input), [](const std::string& t) { return std::atoll(t.c_str()); });
        return input;
    }();

    const size_t preambleLength = 25;
    const auto invalidNumber = [&input, preambleLength]() -> std::optional<long long>{
        auto preambleBegins = begin(input);
        auto preambleEnds = preambleBegins + preambleLength;
        const long long value = *preambleEnds;
        for (;preambleEnds != end(input); ++preambleBegins, ++preambleEnds) {
            if (!AnyPairSumsTo(preambleBegins, preambleEnds, value)) {
                return std::make_optional(value);
            }
        }
        return {};
    }();

    if (invalidNumber) {
        const auto& [first, last] = FindRangeThatSumsTo(input, invalidNumber.value());
            if (last != end(input)) {
                std::cout << "Sequence is: " << std::endl;
                std::copy(first, last, std::ostream_iterator<long long>(std::cout, " "));
                std::cout << std::endl;

                const auto& [min, max] = std::minmax_element(first, last);
                std::cout << "Magic numbers are: " << *min << " " << *max << " " << *min + *max << std::endl;
            }        
    }

    std::cout << "Done searching" << std::endl;
}
