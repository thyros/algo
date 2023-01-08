#pragma once

#include <iostream>
#include <functional>

struct PointL {
    long long x {0};
    long long y {0};
    auto operator<=>(const PointL&) const = default;
};

template<>
struct std::hash<PointL> {
    std::size_t operator()(const PointL& p) const noexcept
    {
        constexpr long long MAX_VALUE = 1610612741;
        return ((p.x+MAX_VALUE)<<16)|((p.y+MAX_VALUE)&0xFFFF);
    }
};

std::ostream & operator << (std::ostream &out, const PointL &p)
{
    out << "(" << p.x << "," << p.y << ")";
    return out;
}