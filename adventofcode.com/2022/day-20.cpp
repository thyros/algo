#include "utils.h"
#include "ut.hpp"
#include <algorithm>
#include <string>
#include <vector>

using Buffer = std::vector<int>;

// template<class ForwardIt>
// constexpr // since C++20
// ForwardIt // void until C++11
// rotate(ForwardIt first, ForwardIt n_first, ForwardIt last)
// {
//    if(first == n_first) return last;
//    if(n_first == last) return first;
 
//    ForwardIt read      = n_first;
//    ForwardIt write     = first;
//    ForwardIt next_read = first; // read position for when "read" hits "last"
 
//    while(read != last) {
//       if(write == next_read) next_read = read; // track where "first" went
//       std::iter_swap(write++, read++);
//    }
 
//    // rotate the remaining sequence into place
//    (rotate)(write, next_read, last);
//    return write;
// }


void print(const std::vector<int>& v) {
    for (int i: v) {
        printf("%i ", i);
    }
}

Buffer rotate(Buffer buffer, size_t index)
{
    const int value = buffer[index] % static_cast<int>(buffer.size());
    const size_t len = buffer.size();

    // printf("Rotating ");
    // print(buffer);
    // printf(" at %zi by %i\n", index, value);


    if (value > 0 && value + index < len) {
        const size_t first = index;
        const size_t n_first = first + 1;
        const size_t last = first + value + 1;
        // printf("\t(value > 0 && value + index < len) %zi %zi %zi\n", first, n_first, last);

        std::rotate(begin(buffer) + first, begin(buffer) + n_first, begin(buffer) + last);

        // printf("Got ");
        // print(buffer);
        // printf("\n");

    }
    else if (value > 0) 
    {
        const size_t first = len - index - 1;
        const size_t n_first = first + 1;
        const size_t last = len - value + len - index - 1;
        // printf("\t(value > 0) %zi %zi %zi\n", first, n_first, last);

        std::rotate(rbegin(buffer) + first, rbegin(buffer) + n_first, rbegin(buffer) + last);

        // printf("Got ");
        // print(buffer);
        // printf("\n");
    }
    else if (value < 0 && index > -value) {
        const size_t first = len - index - 1;
        const size_t n_first = first + 1;
        const size_t last = first - value + 1;
        // printf("\t(value < 0 && index - value > 0) %zi %zi %zi\n", first, n_first, last);

        std::rotate(rbegin(buffer) + first, rbegin(buffer) + n_first, rbegin(buffer) + last);

        // printf("Got ");
        // print(buffer);
        // printf("\n");
    } else if (value < 0) {
        const size_t first = index;
        const size_t n_first = first + 1;
        const size_t last = len + value + index;
        // printf("\t(value < 0) %zi %zi %zi\n", first, n_first, last);

        std::rotate(begin(buffer) + first, begin(buffer) + n_first, begin(buffer) + last);

        // printf("Got ");
        // print(buffer);
        // printf("\n");
    }


    return buffer;
}

Buffer parse(const Lines& lines) {
    Buffer result(lines.size(), 0);
    for (int i = 0; i < lines.size(); ++i) {
        result[i] = std::stoi(lines[i]);
    }
    return result;
}

void part1(const Buffer& buffer) {

    Buffer current = buffer;
    for (int a: buffer) {
        const auto it = std::find(begin(current), end(current), a);
        const auto i = it - begin(current);

        current = rotate(current, i);
    }

    const auto it = std::find(begin(current), end(current), 0);
    const size_t i = it - begin(current);

    const size_t first = (i + 1000) % buffer.size();
    const size_t second = (i + 2000) % buffer.size();
    const size_t third = (i + 3000) % buffer.size();

    const int value = current[first] + current[second] + current[third];
    printf("Part 1: %i+%i+%i = %i\n", current[first], current[second], current[third], value);

}

void solve(const char* filename) {
    const Lines lines = readFile(filename);
    const Buffer buffer = parse(lines);

    part1(buffer);
}

