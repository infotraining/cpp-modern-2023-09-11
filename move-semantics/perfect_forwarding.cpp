#include <catch2/catch_test_macros.hpp>
#include "gadget.hpp"

//#define MSVC

// #if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
// #define __PRETTY_FUNCTION__ __FUNCSIG__
// #endif


////////////////////////////////////////////////////////
///  PERFECT FORWARDING

void have_fun(Gadget& g)
{
    puts(__PRETTY_FUNCTION__);
    g.use();
}

void have_fun(const Gadget& cg)
{
    puts(__PRETTY_FUNCTION__);
    cg.use();
}

void have_fun(Gadget&& g)
{
    puts(__PRETTY_FUNCTION__);
    g.use();
}

// void use(const Gadget& g)
// {
//     have_fun(g);
// }

// void use(Gadget& g)
// {
//     have_fun(g);
// }

// void use(Gadget&& g) // g is lvalue - Gadget&&
// {
//     have_fun(std::move(g)); // beacause of lvalue of g we have to use std::move
// }

template <typename TGadget>
void use(TGadget&& g) // universal reference - forwarding reference
{
    have_fun(std::forward<TGadget>(g)); // perfect forwarding

    // if constexpr(std::is_reference_v<TGadget>)
    // {
    //     have_fun(g);
    // }
    // else
    // {
    //     have_fun(std::move(g));
    // }
}

// namespace Cpp20
// {
//     void use(auto&& g) // universal reference - forwarding reference
//     {
//         have_fun(std::forward<decltype(g)>(g)); // perfect forwarding
//     }
// }



TEST_CASE("custom forwarding")
{
    Gadget g{ 1, "gadget" };
    const Gadget cg{ 2, "const-gadget" };

    use(g);
    use(cg);
    use(Gadget{ 3, "temp-gadget" });
}

///////////////////////////////////////////////////////////////////////////

struct GadgetContainer
{
    std::vector<Gadget> gadgets;

    GadgetContainer()
    {
        gadgets.reserve(100);
    }
    
    /*
    void add(const Gadget& g)
    {
        gadgets.push_back(g);
    }

    void add(Gadget&& g)
    {
        gadgets.push_back(std::move(g));
    }
   */
   
    template<typename T>
    void add(T&& g)
    {
        gadgets.push_back(std::forward<T>(g));
    }    

    template <typename... TArgs>
    void emplace(TArgs&&... args)
    {
        gadgets.emplace_back(std::forward<TArgs>(args)...);
    }
};

TEST_CASE("forwarding to containers")
{
    Gadget::reset_counters();

    GadgetContainer g_container;

    Gadget g1{ 1, "ipad" };
    g_container.add(g1);

    g_container.add(Gadget{ 2, "ipod" });

    g_container.emplace(3, "smartwatch");
}

///////////////////////////////

template <typename T>
void ref_collapse(T& arg)
{    
}

template <typename T>
void accepts_all_value_categories(T&& t)
{
}

TEST_CASE("reference collapsing")
{
    int x = 42;
    ref_collapse(x);

    accepts_all_value_categories(x);
    accepts_all_value_categories(42);
}

TEST_CASE("auto declarations & universal refs")
{
    auto x = 42;

    auto&& ax1 = x;  //  int&
    auto&& ax2 = 42; // int&&
}

TEST_CASE("auto& vs. auto&&")
{
    int x = 42;

    auto& aref1 = x;

    std::vector<bool> vec = {1, 1, 1, 1, 1};

    for(auto&& item : vec)
    {
        item = 0;
    }
}
