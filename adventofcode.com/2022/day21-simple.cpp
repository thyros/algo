#include "utils.h"
#include <cassert>
#include <string>
#include <unordered_map>
#include <iostream>

using ValueType = long long;
using ValueMonkeys = std::unordered_map<std::string, ValueType>;

ValueType parse(Lines lines)
{
    ValueMonkeys valueMonkeys;
    while (!lines.empty()) {
        const std::string line = lines.front();
        lines.erase(begin(lines));

        int value;
        char id[6] = {};
        char idLhs[5] = {};
        char idRhs[5] = {};
        char op;
        if (sscanf(line.c_str(), "%s %s %c %s", id, idLhs, &op, idRhs) == 4)
        {
            const std::string monkeyId{std::begin(id), std::begin(id) + 4};
            const std::string l {idLhs};
            const std::string r {idRhs};
            
            const auto lhsIt = valueMonkeys.find(l);
            const auto rhsIt = valueMonkeys.find(r);

            if (lhsIt != end(valueMonkeys) && rhsIt != end(valueMonkeys)) {
                const ValueType vl = lhsIt->second;
                const ValueType vr = rhsIt->second;
                const ValueType v = op == '+' ? vl + vr :
                              op == '-' ? vl - vr :
                              op == '*' ? vl * vr : vl / vr;
                valueMonkeys[monkeyId] = v;
            } else {
                lines.push_back(line);
            }
        }
        else if (sscanf(line.c_str(), "%s %i", id, &value) == 2)
        {
            const std::string monkeyId{std::begin(id), std::begin(id) + 4};
            valueMonkeys[monkeyId] = value;
        }


    };
    return valueMonkeys["root"];
}

void solve(const char* filename)
{
    const Lines& lines = readFile(filename);
    const ValueType v = parse(lines);

    std::cout << v << std::endl;
}

void test() {}

int main(int argc, char* argv[])
{
    test();

    const char* filename = argc > 1 ? argv[1] : "day21.input";
    solve(filename);
}