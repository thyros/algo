#pragma once

#include <cassert>
#include <fstream>
#include <string>
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

std::vector<std::string_view> tokenize2(std::string_view input, 
                                        std::string_view::size_type begin = 0, 
                                        std::string_view::size_type end = std::string_view::npos, 
                                        std::string_view delimeter = " ")  {
	std::vector<std::string_view> tokens;

	size_t b = begin;

	while (b != end)
	{
		size_t e = input.find(delimeter, b);

		tokens.push_back(input.substr(b, e - b));
		b = e == end ? e : e + delimeter.length();
	};

	return tokens;

}

std::vector<std::string_view> tokenize(const std::string_view& input, std::string_view delimeter = " ")
{
    return tokenize2(input, 0, std::string_view::npos, delimeter);
}

std::string toString(std::string_view sv) {
    return std::string{begin(sv), end(sv)};
}