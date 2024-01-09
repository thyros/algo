#include "pointL.h"
#include "utils.h"
#include "ut.hpp"
#include <format>
#include <unordered_map>

using T = long long;

struct Data {
    char dir;
    T len;
    std::string color;
};
using Datas = std::vector<Data>;
using Graph = std::vector<Point>;

Datas parse(const Lines& lines) {
    Datas datas;

    for (const std::string& line: lines) {
        Tokens tokens = tokenize(line);
        datas.emplace_back(Data {tokens[0][0], atoi(toString(tokens[1]).c_str()), toString(tokens[2])} );
    }

    return datas;
}

Point dirToOffset(const char c) {
    if (c == 'R') return Point {1, 0};
    if (c == 'L') return Point {-1, 0};
    if (c == 'U') return Point {0, -1};
    return Point {0, 1};
}

Graph toGraph(const Datas& datas) {
    Point c {0, 0};
    Graph result;

    for (const Data& data: datas) {
        const Point offset = dirToOffset(data.dir);
        c.x += offset.x * data.len;
        c.y += offset.y * data.len;

        result.push_back(c);
    }

    return result;
}

Graph toGraphPart2(const Datas& datas) {
    const std::vector<Point> offsets {{1,0}, {0, 1}, {-1, 0}, {0, -1}};
    Point c {0, 0};

    Graph result ;
    for (const Data& data: datas) {
        const Point offset = offsets[data.color[7] - '0'];
        const std::string length = data.color.substr(2, 5);
        const T len = std::stoll(length, nullptr, 16);

        c.x += offset.x * len;
        c.y += offset.y * len;

        result.push_back(c);
    }

    return result;
}


T solvePart1(const Datas& datas) {
    const Graph graph = toGraph(datas);
    T area = 0;
    T edge = 0;
    for (T i = 0; i < graph.size(); ++i) {
        const Point& p1 = graph[i - 1 >= 0 ? i - 1 : graph.size() - 1];
        const Point& p2 = graph[i + 1 < graph.size() ? i + 1 : 0];
        const Point& p = graph[i];

        const T t = p.y * (p1.x - p2.x);
        area += t;

        edge += std::abs(p.x - p1.x) + std::abs(p.y - p1.y);
    }
    area = std::abs(area) / 2;
    const T inside = area - edge / 2 + 1;
    const T result = inside + edge;

    std::cout << std::format("part1: e: {}, a: {}, i: {}, r: {}\n", edge, area, inside, result);
    return inside + edge;
}

T solvePart2(const Datas& datas) {

    const Graph graph = toGraphPart2(datas);
    T area = 0;
    T edge = 0;
    for (T i = 0; i < graph.size(); ++i) {
        const Point& p1 = graph[i - 1 >= 0 ? i - 1 : graph.size() - 1];
        const Point& p2 = graph[i + 1 < graph.size() ? i + 1 : 0];
        const Point& p = graph[i];

        const T t = p.y * (p1.x - p2.x);
        area += t;

        edge += std::abs(p.x - p1.x) + std::abs(p.y - p1.y);
    }
    area = std::abs(area) / 2;
    const T inside = area - edge / 2 + 1;
    const T result = inside + edge;

    std::cout << std::format("part2: e: {}, a: {}, i: {}, r: {}\n", edge, area, inside, result);
    return inside + edge;
}

void solve(const char* filename) {
    const Lines& lines = readFile(filename);
    const Datas datas = parse(lines);
    const T area1 = solvePart1(datas);
    std::cout << std::format("Part 1: total area: {}\n", area1);
    const T area2 = solvePart2(datas);
    std::cout << std::format("Part 2: total area: {}\n", area2);
}

void test() {
    using namespace boost::ut;


}

int main(int argc, const char** argv) {
    test();
    const char* filename = argc > 1 ? argv[1] : "day18.sample";
    solve(filename);
}