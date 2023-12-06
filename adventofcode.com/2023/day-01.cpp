#include "ut.hpp"
#include "utils.h"
#include <iostream>
#include <utility>
#include <vector>

std::vector<std::pair<std::string, int>> numbers = {
    {"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5}, {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9}};

int to_number(std::string_view text) {
    if (isdigit(text[0])) {
        return text[0] - '0';
    }

    for (const std::pair<std::string, int>& p: numbers) {
        const auto l = p.first.length();
        if (text.substr(0, l) == p.first) {
            return p.second;
        }
    }    
    return -1;
}

int find_number(std::string_view text) {
    for (auto i = 0; i < text.length(); ++i) {
        const int number = to_number(text.substr(i));
        if (number >= 0) {
            return number;
        }
    }
    return -1;
}

int rfind_number(std::string_view text) {
    for (int i = text.length() - 1; i >= 0; --i) {
        const int number = to_number(text.substr(i));
        if (number >= 0) {
            return number;
        }
    }
    return -1;
}


std::pair<int, int> find_numbers(std::string text) {
    const auto left = find_number(text);
    const auto right = rfind_number(text);

    if (left > -1 && right > -1) {
        return std::make_pair(left, right);
    }

    return std::make_pair(-1, -1);
}

int solve(const char* filename) {
    Lines lines = readFile(filename);

    int total = 0;
    for (const std::string& line: lines) {
        const std::pair<int, int> numbers = find_numbers(line);
        const int number = 10*numbers.first + numbers.second;
        total += number;
    }
    return total;
}

void test() {
    using namespace boost::ut;

    "When passing digits to_number should return them"_test = []{
        expect(1_i == to_number("1"));
        expect(2_i == to_number("2"));
        expect(3_i == to_number("3"));
        expect(4_i == to_number("4"));
        expect(5_i == to_number("5"));
        expect(6_i == to_number("6"));
        expect(7_i == to_number("7"));
        expect(8_i == to_number("8"));
        expect(9_i == to_number("9"));
    };

    "When passing words to_number should return their numberic value"_test = []{
        expect(1_i == to_number("one"));
        expect(2_i == to_number("two"));
        expect(3_i == to_number("three"));
        expect(4_i == to_number("four"));
        expect(5_i == to_number("five"));
        expect(6_i == to_number("six"));
        expect(7_i == to_number("seven"));
        expect(8_i == to_number("eight"));
        expect(9_i == to_number("nine"));
    };

    "When passing text with first digit find_number should find it first"_test = []{
        expect(1_i == find_number("12"));
        expect(2_i == find_number("24"));
        expect(3_i == find_number("35"));
        expect(4_i == find_number("46"));
        expect(5_i == find_number("57"));
    };

    "When passing text with digit find_number should find first number from left"_test = []{
        expect(1_i == find_number("aaa12"));
        expect(2_i == find_number("a24"));
        expect(3_i == find_number("aa35"));
        expect(4_i == find_number("affa46"));
        expect(5_i == find_number("aggga57"));
    };

    "When passing text with words find_number should find first number from left"_test = []{
        expect(1_i == find_number("aaaone"));
        expect(2_i == find_number("atwothree"));
        expect(3_i == find_number("aatreethreesix"));
        expect(4_i == find_number("affafofourrr"));
        expect(5_i == find_number("agggafiver"));
    };

    "When passing text with last digit rfind_number should find it first"_test = []{
        expect(1_i == rfind_number("aaa21"));
        expect(2_i == rfind_number("a42"));
        expect(3_i == rfind_number("aa53"));
        expect(4_i == rfind_number("affa54"));
        expect(5_i == rfind_number("aggga5"));
    };

    "When passing text with digit rfind_number should find first number from right"_test = []{
        expect(1_i == rfind_number("aaa1fefe"));
        expect(2_i == rfind_number("a42brf"));
        expect(3_i == rfind_number("aa53gggfd"));
        expect(4_i == rfind_number("affa4rr"));
        expect(5_i == rfind_number("aggga5dvg"));
    };

    "When passing text with digit rfind_number should find first number from right"_test = []{
        expect(1_i == rfind_number("aaa1fefe"));
        expect(2_i == rfind_number("a42brf"));
        expect(3_i == rfind_number("aa53gggfd"));
        expect(4_i == rfind_number("affa4rr"));
        expect(5_i == rfind_number("aggga5dvg"));
    };

    "When passing text with words rfind_number should find first number from right"_test = []{
        expect(1_i == rfind_number("atwoonevee"));
        expect(2_i == rfind_number("athreetwotree"));
        expect(3_i == rfind_number("aatreethreesis"));
        expect(4_i == rfind_number("affafofourrr"));
        expect(5_i == rfind_number("agggafiver"));
    };

    "When passing only one number to find_numbers both values should equal that number"_test = []{
        const std::pair<int, int> withDigit = find_numbers("vsdgge2fsdfv");
        expect(2_i == withDigit.first);
        expect(2_i == withDigit.second);

        const std::pair<int, int> withWord = find_numbers("fthrthreesis");
        expect(3_i == withWord.first);
        expect(3_i == withWord.second);
    };
}

int main(int argc, const char** argv) {
    test();
    if (argc >= 2) {
        std::cout << "Result: " << solve(argv[1]) << std::endl;
    }
}