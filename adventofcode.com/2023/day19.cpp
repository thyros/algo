#include "utils.h"
#include "ut.hpp"
#include <format>
#include <functional>
#include <unordered_map>

using T = long long;

struct Condition {
    char param;
    T value;
    std::function<bool(T, T)> f;
    std::string id;
};

struct Workflow {
    std::string id;
    std::vector<Condition> conditions;
    std::string defaultWorkflow;
};
using Workflows = std::unordered_map<std::string, Workflow>;

struct Part {
    T x;
    T m;
    T a;
    T s;
};
using Parts = std::vector<Part>;

Workflow parseWorkflow(const std::string& line) {
    const Tokens tokens = tokenize(line, "{");
    const std::string id {tokens[0]};

    const Tokens conditionTokens = tokenize(tokens[1], ",");
    std::vector<Condition> conditions;
    for (T i = 0; i < conditionTokens.size() - 1; ++i) {
        const std::string_view t = conditionTokens[i];
        std::function<bool(T, T)> f;
        if (t[1] == '>') {
            f = std::greater<T>();
        } else {
            f = std::less<T>();
        }
        
        const size_t colon = t.find(':');
        const T value{std::stoll(toString(t.substr(2, colon - 2)))};
        const std::string defaultWorkflow{t.substr(colon + 1)};

        conditions.push_back(Condition {.param = t[0], .value = value, .f = f, .id = defaultWorkflow});
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

bool simulate(const Workflows& workflows, const Part& part, std::string workflow) {
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
            if (c.f(paramValue, c.value)) {
                workflow = c.id;
                handled = true;
                break;
            }
        }

        if (!handled) {
            workflow = w.defaultWorkflow;
        }
    }
    return false;
}

void solvePart1(const Workflows& workflows, const Parts& parts, const std::string& workflow) {
    T total = 0;

    for (const Part& p: parts) {
        if (simulate(workflows, p, "in")) {
            const T partTotal = p.x + p.m + p.a + p.s;
            total += partTotal;

            std::cout << std::format("\taccepted: {},{},{},{}: {}\n", p.x, p.m, p.a, p.s, partTotal);
        }
    }

    std::cout << std::format("part 1: {}\n", total);
}

void solve(const char* filename) {
    const Lines& lines = readFile(filename);

    const auto [workflows, parts] = parse(lines);
    solvePart1(workflows, parts, "in");
}

void test() {
    using namespace boost::ut;

    "Parsing workflows"_test = [] {
        const Workflow workflow = parseWorkflow("px{a<2006:qkq,m>2090:A,rfg}");
        expect(workflow.id == "px");
        expect(workflow.conditions.size() == 2_i);
        expect(workflow.conditions[0].param == 'a');
        expect(workflow.conditions[0].value == 2006_i);
        expect(workflow.conditions[0].id == "qkq");
        expect(workflow.conditions[1].param == 'm');
        expect(workflow.conditions[1].value == 2090_i);
        expect(workflow.conditions[1].id == "A");
        expect(workflow.defaultWorkflow == "rfg");
    };

    "Parsing parts"_test = [] {
        const Part part = parsePart("{x=787,m=2655,a=1222,s=2876}");
        expect(part.x == 787_i);
        expect(part.m == 2655_i);
        expect(part.a == 1222_i);
        expect(part.s == 2876_i);
    };
}

int main(int argc, const char** argv) {
    test();
    const char* filename = argc > 1 ? argv[1] : "day19.sample";
    solve(filename);
}