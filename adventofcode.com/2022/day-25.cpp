#include "utils.h"


void solve(const char* filename) {
    const Lines& lines = readFile(filename);
}

void test() {

}

int main(int argc, char* argv[])
{
    test();

    const char* filename = argc > 1 ? argv[1] : "day-25.sample";
    solve(filename);
}