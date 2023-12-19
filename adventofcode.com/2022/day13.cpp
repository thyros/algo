#include "utils.h"
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <numeric>
#include <stack>
#include <vector>

enum class Result {
    InOrder,
    NotInOrder,
    DontKnow
};

struct X {
    bool number = false;

    int v = 0;
    std::vector<X*> sub;
};

Result inOrder(const X& l, const X& r);
Result inOrder(const std::vector<X*>& l, const std::vector<X*>& r);

Result inOrder(int l, int r) {
    if (l < r) {
        return Result::InOrder;
    } else if (l > r) {
        return Result::NotInOrder;
    }
    return Result::DontKnow;
}

Result inOrder(const std::vector<X*>& l, const std::vector<X*>& r) {
    for (int i = 0; i < std::max(l.size(), r.size()); ++i) {
        if (i == l.size()) {
            return Result::InOrder;
        } else if (i == r.size()) {
            return Result::NotInOrder;
        }

        const Result result = inOrder(*l[i], *r[i]);

        if (result == Result::InOrder) {
            return Result::InOrder;
        } else if (result == Result::NotInOrder) {
            return Result::NotInOrder;
        }
    }

    return Result::DontKnow;
}

Result inOrder(const X& l, const X& r) {
    if (!l.number && !r.number) {
        return inOrder(l.sub, r.sub);
    } else if (!l.number) {
        std::vector<X*> newR;
        newR.push_back(new X{.number = true, .v = r.v});
        return inOrder(l.sub, newR);
    } else if (!r.number) {
        std::vector<X*> newL;
        newL.push_back(new X {.number = true, .v = l.v});
        return inOrder(newL, r.sub);
    }

    return inOrder(l.v, r.v);
}

X* toX(const std::string& line) {
    // [1,[2,[3,[4,[5,6,7]]]],8,9]

    X* result = new X;
    std::stack<X*> stack;
    stack.push(result);

    for (int i = 0; i < line.size(); /* do nothing */) {
        X* currentX = stack.top();

        if (line[i] == '[') {
            // parse array
            currentX->sub.push_back(new X{ .number = false });
            stack.push(currentX->sub.back());

            ++i;
        } else if (line[i] == ']') {
            // finish array
            stack.pop();
            ++i;
        } else if (line[i] >= '0' && line[i] <= '9') {
            // parse number
            const auto after = line.find_first_not_of("0123456789", i);
            const std::string number {begin(line) + i, begin(line) + after};

            currentX->sub.push_back(new X{.number = true, .v = std::stoi(number)});
            i = after;
        } else {
            assert(line[i] == ',' && "all other cases are already covered");
            ++i;
        }
    }

    return result;
}

std::vector<X*> toX(const Lines& lines) {

    std::vector<X*> result;

    for (int i = 0; i < lines.size(); i += 3) {
        result.push_back(toX(lines[i]));
        result.push_back(toX(lines[i + 1]));
    }
    return result;
}

void print(const X& x) {
    if (x.number) {
        printf(" %i", x.v);
    } else {
        printf("[");
        for (const auto sub: x.sub) {
            print(*sub);
        }
        printf("]");
    }
}

void part1(const std::vector<X*> xs) {
    std::vector<int> indices;
    for (int i = 0; i < xs.size(); i += 2) {
        if (inOrder(*xs[i], *xs[i+1]) == Result::InOrder) {
            indices.push_back(i / 2 + 1);
        }
    }
    for (int i: indices) {
        printf(" %i\n", i);
    }

    printf("Part 1: %i", std::accumulate(begin(indices), end(indices), 0));
}

void part2(std::vector<X*> xs) {
    std::sort(begin(xs), end(xs), [](const X *const l, const X* const r) -> bool { return inOrder(*l, *r) == Result::InOrder; });

    // for (const auto p: xs) {
    //     print(*p);
    //     printf("\n");
    // }

    auto* firstDivider = toX("[[2]]");
    int first = 0;
    for (first = 0; first < xs.size(); ++first) {
        if (inOrder(*xs[first], *firstDivider) == Result::NotInOrder) {
            break;
        }
    }

    auto* secondDivider = toX("[[6]]");
    int second = 0;
    for (second = first + 1; second < xs.size(); ++second) {
        if (inOrder(*xs[second], *secondDivider) == Result::NotInOrder) {
            break;
        }
    }

    first += 1;
    second += 2;

    printf("Part 2: %i %i -> %i", first, second, first * second);
}

void solve(const char* filename) {
    const Lines lines = readFile(filename);
    const std::vector<X*> xs = toX(lines);

    // part1(xs);
    part2(xs);

    // for (const auto& p: input) {
    //     print(*p.first);
    //     printf("\n");
    //     print(*p.second);
    //     printf("\n\n");
    // }


}

void test() {

}

int main() {
    test();

    constexpr char filename[] { "day13.input" };
    solve(filename);
}