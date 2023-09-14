#include <catch2/catch_test_macros.hpp>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>

class Observer
{
public:
    virtual void update(const std::string& event_args) = 0;
    virtual ~Observer() { }
};

class Subject
{
    int state_;
    std::set<std::weak_ptr<Observer>, std::owner_less<std::weak_ptr<Observer>>> observers_;

public:
    Subject()
        : state_(0)
    {
    }

    void register_observer(std::weak_ptr<Observer> observer)
    {
        observers_.insert(observer);
    }

    void unregister_observer(std::weak_ptr<Observer> observer)
    {
        observers_.erase(observer);
    }

    void set_state(int new_state)
    {
        if (state_ != new_state)
        {
            state_ = new_state;
            notify("State has been set to: " + std::to_string(state_));
        }
    }

protected:
    void notify(const std::string& event_args)
    {
        for (auto it = observers_.begin(); it != observers_.end(); ++it)
        {
            std::shared_ptr<Observer> living_observer = it->lock();
            if (living_observer)
                living_observer->update(event_args);
            // else
            // we should remove the pointer to destroyed object from the set
            
        }
    }
};

class Customer : public Observer
{
public:
    void update(const std::string& event)
    {
        std::cout << "Customer notified: " << event << std::endl;
    }
};

class Logger : public Observer
{
    std::string path_;

public:
    Logger(const std::string& p)
        : path_{p}
    {
    }

    void update(const std::string& event)
    {
        std::cout << "Logging to " << path_ << ": " << event << std::endl;
    }

    ~Logger()
    {
        std::cout << "Logger " << path_ << " is closed...\n";
    }
};

TEST_CASE("observer")
{
    using namespace std;

    Subject s;

    {
        auto o1 = std::make_shared<Customer>();
        s.register_observer(o1);

        {
            auto logger = std::make_shared<Logger>("log-20200707-12:22:33.dat");
            s.register_observer(logger);

            s.set_state(1);
            s.set_state(2);
            s.set_state(3);            
        }

        cout << "++++ End of scope ++++\n\n"
             << endl;

        s.set_state(42);
        s.set_state(665);
        s.unregister_observer(o1);
    }
}
