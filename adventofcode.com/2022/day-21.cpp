#include "utils.h"
#include <cassert>
#include <iostream>
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
    Tree(int v) : value(v) {}
    Tree(char t, Tree* l, Tree* r) : type(t), lhs(l), rhs(r) {}

    char type{'v'};
    ValueType value{0};
    Tree* lhs{nullptr};
    Tree* rhs{nullptr};
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

Tree* toTree(const std::string& id, const ValueMonkeys& valueMonkeys, const OpMonkeys& opMonkeys, const char* humn)
{
    // part 2
    if (humn && id == humn) {
        return new Tree('?', nullptr, nullptr);
    }

    if (const auto valueIt = valueMonkeys.find(id); valueIt != end(valueMonkeys))
    {
        return new Tree(valueIt->second);
    }

    if (const auto opIt = opMonkeys.find(id); opIt != end(opMonkeys))
    {
        Tree* lhs = toTree(opIt->second.lhs, valueMonkeys, opMonkeys, humn);
        Tree* rhs = toTree(opIt->second.rhs, valueMonkeys, opMonkeys, humn);
        return new Tree(opIt->second.op, lhs, rhs);
    }

    assert(false && "monkey not found");
    return nullptr;
}

ValueType calculate(const Tree* tree)
{
    switch (tree->type)
    {
        case '+':
            return calculate(tree->lhs) + calculate(tree->rhs);
        case '-':
            return calculate(tree->lhs) - calculate(tree->rhs);
        case '*':
            return calculate(tree->lhs) * calculate(tree->rhs);
        case '/':
            return calculate(tree->lhs) / calculate(tree->rhs);
    }
    return tree->value;
}

Tree* fakeLeftTree()
{
    return new Tree('+',
        new Tree('/',
            new Tree('*', new Tree('-', new Tree('+', new Tree('?', nullptr, nullptr), new Tree(1)), new Tree(3)), new Tree(2)),
            new Tree(4)),
        new Tree(5));
}

Tree* fakeRightTree() {
    return new Tree('+', 
                new Tree(4), new Tree('/', 
                                new Tree(8), new Tree('*',
                                    new Tree(2), new Tree('-',
                                        new Tree(3), new Tree('+',
                                            new Tree(1), new Tree('?', nullptr, nullptr))))));
}

Tree* flattenTree(Tree* tree) {
    if (tree->lhs) {
        tree->lhs = flattenTree(tree->lhs);
    }
    if (tree->rhs) {
        tree->rhs = flattenTree(tree->rhs);
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

        return new Tree(v);
    }

    return tree;
}

ValueType calculateReverse(const Tree* tree, ValueType valueSoFar)
{
    if (tree->type == '?')
        return valueSoFar;

    if (tree->lhs->type == 'v') {
        switch (tree->type) {
            case '+':
                return calculateReverse(tree->rhs, valueSoFar - tree->lhs->value);
            case '-':
                return calculateReverse(tree->rhs, tree->lhs->value - valueSoFar);
            case '/':
                return calculateReverse(tree->rhs, tree->lhs->value / valueSoFar);
            case '*':
                return calculateReverse(tree->rhs, valueSoFar / tree->lhs->value);
        }
    }

    if (tree->rhs->type == 'v') {
        switch (tree->type) {
            case '+':
                return calculateReverse(tree->lhs, valueSoFar - tree->rhs->value);
            case '-':
                return calculateReverse(tree->lhs, valueSoFar + tree->rhs->value);
            case '/':
                return calculateReverse(tree->lhs, valueSoFar * tree->rhs->value);
            case '*':
                return calculateReverse(tree->lhs, valueSoFar / tree->rhs->value);
        }
    }

    assert(false && "Unsupported node");
    return -1;
}

void part1(const ValueMonkeys& valueMonkeys, const OpMonkeys& opMonkeys)
{
    const Tree* tree = toTree("root", valueMonkeys, opMonkeys, nullptr);
    const ValueType v = calculate(tree);
    printf("Part 1: %lli\n", v);
}

void part2(const ValueMonkeys& valueMonkeys, const OpMonkeys& opMonkeys)
{
    // Tree* tree = fakeRightTree();
    Tree* tree = toTree("root", valueMonkeys, opMonkeys, "humn");
    
    const ValueType lv = calculate(tree->lhs);
    const ValueType rv = calculate(tree->rhs);
    
    tree = flattenTree(tree);



    const ValueType v = tree->lhs->type == 'v' ?
         calculateReverse(tree->rhs, tree->lhs->value) :
         calculateReverse(tree->lhs, tree->rhs->value);

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

    const char* filename = argc > 1 ? argv[1] : "day-21.input";
    solve(filename);
}