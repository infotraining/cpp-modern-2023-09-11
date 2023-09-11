#include <catch2/catch_test_macros.hpp>
#include "gadget.hpp"
#include <memory>
#include <string>

template <typename T>
class UniquePtr
{
    T* ptr_;

public:
    UniquePtr(nullptr_t)
        : ptr_{ nullptr }
    {
    }

    UniquePtr() : ptr_{ nullptr }
    {}

    explicit UniquePtr(T* ptr)
        : ptr_{ ptr }
    {
    }

    // move constructor
    UniquePtr(UniquePtr&& other) : ptr_{ other.ptr_ }
    {
        other.ptr_ = nullptr;
    }

    // move assignment operator
    UniquePtr& operator=(UniquePtr&& other)
    {
        if (this != &other)
        {
            delete this->ptr_; // free the previous state

            this->ptr_ = other.ptr_; // transfer the ownership
            other.ptr_ = nullptr;

            // UniquePtr temp(std::move(other));
            // swap(temp);
        }

        return *this;
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    ~UniquePtr()
    {
        delete ptr_;
    }

    explicit operator bool() const
    {
        return ptr_ != nullptr;
    }

    T* get() const
    {
        return ptr_;
    }

    T* operator->() const
    {
        return ptr_;
    }

    T& operator*() const
    {
        return *ptr_;
    }

    void swap(UniquePtr& other)
    {
        std::swap(this->ptr_, other.ptr_);
    }
};


TEST_CASE("move semantics - UniquePtr")
{
    UniquePtr<Gadget> pg1{ new Gadget{1, "ipad"} };
    (*pg1).use(); // calls operator*
    pg1->use();   // call operator->

    UniquePtr<Gadget> pg2 = std::move(pg1);
    pg2->use();

    UniquePtr<Gadget> pg3{ new Gadget{2, "ipod"} };
    pg3 = std::move(pg2);

    pg2->use();

    CHECK(pg1.get() == nullptr);
}