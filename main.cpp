#include <iostream>

#include "speed_tests.h"
#include "tests.h"

int main() {
    grobner::Tests::test_all();
    std::cout << "All tests passed\n";
    grobner::SpeedTests::run_tests();
    return 0;
}
