#pragma once

#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <vector>

using Lines = std::vector<std::string>;
using Tokens = std::vector<std::string_view>;

Lines readLines(std::istream& istr) {
    Lines lines;

    for (std::string line; std::getline(istr, line); ) {
        lines.emplace_back(line);
    }
    return lines;
}

Lines readFile(const char* filename) {
    std::ifstream file(filename);
    assert(file.good());
    return readLines(file);
}

Tokens tokenize2(std::string_view input, 
                                        std::string_view::size_type begin = 0, 
                                        std::string_view::size_type end = std::string_view::npos, 
                                        std::string_view delimeter = " ")  {
	Tokens tokens;

	size_t b = begin;

	while (b != end)
	{
		size_t e = input.find(delimeter, b);

		tokens.push_back(input.substr(b, e - b));
		b = e == end ? e : e + delimeter.length();
	};

	return tokens;

}

Tokens tokenize(std::string_view input, std::string_view delimeter = " ")
{
    return tokenize2(input, 0, std::string_view::npos, delimeter);
}

std::string toString(std::string_view sv) {
    return std::string{begin(sv), end(sv)};
}

template <typename T1, typename T2>
auto max(const T1 &a, const T2 &b) -> typename std::common_type<const T1&, const T2&>::type
{
    return a > b ? a : b;
}

template <typename T1, typename T2, typename ... Args>
auto max(const T1 &a, const T2 &b, const Args& ... args) -> typename std::common_type<const T1&, const T2&, const Args& ...>::type
{
    return max(max(a, b), args...);
}

void printAll(auto container) {
    std::copy(begin(container), end(container), std::ostream_iterator<double>(std::cout, " "));
}