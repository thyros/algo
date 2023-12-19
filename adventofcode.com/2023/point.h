#pragma once

#include <iostream>
#include <functional>

struct Point {
    int x {0};
    int y {0};
    auto operator<=>(const Point&) const = default;
};

template<>
struct std::hash<Point> {
    std::size_t operator()(const Point& p) const noexcept
    {
        constexpr int MAX_VALUE = 1610612741;
        return ((p.x+MAX_VALUE)<<16)|((p.y+MAX_VALUE)&0xFFFF);
    }
};

std::ostream & operator << (std::ostream &out, const Point &p)
{
    out << "(" << p.x << "," << p.y << ")";
    return out;
}