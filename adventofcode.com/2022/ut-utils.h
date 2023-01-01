#pragma once
#include <algorithm>

template <class C>
class sorted {
public:
    sorted(const C& c) : container(c) {
        std::sort(begin(container), end(container));
    }

    bool operator==(const sorted& other) const {
        return container == other.container;
    }

private:
    C container;
};
