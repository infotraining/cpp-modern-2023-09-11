#include <algorithm>
#include <array>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>
#include <string_view>

using namespace std;

template <typename Container>
void print_all(const Container& container, std::string_view prefix)
{
    cout << prefix << ": [ ";
    for (const auto& item : container)
        cout << item << " ";
    cout << "]\n";
}

TEST_CASE("string_view")
{
    std::string text = "Some Text";
    
    print_all(text, "text");

    std::string_view text_sv = text;
    std::string_view other_text_sv = "Hello SV";

    std::string backup(other_text_sv); // explicit conversion from string_view -> string
}

TEST_CASE("std::string vs. std::string_view")
{
    std::string empty;
    CHECK(empty.data() != nullptr);

    std::string_view sv;
    CHECK(sv.data() == nullptr);
}

string_view start_from_word(string_view text, string_view word)
{
      return text.substr(text.find(word));
}

TEST_CASE("tokenizing")
{
    std::string_view prefix = "World"sv;
    const char* text = "HelloWorld!!!";

    auto token = start_from_word(text, prefix);

    REQUIRE(token == "World!!!");

    SECTION("beware of dangling pointers")
    {
        auto token1 = start_from_word("HelloWorld!!!", prefix); // OK
        auto token2 = start_from_word(std::string{"HelloWorld!!!"}, prefix); // UB

        SECTION("it should be")
        {
            std::string text{"HelloWorld!!!"};
            
            auto token = start_from_word(text, prefix);

            CHECK(token == "World!!!"sv);
        }
    }
}

TEST_CASE("UB")
{
    std::string_view path = "/dev/lib";

    fopen(path.data(), "w+"); // UB
}