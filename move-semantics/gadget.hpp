#ifndef GADGET_HPP
#define GADGET_HPP

#include <iostream>
#include <string>

struct Gadget
{
    inline static uintmax_t copy_counter = 0;
    inline static uintmax_t move_counter = 0;

    static void reset_counters()
    {
        copy_counter = 0;
        move_counter = 0;
    }

    static void print_counter_stats()
    {
        std::cout << "----------------------\n";
        std::cout << "No of copies: " << copy_counter << "\n";
        std::cout << "No of moves: " << move_counter << "\n";
        std::cout << "----------------------\n";
    }

    int id{};
    std::string name{"default-name"};
    bool is_after_move{false};

    Gadget()
    {
        std::cout << "Gadget(dc: " << id << ", " << name << ")\n";
    }

    Gadget(int v, const std::string& n)
        : id{v}
        , name{n}
    {
        std::cout << "Gadget(" << id << ", " << name << ")\n";
    }

    Gadget(const Gadget& other)
        : id{other.id}, name{other.name}, is_after_move{other.is_after_move}
    {
        std::cout << "Gadget(cc: " << id << ", " << name << ")\n";
        ++copy_counter;
    }

    Gadget& operator=(const Gadget& other)
    {
        if (this != &other)
        {
            id = other.id;
            name = other.name;
            is_after_move = other.is_after_move;
        }

        std::cout << "Gadget(copy_assignment: " << id << ", " << name << ")\n";
        ++copy_counter;

        return *this;
    }

    Gadget(Gadget&& other) noexcept
        : id{std::move(other.id)}, name{std::move(other.name)}
    {
        other.is_after_move = true;
        std::cout << "Gadget(mv: " << id << ", " << name << ")\n";
        ++move_counter;
    }

    Gadget& operator=(Gadget&& other) 
    {
        if (this != &other)
        {
            id = std::move(other.id);
            name = std::move(other.name);

            is_after_move = false;
            other.is_after_move = true;
        }

        std::cout << "Gadget(move_assignment: " << id << ", " << name << ")\n";
        ++move_counter;

        return *this;
    }

    ~Gadget()
    {
        if (!is_after_move)
        {
            std::cout << "~Gadget(" << id << ", " << name << ")\n";
        }
        else
        {
            std::cout << "~Gadget(" << id << " - after move)\n";
        }
    }

    void use() const
    {
        std::cout << "Using Gadget(" << id << ", " << name << ")\n";
    }
};

#endif