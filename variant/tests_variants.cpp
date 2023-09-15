#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <numeric>
#include <string>
#include <variant>
#include <vector>

using namespace std;



TEST_CASE("variant")
{
    std::variant<int, double, std::string> var1 = 42;

    var1 = 3.14;
    var1 = "text"s;
    
    CHECK(std::holds_alternative<std::string>(var1) == true);

    CHECK(std::get<std::string>(var1) == "text"s);
    CHECK_THROWS_AS(std::get<int>(var1), std::bad_variant_access);

    std::string* ptr_str = std::get_if<std::string>(&var1);
    if (ptr_str)
    {
        std::cout << *ptr_str << "\n";
    }
}

struct Printer
{
    int counter = 0;

    void operator()(int n) { std::cout << "int: " << n << "\n"; ++counter; }
    void operator()(double d) { std::cout << "double: " << d << "\n"; ++counter; }

    template <typename TContainer>
    void operator()(const TContainer& c) { std::cout << "container: " << c.size() << "\n"; ++counter; }
};

template <typename... Ts>
struct MagicOverload : Ts...
{
    using Ts::operator()...;
};

// CTAD - deduction guide
template<typename... Ts>
MagicOverload(Ts...) -> MagicOverload<Ts...>;


// struct A { void foo() {} };
// struct B { void bar() {} };

// TEST_CASE("MagicOverload")
// {
//     MagicOverload<A, B> moa;
//     moa.bar();
//     moa.foo();

//     MagicOverload mob{A{}, B{}};
//     mob.foo();
//     moa.bar();
// }


TEST_CASE("visiting variants")
{
    std::variant<int, double, std::string, std::vector<int>> var1 = 42;

    Printer prn;
    std::visit(prn, var1);
    var1 = "text"s;
    std::visit(prn, var1);

    CHECK(prn.counter == 2);

    std::visit(Printer{2}, var1);

    int counter = 0;

    auto inline_printer = MagicOverload {
        [&counter](int n) { std::cout << "int: " << n << "\n"; ++counter; },
        [&counter](double d) { std::cout << "double: " << d << "\n"; ++counter; },
        [&counter](const auto& c) { std::cout << "container: " << c.size() << "\n"; ++counter; }
    };

    std::visit(MagicOverload {
        [&counter](int n) { std::cout << "int: " << n << "\n"; ++counter; },
        [&counter](double d) { std::cout << "double: " << d << "\n"; ++counter; },
        [&counter](const auto& c) { std::cout << "container: " << c.size() << "\n"; ++counter; }
    }, var1);
}

[[nodiscard]] std::variant<std::string, std::errc> load_content(const std::string& filename)
{
    if (filename == "")
        return std::errc::bad_file_descriptor;
    return "content"s;
}

template <typename... Ts>
using overload = MagicOverload<Ts...>;

TEST_CASE("using variant")
{
    auto result = load_content("data");

    std::visit(overload{ 
        [](std::errc ec) { std::cout << "Error!!! " <<  static_cast<int>(ec) << std::endl; },
        [](const std::string& s) { std::cout << "Content: " << s << "\n"; }
    }, result);
}