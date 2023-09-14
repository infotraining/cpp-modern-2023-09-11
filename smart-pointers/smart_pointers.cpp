#include "utils.hpp"
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <string>
#include <vector>
#include <catch2/catch_test_macros.hpp>


// https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-resource

using namespace std;
using namespace Utils;

namespace LegacyCode
{
    // forward declarations
    Gadget* get_gadget(const std::string& name);
    void use(Gadget* g);
    void use_gadget(Gadget* g);


    // definitions
    Gadget* get_gadget(const std::string& name)
    {
        static int id = 665;
        return new Gadget(++id, name);
    }

    void use(Gadget* g)
    {
        if (g)
            std::cout << "Using " << g->name() << "\n";

        delete g;
    }

    void use_gadget(Gadget* g)
    {
        if (g)
            std::cout << "Using " << g->name() << "\n";
    }
}

namespace ModernCpp
{
    std::unique_ptr<Gadget> create_gadget(const std::string& name);
    void use(std::unique_ptr<Gadget> g);

    std::unique_ptr<Gadget> create_gadget(const std::string& name)
    {
        static int id = 665;
        return std::make_unique<Gadget>(++id, name);
    }

    void use(std::unique_ptr<Gadget> g)
    {
        if (g)
            std::cout << "Using " << g->name() << "\n";
    }

    void use_gadget(Gadget* g)
    {
        if (g)
            std::cout << "Using " << g->name() << "\n";
    }

    std::unique_ptr<Gadget> use_gadget(std::unique_ptr<Gadget> g)
    {
        if (g)
            std::cout << "Using " << g->name() << "\n";
        return g;
    }
}

TEST_CASE("Legacy hell with dynamic memory")
{
    using namespace ModernCpp;

    {
        std::unique_ptr<Gadget> g = create_gadget("ipad");

        use_gadget(g.get());

        g = use_gadget(std::move(g));

        use(std::move(g));
    }

    {
        use_gadget(create_gadget("ipad"));
    }

    {
        std::unique_ptr<Gadget> g = create_gadget("ipad");

        //use(g.get());
        use(std::move(g));

        //std::cout << g->name() << std::endl;
    }
}

TEST_CASE("working with legacy")
{
    std::unique_ptr<Gadget> g{ LegacyCode::get_gadget("legacy ipad") };

    LegacyCode::use_gadget(g.get());

    //may_throw();

    LegacyCode::use(g.release());

    CHECK(g == nullptr);
}

TEST_CASE("custom deallocators")
{

    SECTION("using function pointer")
    {
        // fopen / fclose

        std::unique_ptr<FILE, int(*)(FILE*)> my_file{ fopen("my_file.txt", "w+"), &fclose };

        if (my_file)
        {
            //may_throw();
            fprintf(my_file.get(), "some text");
        }
    }

    SECTION("lambda")
    {
        auto file_closer = [](FILE* f) { fclose(f); std::cout << "File is closed!\n"; };

        std::unique_ptr<FILE, decltype(file_closer)> my_file{fopen("my_file.txt", "w+"), file_closer};

        if (my_file)
        {
            //may_throw();
            fprintf(my_file.get(), "some text");
        }
    }
}


TEST_CASE("shared_ptr vs. weak_ptr")
{
    std::weak_ptr<Gadget> wptr_g;
    auto ptr_g1 = std::make_shared<Gadget>(1, "ipad");

    {
        std::shared_ptr ptr_g2 = ptr_g1;

        wptr_g = ptr_g1;
    }

    std::shared_ptr<Gadget> ptr_g3 = wptr_g.lock();

    if (ptr_g3)
    {
        std::cout << ptr_g3->name() << " lives...\n";
    }
    else
    {
        std::cout << "Gadget was destroyed...\n";
    }
}

TEST_CASE("unique_ptr -> shared_ptr")
{
    std::unique_ptr<Gadget> uptr_g = ModernCpp::create_gadget("IPad"); // we have unique ownership

    std::shared_ptr<Gadget> sptr_g = std::move(uptr_g); // we start to share ownership

    auto another_owner = sptr_g;
}