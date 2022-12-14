#pragma once

#include <functional>

struct Point {
    int x;
    int y;
};

template<>
struct std::hash<Point> {
    std::size_t operator()(const Point& p) const noexcept
    {
        constexpr int MAX_VALUE = 1000;
        return ((p.x+MAX_VALUE)<<16)|((p.y+MAX_VALUE)&0xFFFF);
    }
};

template<>
struct std::equal_to<Point> {
    bool operator()( const Point& lhs, const Point& rhs ) const {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }
};
