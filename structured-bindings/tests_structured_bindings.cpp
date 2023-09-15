#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <string>
#include <tuple>
#include <vector>
#include <array>

using namespace std;

namespace LegacyCode
{
    void calc_stats(const vector<int>& data, int& min, int& max, double& avg)
    {
        vector<int>::const_iterator min_pos, max_pos;
        tie(min_pos, max_pos) = minmax_element(data.begin(), data.end());

        min = *min_pos;
        max = *max_pos;
        avg = accumulate(data.begin(), data.end(), 0.0) / data.size();
    }
}

tuple<int, int, double> calc_stats(const vector<int>& data)
{
    // vector<int>::const_iterator min_pos, max_pos;
    // tie(min_pos, max_pos) = minmax_element(data.begin(), data.end());

    auto [min_pos, max_pos] = minmax_element(data.begin(), data.end());

    double avg = accumulate(data.begin(), data.end(), 0.0) / data.size();

    return make_tuple(*min_pos, *max_pos, avg);
}

TEST_CASE("std::tuple")
{
    std::tuple<int, double> tpl_1{ 42, 3.14 };
    std::tuple<int, double, std::string, std::vector<int>> tpl_2{ 42, 3.14, "text"s, std::vector{1, 2, 3} };

    auto tpl_3 = std::make_tuple(1, 3.14, "text", "text"s); // std::tuple<int, double, const char*, std::string>

    SECTION("Since C++17")
    {
        std::tuple tpl{ 1, 3.14, "text", "text"s }; // std::tuple<int, double, const char*, std::string>
    }

    SECTION("getting item")
    {
        std::tuple<int, double, std::string, std::vector<int>> tpl{ 42, 3.14, "text"s, std::vector{1, 2, 3} };

        CHECK(std::get<0>(tpl) == 42);
        CHECK(std::get<3>(tpl) == std::vector{ 1, 2, 3 });

        std::get<0>(tpl) = 665;
    }

    SECTION("tuple of refs")
    {
        int x = 42;
        std::string str = "text";

        std::tuple<int&, std::string&> tpl_refs{ x, str };

        std::get<0>(tpl_refs) = 665;

        CHECK(x == 665);

        tpl_refs = std::make_tuple(100, "new text");
        // lhs: std::tuple<int&, std::string&>
        // rhs: std::tuple<int, const char*>

        SECTION("tie")
        {
            auto tpl_refs = std::tie(x, str); // tie creates ref tuple: std::tuple<int&, std::string&>

            std::tie(x, str) = std::tuple{ 200, "some text" };
        }
    }
}

TEST_CASE("Before C++17")
{
    vector<int> data = { 4, 42, 665, 1, 123, 13 };

    SECTION("using tie - before C++17")
    {
        int min, max;
        double avg;

        tie(min, max, avg) = calc_stats(data);

        REQUIRE(min == 1);
        REQUIRE(max == Catch::Approx(665));
        REQUIRE(avg == Catch::Approx(141.333));
    }

    SECTION("structed bindings - before C++17")
    {
        auto [min, max, avg] = calc_stats(data);

        REQUIRE(min == 1);
        REQUIRE(max == Catch::Approx(665));
        REQUIRE(avg == Catch::Approx(141.333));
    }
}

std::array<int, 3> get_coordinates() 
{ 
    return std::array{1, 2, 3}; 
}

struct Timestamp
{
    int h, m, s;
};

TEST_CASE("Structured bindings")
{
    SECTION("native arrays")
    {
        int coords[2] = { 10, 20 };

        auto& [x, y] = coords;

        x = 42;

        CHECK(coords[0] == 42);
    }

    SECTION("std::array")
    {
        const auto [x, y, z] = get_coordinates();
    }

    SECTION("struct/class")
    {
        const auto& [hours, minutes, seconds] = Timestamp{1, 30, 0};

        SECTION("it works like this")
        {
            const auto& entity = Timestamp{1, 30, 0};

            auto&& hours = entity.h;
            auto&& minutes = entity.m;
            auto&& seconds = entity.s;
        }
    }
}

TEST_CASE("iterating over map")
{
    std::map<int, std::string> dict = { {1, "one"}, {2, "two"}, {3, "three"} };

    for(const auto& [key, value] : dict)
    {
        std::cout << key << " - " << value << "\n";
    }   
}

struct Gadget
{
    std::string name;
    int price;

    auto tied() const
    {
        return std::tie(name, price);
    }

    bool operator==(const Gadget& other) const
    {
        return tied() == other.tied();
    }

    bool operator<(const Gadget& other) const
    {
        return tied() < other.tied();
    }
};

TEST_CASE("comparisons")
{
    Gadget g{"ipad", 100};

    CHECK(g == Gadget{"ipad", 100});
    CHECK(g < Gadget{"ipod", 100});
}