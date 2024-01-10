#include "utils.h"
#include "ut.hpp"
#include <format>
#include <functional>
#include <unordered_map>

using T = unsigned long long;

struct Condition {
    char param;
    char op;
    T value;
    std::string flowId;
};

struct Workflow {
    std::string id;
    std::vector<Condition> conditions;
    std::string fallback;
};
using Workflows = std::unordered_map<std::string, Workflow>;

struct Part {
    T x;
    T m;
    T a;
    T s;
};
using Parts = std::vector<Part>;

enum class PartResult {
    unknown,
    rejected,
    accepted
};

struct Range {
    T min;
    T max;
};
using PartRange = std::unordered_map<char, Range>;

Workflow parseWorkflow(const std::string& line) {
    const Tokens tokens = tokenize(line, "{");
    const std::string id {tokens[0]};

    const Tokens conditionTokens = tokenize(tokens[1], ",");
    std::vector<Condition> conditions;
    for (T i = 0; i < conditionTokens.size() - 1; ++i) {
        const std::string_view t = conditionTokens[i];
        const size_t colon = t.find(':');
        const T value{std::stoull(toString(t.substr(2, colon - 2)))};
        const std::string flowId{t.substr(colon + 1)};

        conditions.push_back(Condition {.param = t[0], .op = t[1], .value = value, .flowId = flowId});
    }

    // the last condition is a default workflow. Use it directly but without the '}'
    const std::string_view last = conditionTokens.back();
    const std::string defaultWorkflow {last.substr(0, last.size() - 1)};

    return Workflow {id, std::move(conditions), defaultWorkflow};
}

Part parsePart(const std::string_view line) {
    const std::string_view sub = line.substr(1, line.size() - 2);
    const Tokens tokens = tokenize(sub, ",");
    const T x = std::stoll(toString(tokens[0].substr(2)));
    const T m = std::stoll(toString(tokens[1].substr(2)));
    const T a = std::stoll(toString(tokens[2].substr(2)));
    const T s = std::stoll(toString(tokens[3].substr(2)));

    return Part{.x=x, .m=m, .a=a, .s=s};
}

std::pair<Workflows, Parts> parse(const Lines& lines) {
    Workflows workflows;
    Parts parts;

    T it = 0;
    while(!lines[it].empty()) {
        Workflow workflow = parseWorkflow(lines[it]);
        workflows[workflow.id] = std::move(workflow);
        ++it;
    }

    while(++it < lines.size()) {
        Part part = parsePart(lines[it]);
        parts.push_back(std::move(part));
    }

    return std::make_pair(workflows, parts);
};

const T& at(const Part& part, char param) {
    if (param == 'x') return part.x;
    if (param == 'm') return part.m;
    if (param == 'a') return part.a;
    return part.s;
}

bool accept(const Workflows& workflows, const Part& part, std::string workflow) {
    const auto op = [](char o, T lhs, T rhs) {
        if (o == '>') {
            return std::greater<T>()(lhs, rhs);
        }
        return std::less<T>()(lhs, rhs);
    };
    while (1) {
        if (workflow == "A") {
            return true;
        } else if (workflow == "R") {
            return false;
        }

        const Workflow& w = workflows.at(workflow);

        bool handled = false;
        for (const Condition& c: w.conditions) {
            const T& paramValue = at(part, c.param);
            if (op(c.op, paramValue, c.value)) {
                workflow = c.flowId;
                handled = true;
                break;
            }
        }

        if (!handled) {
            workflow = w.fallback;
        }
    }
    return false;
}

void solvePart1(const Workflows& workflows, const Parts& parts, const std::string& workflow) {
    T total = 0;

    for (const Part& p: parts) {
        if (accept(workflows, p, workflow)) {
            const T partTotal = p.x + p.m + p.a + p.s;
            total += partTotal;

            // std::cout << std::format("\taccepted: {},{},{},{}: {}\n", p.x, p.m, p.a, p.s, partTotal);
        }
    }

    std::cout << std::format("Part 1: {}\n", total);
}

T count(const Workflows& workflows, PartRange partRange, const std::string& workflow, const std::string& space = " ") {
    if (workflow == "A") {        
        T product = 1;
        for (auto range: partRange) {
            product *= range.second.max - range.second.min + 1;
        }
        // std::cout << std::format("{}Accept {}\n", space, product);
        return product;
    } else if (workflow == "R") {
        // std::cout << std::format("{}Reject\n", space);
        return 0;
    }

    const Workflow& w = workflows.at(workflow);
    bool handled = false;

    T total = 0;
    for (const Condition& c: w.conditions) {
        const Range& r = partRange.at(c.param);
        Range trueBranch;
        Range falseBranch;
        if (c.op == '<') {
            trueBranch = Range {r.min, c.value - 1};
            falseBranch = Range {c.value, r.max};
        } else {
            trueBranch = Range {c.value + 1, r.max};
            falseBranch = Range {r.min, c.value};
        }

        // std::cout << std::format("{}Handling {}: {} {} {} ({} {}) ({} {}) ({} {})", space, w.id, c.param, c.op, c.value, r.min, r.max, trueBranch.min, trueBranch.max, falseBranch.min, falseBranch.max);
        if (trueBranch.min <= trueBranch.max) {
            // std::cout << " true";
        }
        if (falseBranch.min <= falseBranch.max) {
            // std::cout << " false" << std::endl;
        }


        if (trueBranch.min <= trueBranch.max) {
            PartRange trueRange = partRange;
            trueRange[c.param] = trueBranch;
            total += count(workflows, trueRange, c.flowId, space + " ");
        } 
        if (falseBranch.min <= falseBranch.max) {
            partRange[c.param] = falseBranch;
        } else {
            handled = true;
            break;
        }
    }

    if (!handled) {
        total += count(workflows, partRange, w.fallback, space + " ");
    }

    return total;
}

