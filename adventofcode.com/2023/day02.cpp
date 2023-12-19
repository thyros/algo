#include "utils.h"
#include "ut.hpp"
#include <istream>
#include <string_view>

struct Color {
    int red = 0;
    int green = 0;
    int blue = 0;
};

std::string to_string(const Color& color) {
    return "r=" + std::to_string(color.red) + " g=" + std::to_string(color.green) + " b=" + std::to_string(color.blue);
}

long long power(const Color& color) {
    return color.red * color.green * color.blue;
}

Color parseDraw(std::string_view text) {
    const Tokens tokens = tokenize(text, ",");

    Color draw;

    for (const std::string_view token: tokens) {
        std::istringstream istr(toString(token));

        int amount;
        std::string color;

        istr >> amount;
        istr >> color;

        if (color == "red") {
            draw.red = amount;
        } else if (color == "green") {
            draw.green = amount;
        } else if (color == "blue") {
            draw.blue = amount;
        }
    }
    return draw;
}

bool isDrawPossible(const Color& draw, const Color& max) {
    return draw.red <= max.red && draw.green <= max.green && draw.blue <= max.blue;
}

bool isGamePossible(std::string_view text, const Color& max) {
    const size_t colon = text.find(':');

    const Tokens draws = tokenize2(text, colon + 1, std::string_view::npos, ";");
    for (const std::string_view drawText: draws) {

        const Color draw = parseDraw(drawText);
        if (!isDrawPossible(draw, max))
            return false;
    }
    return true;
}

Color getMax(const Color& left, const Color& right) {
    return Color {.red = std::max(left.red, right.red),
        .green = std::max(left.green, right.green),
        .blue = std::max(left.blue, right.blue)};
}

Color getMaxFromGame(std::string_view text) {
    const size_t colon = text.find(':');

    const Tokens draws = tokenize2(text, colon + 1, std::string_view::npos, ";");
    Color min;
    for (const std::string_view drawText: draws) {
        const Color draw = parseDraw(drawText);
        min = getMax(min, draw);
    }
    return min;
}

void solve(const char* filename) {
    const Lines lines = readFile(filename);
    const Color max {.red = 12, .green = 13, .blue = 14};

    int sum = 0;
    long long powerOfSets = 0;
    for (size_t i = 0; i < lines.size(); ++i) { 
        if (isGamePossible(lines[i], max)) {
            sum += i + 1;
        }

        const Color colorsCount = getMaxFromGame(lines[i]);
        powerOfSets += power(colorsCount); 
    }
    std::cout << "Sum = " << sum << std::endl;
    std::cout << "Power: " << powerOfSets << std::endl;
}

void test() {
    using namespace boost::ut;

    "When parsing 3 colors all colors are processed"_test = [] {
        const Color draw = parseDraw("31 blue, 16 green, 44 red");
        expect(44_i == draw.red);
        expect(16_i == draw.green);
        expect(31_i == draw.blue);
    };    

    "When parsing 2 colors, missing colors are 0"_test = [] {
        expect(0_i == parseDraw("31 blue, 16 green").red);
        expect(0_i == parseDraw("31 blue, 44 red").green);
        expect(0_i == parseDraw("16 green, 44 red").blue);
    };

}

int main(int argc, const char** argv) {
    test();
    const char* filename = argc > 1 ? argv[1] : "day02.sample";
    solve(filename);
}