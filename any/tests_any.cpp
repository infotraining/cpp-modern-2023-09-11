#include <algorithm>
#include <any>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

TEST_CASE("any")
{
    std::any anything;

    CHECK(anything.has_value() == false);

    anything = 42;
    anything = "text"s;
    anything = 3.14;
    anything = std::vector{1, 2, 3};

    CHECK(std::any_cast<std::vector<int>>(anything) == std::vector{1, 2, 3}); // the copy of vector is returned    
    CHECK_THROWS_AS(std::any_cast<double>(anything), std::bad_any_cast); 

    std::vector<int>* ptr_vec = std::any_cast<std::vector<int>>(&anything);

    if (ptr_vec)
    {
        CHECK(ptr_vec->size() == 3);
    }
}