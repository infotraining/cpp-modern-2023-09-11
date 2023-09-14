#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>

#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <vector>

using namespace std;

TEST_CASE("lambda exercise")
{
    using namespace Catch::Matchers;

    vector<int> data = { 1, 6, 3, 5, 8, 9, 13, 12, 10, 45 };
    auto is_even = [](int n) { return n % 2 == 0; };

    SECTION("count even numbers")
    {
        auto evens_count = std::count_if(data.begin(), data.end(), is_even);

        REQUIRE(evens_count == 4);
    }

    SECTION("copy evens to vector")
    {
        vector<int> evens;

        std::copy_if(data.begin(), data.end(), std::back_inserter(evens), is_even);

        REQUIRE_THAT(evens, Equals(vector<int>{6, 8, 12, 10}));
    }

    SECTION("create container with squares")
    {
        vector<int> squares(data.size());

        std::transform(data.begin(), data.end(), squares.begin(), [](int x) { return x * x; });

        REQUIRE_THAT(squares, Equals(vector<int>{1, 36, 9, 25, 64, 81, 169, 144, 100, 2025}));
    }

    SECTION("remove from container items divisible by any number from a given array")
    {
        array<int, 3> eliminators = { 3, 5, 7 };

        auto new_end = std::remove_if(data.begin(), data.end(), [&eliminators = std::as_const(eliminators)](const auto& val) {
                return std::any_of(eliminators.begin(), eliminators.end(), [&val](const int eliminator) { return val % eliminator == 0; });
            });


        data.erase(new_end, data.end());

        REQUIRE_THAT(data, Equals(vector<int>{1, 8, 13}));
    }

    SECTION("calculate average")
    {
        auto sum = 0.0;

        std::for_each(data.begin(), data.end(), [&sum](auto a) { sum += a; });

        double avg = sum / data.size();

        REQUIRE_THAT(avg, WithinAbs(11.2, 0.1));

        SECTION("create two containers - 1st with numbers less or equal to average & 2nd with numbers greater than average")
        {
            vector<int> less_equal_than_avg;
            vector<int> greater_than_avg;

            std::partition_copy(data.begin(), data.end(),
                std::back_inserter(less_equal_than_avg),
                std::back_inserter(greater_than_avg),
                [avg](auto a) { return a <= avg; });

            REQUIRE_THAT(less_equal_than_avg, Equals(vector<int>{1, 6, 3, 5, 8, 9, 10}));
            REQUIRE_THAT(greater_than_avg, Equals(vector<int>{13, 12, 45}));
        }
    }
}
