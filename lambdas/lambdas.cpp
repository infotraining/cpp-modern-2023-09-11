#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <memory>
#include <string_view>

using namespace std;


auto foo(int a) -> int
{
    return a * a;
};

struct Foo
{
    int factor;

    int operator()(int a) const
    {
        return a * factor;
    }
};

TEST_CASE("callables in C++98")
{
    CHECK(foo(4) == 16);

    Foo f{2};

    CHECK(f(8) == 16);
}

TEST_CASE("lambda")
{
    auto l_foo = [](int a) { return a * a; };

    CHECK(l_foo(4) == 16);
}

TEST_CASE("using lambdas")
{
    std::vector vec = { 1, 42, 665, 3, 4, 7, 10 };

    if (auto pos_gt_100 = std::find_if(vec.begin(), vec.end(), [](int value) { return value > 100; }); pos_gt_100 != vec.end())
    {
        CHECK(*pos_gt_100 == 665);
    }
}

//////////////////////////////////////
// how lambda works

struct Lambda_2734728345675
{
    //Lambda_2734728345675() = deleted;

    auto operator()(int a) const
    { 
        return a * a; 
    }
};

TEST_CASE("how lambda works")
{
    auto l_foo = [](int a) { return a * a; };

    SECTION("closure object")
    {
        auto l_foo = Lambda_2734728345675{};
    }
}

TEST_CASE("defining a return type")
{
    auto get_name = [](int id) -> std::string {
        if (id == 1)
            return "one";
        else
            return "unknown";
    };

    auto name = get_name(1);
}

TEST_CASE("captures")
{
    std::vector vec = { 1, 42, 665, 3, 4, 7, 10 };

    double sum = 0.0;
    double avg = 0.0;
    
    std::for_each(vec.begin(), vec.end(), [&sum](int value) { sum += value; });
    
    avg = sum / vec.size();

    auto pos_gt_than_avg = std::find_if(vec.begin(), vec.end(), [avg](int value) { return value > avg; });

    std::cout << *pos_gt_than_avg << "\n";

    std::unique_ptr<int> uptr = std::make_unique<int>(42);

    auto lambda_with_movable_object = [lambda_ptr = std::move(uptr)]() { std::cout << *lambda_ptr << "\n"; };

    lambda_with_movable_object();
}

struct Gadget
{
    std::string name;

    void print() const
    {
        auto printer = [this] { std::cout << name << "\n"; };       
        printer();
    }
};

TEST_CASE("capturing this")
{
    Gadget g{"ipad"};

    g.print();
}

// struct Lambda_282592635279836492
// {
//     int seed_;

//     auto operator()() { return ++seed; }
// };

auto create_generator(int seed)
{
    return [seed]() mutable { return ++seed; };
}

TEST_CASE("mutable lambda")
{
    int seed = 0;
    auto f = [seed]() mutable { return ++seed; };

    CHECK(f() == 1);
    CHECK(f() == 2);
    CHECK(f() == 3);

    auto g = create_generator(1000);
    CHECK(g() == 1001);
}

struct Lambda_473653814759314873846
{
    template <typename T>
    auto operator()(const T& value) const { std::cout << value << "\n"; }
};


TEST_CASE("generic lambda")
{
    auto printer = [](const auto& value) { std::cout << value << "\n"; };

    printer(42);
    printer("CText");
    printer("Text"s);

    auto cmp_by_size = [](const auto& a, const auto& b) { return a.size() < b.size(); };

    std::vector<std::string> words = { "fifty-one", "one", "two", "zero", "four" };

    std::sort(words.begin(), words.end(), cmp_by_size);

    for(const auto& w : words)
    {
        printer(w);
    }
}
    
template <typename T>
void some_function(T str_arg) 
{ }

TEST_CASE("deduction rules for auto")
{
    std::string str = "str";

    auto str_a = str;

    auto gl = [](auto str_arg) { };
    
    some_function(str);

    gl(str);

    SECTION("stripping refs")
    {
        std::string& ref_str = str;

        auto str_a = ref_str;

        auto gl = [](auto str_arg) {};
        gl(ref_str);

        some_function(ref_str);
    }
}


TEST_CASE("forwarding and lambda")
{
    std::vector<int> big_data(1'000'000);
    int counter = 0;
    auto call_wrapper = [&counter](auto&& f, auto&& arg) { f(std::forward<decltype(arg)>(arg)); ++counter; };

    call_wrapper(foo, 42);
    call_wrapper(foo, 665);
    auto printer = [big_data](const std::string& str) { std::cout << str << "\n"; };
    call_wrapper(printer, "text");

    CHECK(counter == 3);
}