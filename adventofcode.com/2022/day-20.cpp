#include "utils.h"
#include "ut.hpp"
#include <algorithm>
#include <string>
#include <vector>

using Buffer = std::vector<int>;
struct Node {
    Node* left = nullptr;
    Node* right = nullptr;
    int value;
};

void print(const std::vector<int>& v) {
    for (int i: v) {
        printf("%i ", i);
    }
}


Buffer parse(const Lines& lines) {
    Buffer result(lines.size(), 0);
    for (int i = 0; i < lines.size(); ++i) {
        result[i] = std::stoi(lines[i]);
    }
    return result;
}

Node* createCircularBuffer(const Buffer& buffer) {
    Node* head = nullptr;
    Node* previous = nullptr;

    for (int i: buffer) {
        Node* current = new Node();
        current->left = previous;
        current->value = i;
        current->right = nullptr;

        if (!head) {
            head = current;
        }
        if (previous) {
            previous->right = current;
        }

        previous = current;
    }

    previous->right = head;
    head->left = previous;

    return head;
}

Node* findNode(Node* node, int v) {
    Node* head = node;

    do {
        if (node->value == v) {
            return node;
        }
        node = node->right;

    } while(node != head);

    return nullptr;
}

void print(Node* node) {
    Node* head = node;

    do {
        printf("%4i ", node->value);
        node = node->right;
    } while(node != head);
    printf("\n");
}

void rotate(Node* node, int width) {

    const int v = std::abs(node->value) % (width-1);
    printf("Rotating %i by %i, width %i\n", node->value, v, width);

    if (v == 0) {
        return;
    }

    node->left->right = node->right;
    node->right->left = node->left;

    Node* temp = node;
    for (int i = 0; i < v; ++i) {
        if (node->value > 0) temp = temp->right;
        else if (node->value < 0) temp = temp->left;
    }

    if (node->value > 0) {
        node->right = temp->right;
        node->left = temp;
    } else if (node->value < 0) {
        node->right = temp;
        node->left = temp->left;
    }

    node->right->left = node;
    node->left->right = node;
}

Node* advance(Node* node, int c, int width) {
    const int v = c % width;
    for (int i = 0; i < v; ++i) {
        node = node->right;
    }
    return node;
}

void part1(const Buffer& buffer) {

    Node* node = createCircularBuffer(buffer);
    Node* zero = findNode(node, 0);
    // print(zero);
    const int width = buffer.size();
    for (int i = 0; i < width; ++i) {
        const int v = buffer[i];
        Node* c = findNode(zero, v);
        rotate(c, width);
        // print(zero);
    }

    const int v1 = advance(zero, 1000, width)->value;
    const int v2 = advance(zero, 2000, width)->value;
    const int v3 = advance(zero, 3000, width)->value;

    printf("Part 1: %i = %i + %i + %i\n", v1+v2+v3, v1, v2, v3);
}

void solve(const char* filename) {
    const Lines lines = readFile(filename);
    const Buffer buffer = parse(lines);

    part1(buffer);
}

void test()
{
    using namespace boost::ut;

}

int main(int argc, char* argv[])
{
    test();

    const char* filename = argc > 1 ? argv[1] : "day-20.sample";
    solve(filename);
}