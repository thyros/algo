#include "utils.h"
#include "ut.hpp"
#include <deque>
#include <format>
#include <functional>
#include <numeric>
#include <unordered_map>

using T = long long;

enum class Type {
    Flip,
    Conj,
    Broad
};

struct Module {
    std::string name;
    Type type;
    bool state = false;
    std::vector<std::string> outputs;
    std::unordered_map<std::string, bool> inputs;
};
using Modules = std::unordered_map<std::string, Module>;

struct Message {
    std::string origin;
    std::string target;
    bool pulse;
};
using Queue = std::deque<Message>;

Modules parse(const Lines& lines) {
    Modules modules;
    
    for (const std::string& line: lines) {
        const Tokens t = tokenize(line, " -> ");
        const std::string_view left = t[0];
        const std::string_view right = t[1];

        Module m;
        if (left[0] == '&') {
            m.name = left.substr(1);
            m.type = Type::Conj;
        } else if (left[0] == '%') {
            m.name = left.substr(1);
            m.type = Type::Flip;
        } else {
            m.name = left;
            m.type = Type::Broad;
        }

        for (const std::string_view t:  tokenize(right, ", ")) {
            m.outputs.push_back(toString(t));
        }

        modules[m.name] = m;
    }

    for (const auto& m: modules) {
        for (const std::string& output: m.second.outputs) {
            modules[output].inputs[m.first] = false;
        }
    }

    return modules;
}

void send(Queue& queue, Module& m, const std::string& origin, bool pulse) {
    switch(m.type) {
        case Type::Broad:
            for (const std::string& target: m.outputs) {
                queue.push_back({m.name, target, pulse});
            }
            break;

        case Type::Flip:
            if (!pulse) {
                m.state = !m.state;
                for (const std::string& target: m.outputs) {
                    queue.push_back({m.name, target, m.state});
                }
            }
            break;

        case Type::Conj:
            m.inputs[origin] = pulse;
            m.state = std::any_of(begin(m.inputs), end(m.inputs), [](const auto& it) { return it.second == false; });

            for (const std::string& target: m.outputs) {
                queue.push_back({m.name, target, m.state});
            }
    }
}

void solvePart1(Modules modules) {
    Queue queue;
    T high = 0;
    T low = 0;

    for (int i = 0; i < 1000; ++i) {
        queue.push_back({"button", "broadcaster", false});
        while (!queue.empty()) {
            const Message message = queue.front();
            queue.pop_front();

            if (message.pulse == true) {
                ++high;
            } else {
                ++low;
            }

            send(queue, modules[message.target], message.origin, message.pulse);
        }
    }

    std::cout << std::format("Part 1: lo: {}, high: {}, total: {}\n", low, high, low * high);
}

void solvePart2(Modules modules) {
    Queue queue;
    T presses = 0;

    // module that sends to rx. It is a conj module
    const std::string& mToRx = modules["rx"].inputs.begin()->first;

    std::unordered_map<std::string, T> previous;
    std::unordered_map<std::string, T> loops;
    std::vector<T> lengths;

    bool done = false;
    while (!done) {
        ++presses;
        queue.push_back({"button", "broadcaster", false});
        
        while (!queue.empty()) {
            const Message message = queue.front();
            queue.pop_front();

            send(queue, modules[message.target], message.origin, message.pulse);

            if (message.target == mToRx && message.pulse) {
                if (!loops.contains(message.origin)) {
                    if (previous.contains(message.origin)) {
                        loops[message.origin] = presses - previous[message.origin];
                        lengths.push_back(loops[message.origin]);

                        if (lengths.size() == modules[mToRx].inputs.size()) {
                            done = true;
                        }
                    } else {
                        previous[message.origin] = presses;
                    }
                }
            }
        }
    }

    T lcm = lengths[0];
    for (int i = 1; i < lengths.size(); ++i) {
        lcm = std::lcm(lcm, lengths[i]);
    }

    std::cout << std::format("Part 2: {} min number of button presses\n", lcm);
}

void solve(const char* filename) {
    const Lines& lines = readFile(filename);

    const Modules modules = parse(lines);
    solvePart1(modules);
    solvePart2(modules);
}

void test() {
    using namespace boost::ut;

    ""_test = [] {
    };
}

int main(int argc, const char** argv) {
    test();
    const char* filename = argc > 1 ? argv[1] : "day20.sample";
    solve(filename);
}