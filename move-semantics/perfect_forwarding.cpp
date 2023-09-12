#include <catch2/catch_test_macros.hpp>
#include "gadget.hpp"

//#define MSVC

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif


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

void use(Gadget& g)
{
    have_fun(g);
}

void use(const Gadget& g)
{
    have_fun(g);
}

TEST_CASE("custom forwarding")
{
    Gadget g{1, "gadget"};
    const Gadget cg{2, "const-gadget"};

    use(g);
    use(cg);
    use(Gadget{3, "temp-gadget"});
}

///////////////////////////////////////////////////////////////////////////

struct GadgetContainer
{
    std::vector<Gadget> gadgets;

    GadgetContainer() 
    {
        gadgets.reserve(100);
    }

    void add(const Gadget& g)
    {
        gadgets.push_back(g);
    }
};

TEST_CASE("forwarding to containers")
{
    Gadget::reset_counters();

    GadgetContainer g_container;

    Gadget g1{1, "ipad"};
    g_container.add(g1);
}