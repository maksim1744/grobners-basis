#include "tests.h"

#include <iostream>

int main() {
    grobner::Tests::test_all();
    std::cout << "All tests passed\n";
    return 0;
}