void solvePart2(const Workflows& workflows, std::string inWorkflow) {
    const PartRange range = {{'x', {1, 4000}}, {'m',{1, 4000}}, {'a',{1, 4000}}, {'s',{1, 4000}}};

    const T total = count(workflows, range, "in");
    std::cout << std::format("Part 2: {}\n", total);
}

void solve(const char* filename) {
    const Lines& lines = readFile(filename);

    const auto [workflows, parts] = parse(lines);
    solvePart1(workflows, parts, "in");
    solvePart2(workflows, "in");
}

void test() {
    using namespace boost::ut;

    "Parsing workflows"_test = [] {
        const Workflow workflow = parseWorkflow("px{a<2006:qkq,m>2090:A,rfg}");
        expect(workflow.id == "px");
        expect(workflow.conditions.size() == 2_i);
        expect(workflow.conditions[0].param == 'a');
        expect(workflow.conditions[0].value == 2006_i);
        expect(workflow.conditions[0].flowId == "qkq");
        expect(workflow.conditions[1].param == 'm');
        expect(workflow.conditions[1].value == 2090_i);
        expect(workflow.conditions[1].flowId == "A");
        expect(workflow.fallback == "rfg");
    };

    "Parsing parts"_test = [] {
        const Part part = parsePart("{x=787,m=2655,a=1222,s=2876}");
        expect(part.x == 787_i);
        expect(part.m == 2655_i);
        expect(part.a == 1222_i);
        expect(part.s == 2876_i);
    };

    "Count on workflow with no condition should just return the range"_test = [] {
        const Workflows workflows { {"in", Workflow{.id = "in", .conditions = {}, .fallback = "A"}} };
        const PartRange range = {{'x', {1, 4000}}, {'m',{1, 4000}}, {'a',{1, 4000}}, {'s',{1, 4000}}};
    
        const T total = count(workflows, range, "in");
        expect(total == std::pow(4000, 4)); // 
    };

    "Count is 0 if range is rejected"_test = [] {
        const Workflows workflows { {"in", Workflow{.id = "in", .conditions = {}, .fallback = "R"}} };
        const PartRange range = {{'x', {1, 4000}}, {'m',{1, 4000}}, {'a',{1, 4000}}, {'s',{1, 4000}}};
    
        const T total = count(workflows, range, "in");
        expect(total == 0);
    };

    "Count is the range length if the true branch accepts and fallback rejects"_test = [] {
        const Workflows workflows { {"in", 
            Workflow{.id = "in", .conditions = {Condition{.param = 'x', .op = '<', .value = 1000, .flowId = "A"}}, .fallback = "R"}} };
        const PartRange range = {{'x', {1, 4000}}, {'m',{1, 4000}}, {'a',{1, 4000}}, {'s',{1, 4000}}};
    
        const T total = count(workflows, range, "in");
        const T rangeLength = 1000 - 1;
        expect(total == rangeLength * std::pow(4000, 3));
    };
  
    "Count is 0 if the true branch rejects and fallback rejects"_test = [] {
        const Workflows workflows { {"in", 
            Workflow{.id = "in", .conditions = {Condition{.param = 'x', .op = '<', .value = 1000, .flowId = "R"}}, .fallback = "R"}} };
        const PartRange range = {{'x', {1, 4000}}, {'m',{1, 4000}}, {'a',{1, 4000}}, {'s',{1, 4000}}};
    
        const T total = count(workflows, range, "in");
        expect(total == 0_i);
    };

    "Count is the false branch length if condition fails and fallback accepts"_test = [] {
        const Workflows workflows { {"in", 
            Workflow{.id = "in", .conditions = {Condition{.param = 'x', .op = '<', .value = 10, .flowId = "R"}}, .fallback = "A"}} };
        const PartRange range = {{'x', {1, 40}}, {'m',{1, 40}}, {'a',{1, 40}}, {'s',{1, 40}}};
    
        const T total = count(workflows, range, "in");
        const T expected = 31*40*40*40;
        expect(total == expected);
    };
}

int main(int argc, const char** argv) {
    test();
    const char* filename = argc > 1 ? argv[1] : "day19.sample";
    solve(filename);
}