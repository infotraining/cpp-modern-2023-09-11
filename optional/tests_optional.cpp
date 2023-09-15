#include <algorithm>
#include <array>
#include <atomic>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <charconv>
#include <iostream>
#include <numeric>
#include <optional>
#include <string>
#include <vector>

using namespace std;

TEST_CASE("optional")
{
    std::optional<int> opt_int;
    CHECK(opt_int.has_value() == false);

    opt_int = 42;
    CHECK(opt_int.has_value() == true);

    opt_int.reset();
    opt_int = std::nullopt;

    std::optional opt_double = 3.14; // CTAD: std::optional<double>

    SECTION("boost::optional vs. std::optional")
    {
        //boost::optional<int&> opt_int_ref;

        int x = 42;

        std::optional<std::reference_wrapper<int>> opt_int_ref = x;

        opt_int_ref = std::nullopt;
    }
}

TEST_CASE("optional API")
{
    std::optional<int> opt_int;

    SECTION("pointer API")
    {
        if (opt_int)
        {
            std::cout << *opt_int << "\n";
        }
    }

    SECTION("safer API")
    {
        CHECK_THROWS_AS(opt_int.value(), std::bad_optional_access);

        opt_int = 42;

        CHECK(opt_int.value() == 42);
    }
}

TEST_CASE("optional comparison ops")
{
    std::optional<int> opt_number = 42;

    CHECK(opt_number == 42);
}