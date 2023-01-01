#include "utils.h"
#include <vector>

long long toDec(const std::string& snafu)
{
    long long pow = 1;
    long long result = 0;
    for (long long i = 0; i < snafu.size(); ++i)
    {
        const char c = snafu[snafu.size() - i - 1];
        const long long v = c == '2' ? 2 : c == '1' ? 1 : c == '0' ? 0 : c == '-' ? -1 : c == '=' ? -2 : -100;

        result += v * pow;
        pow *= 5;
    }
    return result;
}

std::string toSnafu(long long i) {
    std::string result;
    while (i > 0) {
        const long long rem = i % 5;
        i /= 5;

        if (rem <= 2) {
            result += '0' + rem;
        } else 
        {
            result += rem == 3 ? "=" : "-";
            i += 1;
        }
    }
    return {result.rbegin(), result.rend()};
}

void part1(const Lines& lines) {
    long long total = 0;
    for (const std::string& line: lines) {
        const long long dec = toDec(line);
        printf("%s %lli\n", line.c_str(), dec);
        total += dec;
    }

    printf("Part 1: %s\n", toSnafu(total).c_str());
}

void solve(const char* filename)
{
    const Lines& lines = readFile(filename);

    part1(lines);
}

void test() {

}

int main(int argc, char* argv[])
{
    test();

    const char* filename = argc > 1 ? argv[1] : "day-25.sample";
    solve(filename);
}