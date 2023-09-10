#include <catch2/catch_test_macros.hpp>
#include <cassert>
#include <iostream>
#include <type_traits>
#include <array>

using namespace std;

namespace ex
{
    // TODO - 1
    template <typename T, size_t N>
    struct Array
    {
        T items[N];

        using reference = T&;
        using const_reference = const T&;
        using iterator = T*;
        using const_iterator = const T*;

        reference operator[](size_t index)
        {
            return items[index];
        }

        const_reference operator[](size_t index) const
        {
            return items[index];
        }

        size_t size() const
        {
            return N;
        }

        iterator begin()
        {
            return &items[0];
        }

        iterator end()
        {
            return begin() + N;
        }

        const_iterator begin() const
        {
            return &items[0];
        }

        const_iterator end() const
        {
            return begin() + N;
        }
    };

    template <typename T, size_t N>
    bool operator==(const Array<T, N>& left, const Array<T, N>& right)
    {        
        auto it_left = left.begin();
        auto it_right = right.begin();
        for(; it_left != left.end(); ++it_left, ++it_right)
            if (*it_left != *it_right)
                return false;
        return true;
    }

    // TODO - 2
    template <size_t N> 
    Array<uintmax_t, N> create_fibonacci_lookup()
    {
        return Array<uintmax_t, N>{};    
    }
}

TEST_CASE("constexpr fibonacci sequence")
{
    // TODO - 1: modernize ex::Array class - all methods should be constexpr
    // TODO - 2: using ex::Array write a function that creates a fibonacci lookup table

    // constexpr auto fibonacci_lookup = ex::create_fibonacci_lookup<10>();

    // static_assert(fibonacci_lookup.size() == 10, "Error");
    // static_assert(fibonacci_lookup == Array<uintmax_t, 10>{0, 1, 1, 2, 3, 5, 8, 13, 21, 34}, "Error");
}