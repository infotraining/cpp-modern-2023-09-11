#include <catch2/catch_test_macros.hpp>
#include <deque>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "gadget.hpp"


std::string full_name(const std::string& first_name, const std::string& last_name)
{
    return first_name + " " + last_name;
}

TEST_CASE("reference binding")
{
    std::string name = "john";

    SECTION("C++98")
    {
        std::string& ref_name = name;

        const std::string& ref_full_name = full_name(name, "doe");
    }

    SECTION("C++11")
    {
        std::string&& rv_ref_full_name = full_name(name, "doe");

        rv_ref_full_name[0] = 'J'; // OK

        // std::string&& rv_ref_name = name; // ERROR

        // std::string&& rv_ref_other = rv_ref_full_name; // rv_ref_full_name - lvalue
    }
}


TEST_CASE("moving objects")
{
    std::string name = "john";

    std::string name_cc = name;  // copy constructor called
    std::string name_cc_2(name); // copy constructor called

    REQUIRE(name_cc == "john");

    std::string target = std::move(name); // move contructor called

    name = "eva";

    REQUIRE(target == "john");

    std::string full = full_name("john", "doe"); // move constructed (C+11 - int the worst scenario)
}
