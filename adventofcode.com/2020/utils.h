#pragma once

#include <istream>
#include <iterator>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

namespace Utils {

std::vector<std::string> ParseInput(std::istream& istr)
{
	std::vector<std::string> input;
	std::string line;
	while (getline(istr, line))
	{
		input.push_back(std::move(line));
	}
	return input;
}

std::vector<std::string_view> Tokenize(const std::string_view& input, char delimeter = ' ')
{
	std::vector<std::string_view> tokens;

	size_t b = 0;

	while (b != std::string::npos)
	{
		size_t e = input.find(delimeter, b);

		tokens.push_back(input.substr(b, e - b));
		b = e == std::string::npos ? e : e + 1;
	};

	return tokens;
}

std::string ToString(std::string_view& sv) {
    return std::string{begin(sv), end(sv)};
}

}

template <class T>
std::ostream& operator<<(std::ostream& ostr, const std::vector<T>& ts) {
    std::copy(begin(ts), end(ts), std::ostream_iterator<T>(ostr, " "));
    return ostr;
}
