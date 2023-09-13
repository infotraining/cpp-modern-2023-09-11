#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <algorithm>
#include <string_view>
#include <utility>
#include <memory>

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
    Data(Data&& other) noexcept
        : name_(std::move(other.name_)),
          data_{ std::exchange(other.data_, nullptr) }, size_{ std::exchange(other.size_, 0) }
    {
        //std::cout << "Data(" << name_ << ": mv)\n";
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

    // LargeDataSet(std::string name_arg, Data ds_arg, int value_arg) : name(name_arg), ds(ds_arg), value(value_arg)
    // {}

    // LargeDataSet(const std::string& name_arg, const Data& ds_arg, int value_arg) : name(name_arg), ds(ds_arg), value(value_arg)
    // {}

    LargeDataSet(std::string name_arg, Data ds_arg, int value_arg) : name(std::move(name_arg)), ds(std::move(ds_arg)), value(value_arg)
    {}

    // template <typename TNameArg, typename TDsArgs>
    // LargeDataSet(TNameArg&& name_arg, TDsArgs&& ds_arg, int value_arg) 
    //     : name(std::forward<TNameArg>(name_arg)), ds(std::forward<TDsArgs>(ds_arg), value{value_arg}
    // {
    // }

    // Rule of Five
    ~LargeDataSet() = default;  // user declared
    LargeDataSet(const LargeDataSet&) = default;  // user declared
    LargeDataSet& operator=(const LargeDataSet&) = default;
    LargeDataSet(LargeDataSet&&) = default;
    LargeDataSet& operator=(LargeDataSet&&) = default;
};

static_assert(std::is_nothrow_move_constructible_v<LargeDataSet>);

TEST_CASE("LargeDataSet")
{
    LargeDataSet lds{"lds", {"ds", {1, 2, 3}}, 42 };

    std::string lds_name = "lds2";
    Data ds2{ "ds2", {1, 2, 3}};

    LargeDataSet lds2{lds_name, ds2, 665};

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

struct PointerWrapper 
{
    int* data;

    PointerWrapper(int* data) noexcept : data{data}
    {}

    PointerWrapper(const PointerWrapper& other) = delete;
    PointerWrapper& operator=(const PointerWrapper& other) = delete;

    PointerWrapper(PointerWrapper&& other) noexcept
        : data{std::exchange(other.data, nullptr)}
    {
    }

    PointerWrapper& operator=(PointerWrapper&& other)
    {
        // TODO

        return *this;
    }

    ~PointerWrapper()
    {
        delete data;
    }
};

struct SharedPointerWrapper 
{
    std::shared_ptr<int> data;
};

TEST_CASE("copy & move for PointerWrapper")
{
    int* x = new int(42);
    
    PointerWrapper pw1{x};
    PointerWrapper pw2 = std::move(pw1); // it will copy pw1.data to pw2.data
}

TEST_CASE("AnotherDataSet")
{
    AnotherDataSet ads1(1024, 1);

    AnotherDataSet ads1_backup = ads1; // cc

    AnotherDataSet target = std::move(ads1);  // mv
}


struct EvilCaseOfForwardingConstructor
{
    std::string member;

    // EvilCaseOfForwardingConstructor(std::string m) : member{std::move(m)}
    // {}

    template <typename TArg>
    EvilCaseOfForwardingConstructor(TArg&& m) : member{std::forward<TArg>(m)}
    {}
};

TEST_CASE("Evil case")
{
    EvilCaseOfForwardingConstructor ecofc{"text"};

    //EvilCaseOfForwardingConstructor backup = ecofc; // evil - copy constructor is overshadowed by templated 
}