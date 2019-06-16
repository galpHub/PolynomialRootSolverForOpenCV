#include <opencv2/core/core.hpp>
#include <iostream>
#include "tests/allTests.h"
using namespace cv;
int main(int argc, char *argv[])
{
    std::cout << "Running tests!" << std::endl;
    return UnitTest::RunAllTests();
}



