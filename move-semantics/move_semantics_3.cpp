#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <algorithm>
#include <string_view>
#include <utility>

////////////////////////////////////////////////////////////////////////////
// Data - class with copy & move semantics (user provided implementation)

class Data
{
    std::string name_;
    int* data_;
    size_t size_;

public:
    using iterator = int*;
    using const_iterator = const int*;

    Data(std::string name, std::initializer_list<int> list)
        : name_{ std::move(name) }, data_{ new int[list.size()] }
        , size_{ list.size() }
    {
        std::copy(list.begin(), list.end(), data_);

        std::cout << "Data(" << name_ << ")\n";
    }

    Data(const Data& other)
        : name_(other.name_)
        , data_{ new int[other.size_] }
        , size_(other.size_)
    {
        std::copy(other.begin(), other.end(), data_);

        std::cout << "Data(" << name_ << ": cc)\n";
    }

    Data& operator=(const Data& other)
    {
        Data temp(other); // cc
        swap(temp);

        std::cout << "Data=(" << name_ << ": cc)\n";

        return *this;
    }

    /////////////////////////////////////////////////
    // move constructor
    Data(Data&& other) : name_(std::move(other.name_)),
        data_{ std::exchange(other.data_, nullptr) }, size_{ std::exchange(other.size_, 0) }
    {
        std::cout << "Data(" << name_ << ": mv)\n";
    }

    /////////////////////////////////////////////////
    // move assignment
    Data& operator=(Data&& other)
    {
        if (this != &other)
        {
            Data temp(std::move(other)); // mv
            swap(temp);
        }

        std::cout << "Data=(" << name_ << ": mv)\n";

        return *this;
    }

    ~Data()
    {
        delete[] data_;
    }

    void swap(Data& other)
    {
        name_.swap(other.name_);
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    }

    iterator begin()
    {
        return data_;
    }

    iterator end()
    {
        return data_ + size_;
    }

    const_iterator begin() const
    {
        return data_;
    }

    const_iterator end() const
    {
        return data_ + size_;
    }

    size_t size() const
    {
        return size_;
    }
};

Data create_data_set()
{
    Data ds{ "data-set-one", {54, 6, 34, 235, 64356, 235, 23} };

    return ds;
}

template <typename TContainer>
void print(const TContainer& container, std::string_view prefix = "items")
{
    std::cout << prefix << " - [ ";
    for (const auto& item : container)
    {
        std::cout << item << " ";
    }
    std::cout << "]\n";
}

TEST_CASE("std::move for primitive types means copy")
{
    int x = 42;
    int y = std::move(x); // copy
    CHECK(x == y);

    int* ptr_a = new int(13);
    int* ptr_b = std::move(ptr_a); // copy
    CHECK(ptr_a == ptr_b);
}

TEST_CASE("Data & move semantics")
{
    Data ds1{ "ds1", {1, 2, 3, 4, 5} };

    Data backup = ds1; // copy
    print(backup, "backup");

    Data target = std::move(ds1); // move
    print(target, "other");

    Data ds2 = create_data_set(); // move
}

struct LargeDataSet
{
    std::string name;
    Data ds;
    int value;

    // Rule of Five
    ~LargeDataSet() = default;  // user declared
    LargeDataSet(const LargeDataSet&) = default;  // user declared
    LargeDataSet& operator=(const LargeDataSet&) = default;
    LargeDataSet(LargeDataSet&&) = default;
    LargeDataSet& operator=(LargeDataSet&&) = default;
};

TEST_CASE("LargeDataSet")
{
    LargeDataSet lds{"lds", {"ds", {1, 2, 3}}, 42};

    LargeDataSet lds_backup = lds; // cc
    LargeDataSet lds_target = std::move(lds); // mv

    // CHECK(lds.name == "");
    // CHECK(lds.value == 42);
}

struct AnotherDataSet 
{
    std::vector<int> data;

    AnotherDataSet(std::size_t size = 1024, int value = 0) : data(size, value)
    {        
    }

    // Rule of ZERO
};

TEST_CASE("AnotherDataSet")
{
    AnotherDataSet ads1(1024, 1);
    
    AnotherDataSet ads1_backup = ads1; // cc

    AnotherDataSet target = std::move(ads1);  // mv
}