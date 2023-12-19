#include "utils.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

struct OpMonkey
{
    std::string lhs;
    std::string rhs;
    char op;
};

using ValueType = long long;
using ValueMonkeys = std::unordered_map<std::string, ValueType>;
using OpMonkeys = std::unordered_map<std::string, OpMonkey>;

struct Tree
{
    Tree(ValueType v) : value(v) {}
    Tree(char t, std::unique_ptr<Tree> l, std::unique_ptr<Tree> r) : type(t), lhs(std::move(l)), rhs(std::move(r)) {}

    char type{'v'};
    ValueType value{0};
    std::unique_ptr<Tree> lhs{nullptr};
    std::unique_ptr<Tree> rhs{nullptr};
};

struct ParseResult
{
    ValueMonkeys valueMonkeys;
    OpMonkeys opMonkeys;
};

ParseResult parse(const Lines& lines)
{
    ValueMonkeys valueMonkeys;
    OpMonkeys opMonkeys;

    for (const std::string& line : lines)
    {
        ValueType value;
        char id[6] = {};
        char idLhs[5] = {};
        char idRhs[5] = {};
        char op;
        if (sscanf(line.c_str(), "%s %s %c %s", id, idLhs, &op, idRhs) == 4)
        {
            const std::string monkeyId{std::begin(id), std::begin(id) + 4};
            const std::string l{idLhs};
            const std::string r{idRhs};
            opMonkeys[monkeyId] = {.lhs = idLhs, .rhs = idRhs, .op = op};
        }
        else if (sscanf(line.c_str(), "%s %lli", id, &value) == 2)
        {
            const std::string monkeyId{std::begin(id), std::begin(id) + 4};
            valueMonkeys[monkeyId] = value;
        }
    }

    return {.valueMonkeys = std::move(valueMonkeys), .opMonkeys = std::move(opMonkeys)};
}

void print(const ValueMonkeys& valueMonkeys, const OpMonkeys& opMonkeys)
{
    for (const auto it : valueMonkeys)
    {
        printf("%s: %lli\n", it.first.c_str(), it.second);
    }
    for (const auto it : opMonkeys)
    {
        printf("%s: %s %c %s\n", it.first.c_str(), it.second.lhs.c_str(), it.second.op, it.second.rhs.c_str());
    }
    printf("Total %zi %zi %zi monkeys\n", valueMonkeys.size(), opMonkeys.size(), valueMonkeys.size() + opMonkeys.size());
}

std::unique_ptr<Tree> toTree(const std::string& id, const ValueMonkeys& valueMonkeys, const OpMonkeys& opMonkeys, const char* humn)
{
    // part 2
    if (humn && id == humn) {
        return std::make_unique<Tree>('?', nullptr, nullptr);
    }

    if (const auto valueIt = valueMonkeys.find(id); valueIt != end(valueMonkeys))
    {
        return std::make_unique<Tree>(valueIt->second);
    }

    if (const auto opIt = opMonkeys.find(id); opIt != end(opMonkeys))
    {
        std::unique_ptr<Tree> lhs = toTree(opIt->second.lhs, valueMonkeys, opMonkeys, humn);
        std::unique_ptr<Tree> rhs = toTree(opIt->second.rhs, valueMonkeys, opMonkeys, humn);
        return std::make_unique<Tree>(opIt->second.op, std::move(lhs), std::move(rhs));
    }

    assert(false && "monkey not found");
    return nullptr;
}

ValueType calculate(const Tree& tree)
{
    switch (tree.type)
    {
        case '+':
            return calculate(*tree.lhs) + calculate(*tree.rhs);
        case '-':
            return calculate(*tree.lhs) - calculate(*tree.rhs);
        case '*':
            return calculate(*tree.lhs) * calculate(*tree.rhs);
        case '/':
            return calculate(*tree.lhs) / calculate(*tree.rhs);
    }
    return tree.value;
}

std::unique_ptr<Tree> flattenTree(std::unique_ptr<Tree> tree) {
    if (tree->lhs) {
        tree->lhs = flattenTree(std::move(tree->lhs));
    }
    if (tree->rhs) {
        tree->rhs = flattenTree(std::move(tree->rhs));
    }
    if (tree->type == 'v' || tree->type == '?') {
        return tree;
    }

    if (tree->lhs->type == 'v' && tree->rhs->type == 'v') {
        const char op = tree->type;
        const ValueType vl = tree->lhs->value;
        const ValueType vr = tree->rhs->value;
        const ValueType v = op == '+' ? vl + vr :
                        op == '-' ? vl - vr :
                        op == '*' ? vl * vr : vl / vr;
        return std::make_unique<Tree>(v);
    }

    return tree;
}

ValueType calculateReverse(const Tree& tree, ValueType valueSoFar)
{
    if (tree.type == '?')
        return valueSoFar;

    if (tree.lhs->type == 'v') {
        switch (tree.type) {
            case '+':
                return calculateReverse(*tree.rhs, valueSoFar - tree.lhs->value);
            case '-':
                return calculateReverse(*tree.rhs, tree.lhs->value - valueSoFar);
            case '/':
                return calculateReverse(*tree.rhs, tree.lhs->value / valueSoFar);
            case '*':
                return calculateReverse(*tree.rhs, valueSoFar / tree.lhs->value);
        }
    }

    if (tree.rhs->type == 'v') {
        switch (tree.type) {
            case '+':
                return calculateReverse(*tree.lhs, valueSoFar - tree.rhs->value);
            case '-':
                return calculateReverse(*tree.lhs, valueSoFar + tree.rhs->value);
            case '/':
                return calculateReverse(*tree.lhs, valueSoFar * tree.rhs->value);
            case '*':
                return calculateReverse(*tree.lhs, valueSoFar / tree.rhs->value);
        }
    }

    assert(false && "Unsupported node");
    return -1;
}

void part1(const ValueMonkeys& valueMonkeys, const OpMonkeys& opMonkeys)
{
    const std::unique_ptr<Tree> tree = toTree("root", valueMonkeys, opMonkeys, nullptr);
    const ValueType v = calculate(*tree);
    printf("Part 1: %lli\n", v);
}

void part2(const ValueMonkeys& valueMonkeys, const OpMonkeys& opMonkeys)
{
    std::unique_ptr<Tree> tree = toTree("root", valueMonkeys, opMonkeys, "humn");
    tree = flattenTree(std::move(tree));

    const ValueType v = tree->lhs->type == 'v' ?
         calculateReverse(*tree->rhs, tree->lhs->value) :
         calculateReverse(*tree->lhs, tree->rhs->value);

    printf("Part 2: %lli\n", v);
}

void solve(const char* filename)
{
    const Lines& lines = readFile(filename);
    const auto [valueMonkeys, opMonkeys] = parse(lines);

    part1(valueMonkeys, opMonkeys);
    part2(valueMonkeys, opMonkeys);
}

void test() {}

int main(int argc, char* argv[])
{
    test();

    const char* filename = argc > 1 ? argv[1] : "day21.input";
    solve(filename);
}