void test()
{
    using namespace boost::ut;

    "sample"_test = [] {
        expect(rotate(Buffer{1, 2, -3, 3, -2, 0, 4}, 0) == Buffer{2, 1, -3, 3, -2, 0, 4});
        expect(rotate(Buffer{2, 1, -3, 3, -2, 0, 4}, 0) == Buffer{1, -3, 2, 3, -2, 0, 4});
        expect(rotate(Buffer{1, -3, 2, 3, -2, 0, 4}, 1) == Buffer{1, 2, 3, -2, -3, 0, 4});
        expect(rotate(Buffer{1, 2, 3, -2, -3, 0, 4}, 2) == Buffer{1, 2, -2, -3, 0, 3, 4});
        expect(rotate(Buffer{1, 2, -2, -3, 0, 3, 4}, 2) == Buffer{1, 2, -3, 0, 3, 4, -2});
        expect(rotate(Buffer{1, 2, -3, 0, 3, 4, -2}, 3) == Buffer{1, 2, -3, 0, 3, 4, -2});
        expect(rotate(Buffer{1, 2, -3, 0, 3, 4, -2}, 5) == Buffer{1, 2, -3, 4, 0, 3, -2});
    };

    // "just overlap rotate"_test = [] {
    //     expect(rotate(Buffer{1, 2, -2, 3}, 2) == Buffer{1, 2, 3, -2});
    //     expect(rotate(Buffer{1, 2, 3, 4}, 1) == Buffer{1, 3, 4, 2});
    // };

    // "zero rotate"_test = [] {
    //     expect(rotate(Buffer{0, 1, 2}, 0) == Buffer{0, 1, 2});
    //     expect(rotate(Buffer{1, 2, 0}, 2) == Buffer{1, 2, 0});
    // };
    // "non-overflow positive rotate"_test = []
    // {
    //     expect(rotate(Buffer{3, 1, 2}, 1) == Buffer{3, 2, 1});
    //     expect(rotate(Buffer{2, 3, 1}, 0) == Buffer{3, 1, 2});
    //     expect(rotate(Buffer{2, 1, -3, 3, -2, 0, 4}, 0) == Buffer{1, -3, 2, 3, -2, 0, 4});
    // };
    // "non-overflow negative rotate"_test = []
    // {
    //     expect(rotate(Buffer{2, -1, 1}, 1) == Buffer{2, 1, -1});
    //     expect(rotate(Buffer{2, 1, -2}, 2) == Buffer{-2, 2, 1});
    //     expect(rotate(Buffer{2, 1, -3, 3, -2, 0, -4}, 6) == Buffer{2, 1, -4, -3, 3, -2, 0});
    // };
    // "reduced zero rotate"_test = [] 
    // {
    //     expect(rotate(Buffer{4, 2, 3, 0}, 0) == Buffer{4, 2, 3, 0});
    // };
    // "reduced positive rotate"_test = [] 
    // {
    //     expect(rotate(Buffer{5, 2, 3, 0}, 0) == Buffer{2, 5, 3, 0});
    //     expect(rotate(Buffer{7, 2, 3, 0}, 0) == Buffer{2, 3, 0, 7});
    // };
    // "reduced negative rotate"_test = [] 
    // {
    //     expect(rotate(Buffer{1, 2, 3, -5}, 3) == Buffer{1, 2, -5, 3});
    //     expect(rotate(Buffer{1, 2, 3, -7}, 3) == Buffer{-7, 1, 2, 3});
    // };
    // "overflow positive rotate"_test = []
    // {
    //     expect(rotate(Buffer{3, 4, 5, 6, 7, 8, 1}, 6) == Buffer {3, 1, 4, 5, 6, 7, 8});
    //     expect(rotate(Buffer{3, 4, 5, 6, 7, 2, 8}, 5) == Buffer {3, 2, 4, 5, 6, 7, 8});
    // };
    // "overflow negative rotate"_test = [] 
    // {
    //     expect(rotate(Buffer{4, -2, 5, 6, 7, 8, 9}, 1) == Buffer {4, 5, 6, 7, 8, -2, 9});
    //     expect(rotate(Buffer{-1, 4, 5, 6, 7, 8, 9}, 0) == Buffer {4, 5, 6, 7, 8, -1, 9});
    // };
}

int main(int argc, char* argv[])
{
    test();

    const char* filename = argc > 1 ? argv[1] : "day-20.sample";
    solve(filename);
}