#include "utils.h"
#include "ut.hpp"

#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

using T = long long;

struct Node {
    Node* left = nullptr;
    Node* right = nullptr;
    T value = 0;
};

using Buffer = std::vector<Node>;

void print(const Buffer& buffer) {
    const Node* head = &buffer[0];
    const Node* temp = head;
    do {
        printf("%3lli ", temp->value);
        temp = temp->right;
    } while(head != temp);
    printf("\n");
}

void rotate(Node& node, int mod) {
    const auto v = node.value;
    const auto c = std::abs(v) % mod;

    if (c == 0) {
        return;
    }

    Node* temp = &node;
    node.right->left = node.left;
    node.left->right = node.right;

    if (v > 0) {
        for (int i = 0; i < c; ++i)
            temp = temp->right;

        temp->right->left = &node;
        node.right = temp->right;
        temp->right = &node;
        node.left = temp;

    } else {
        for (int i = 0; i < c; ++i)
            temp = temp->left;

        temp->left->right = &node;
        node.left = temp->left;
        temp->left = &node;
        node.right = temp;
    }

}

Node* advance(Node* node, int c) {
    for (int i = 0; i < c; ++i) {
        node = node->right;
    }
    return node;
}

void part1(const Lines& lines) {
    const int m = lines.size();
    Buffer buffer(lines.size(), Node {});
    for (int i = 0; i < lines.size(); ++i) {
        buffer[i].value = std::stoi(lines[i]);
        buffer[i].right = &buffer[(i + 1) % m];
        buffer[i].left = &buffer[(i - 1 + m)%m];
    }

    const int width = buffer.size();

    Node* zero = nullptr;
    for (int i = 0; i < buffer.size(); ++i) {
        Node& node = buffer[i];
        if (node.value == 0) {
            zero = &node;
            continue;
        }

        rotate(node, width - 1);
    }
    const auto v1 = advance(zero, 1000)->value;
    const auto v2 = advance(zero, 2000)->value;
    const auto v3 = advance(zero, 3000)->value;

    printf("Part 1: %lli = %lli + %lli + %lli\n", v1+v2+v3, v1, v2, v3);
}

void part2(const Lines& lines) {
    const long long magicNumber = 811589153;
    const int m = lines.size();
    Buffer buffer(lines.size(), Node {});
    for (int i = 0; i < lines.size(); ++i) {
        buffer[i].value = std::stoll(lines[i]) * magicNumber;
        buffer[i].right = &buffer[(i + 1) % m];
        buffer[i].left = &buffer[(i - 1 + m)%m];
    }

    const int width = buffer.size();

    Node* zero = nullptr;
    for (int a = 0; a < 10; ++a) {
        for (int i = 0; i < buffer.size(); ++i) {
            Node& node = buffer[i];
            if (node.value == 0) {
                zero = &node;
                continue;
            }

            rotate(node, width - 1);
        }
    }
    const auto v1 = advance(zero, 1000)->value;
    const auto v2 = advance(zero, 2000)->value;
    const auto v3 = advance(zero, 3000)->value;

    printf("Part 2: %lli = %lli + %lli + %lli\n", v1+v2+v3, v1, v2, v3);
}

void solve(const char* filename) {
    const Lines lines = readFile(filename);
    part1(lines);
    part2(lines);
}

void test()
{
    using namespace boost::ut;

}

int main(int argc, char* argv[])
{
    test();

    const char* filename = argc > 1 ? argv[1] : "day-20.input";
    solve(filename);
}