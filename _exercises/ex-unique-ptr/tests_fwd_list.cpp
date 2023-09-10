#include <catch2/catch_test_macros.hpp>
#include "fwd_list.hpp"

TEST_CASE("Forward List")
{
    using namespace ModernCpp;

    SECTION("default construction")
    {
        FwdList<int> fwd_lst;

        SECTION("is empty")
        {
            CHECK(fwd_lst.empty());
        }

        SECTION("size is zero")
        {
            CHECK(fwd_lst.size() == 0);
        }
    }

    SECTION("push_front")
    {
        FwdList<int> fwd_lst;

        SECTION("pushing one item")
        {
            fwd_lst.push_front(1);

            SECTION("is not empty")
            {
                CHECK(fwd_lst.empty() == false);
            }

            SECTION("size is changed")
            {
                CHECK(fwd_lst.size() == 1);
            }

            SECTION("inserts item at front")
            {
                CHECK(fwd_lst.front() == 1);
            }
        }

        SECTION("pushing many items")
        {
            fwd_lst.push_front(1);
            fwd_lst.push_front(2);
            fwd_lst.push_front(3);
            fwd_lst.push_front(4);

            SECTION("is not empty")
            {
                CHECK(fwd_lst.empty() == false);
            }

            SECTION("size is changed")
            {
                CHECK(fwd_lst.size() == 4);
            }

            SECTION("inserts item at front")
            {
                CHECK(fwd_lst.front() == 4);
            }
        }
    }

    SECTION("pop_front")
    {
        FwdList<int> fwd_lst;

        SECTION("one item in the list")
        {
            fwd_lst.push_front(1);

            CHECK(fwd_lst.front() == 1);

            fwd_lst.pop_front();
            
            SECTION("list is empty")
            {
                CHECK(fwd_lst.empty());
            }

            SECTION("size is zero")
            {
                CHECK(fwd_lst.size() == 0);
            }
        }

        SECTION("many items in the list")
        {
            fwd_lst.push_front(1);
            fwd_lst.push_front(2);
            fwd_lst.push_front(3);
            fwd_lst.push_front(4);

            CHECK(fwd_lst.front() == 4);
            CHECK(fwd_lst.size() == 4);

            fwd_lst.pop_front();
            
            SECTION("size is decreased")
            {
                CHECK(fwd_lst.size() == 3);
            }

            SECTION("item is popped from the front")
            {
                CHECK(fwd_lst.front() == 3);
            }
        }
    }

    SECTION("stress test")
    {
        FwdList<int> fwd_lst;

        for(int i = 0; i < 1'000'000; ++i)
            fwd_lst.push_front(i);
    }
}