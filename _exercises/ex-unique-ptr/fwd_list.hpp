#ifndef FWD_LIST_HPP
#define FWD_LIST_HPP

#include <memory>

namespace LegacyCode
{
    template <typename T>
    class FwdList
    {
    private:
        struct Node
        {
            T value;
            Node* next;
        };
    public:
        FwdList() : size_(0), head_(NULL)
        {
        }

        ~FwdList()
        {
            while (head_)
            {
                Node* node = head_;
                head_ = node->next;
                delete node;
            }
        }

        bool empty() const
        {
            return size_ == 0;
        }

        size_t size() const
        {
            return size_;
        }

        void push_front(const T& item)
        {
            Node* new_node = new Node{ item, head_ };
            head_ = new_node;
            ++size_;
        }

        T& front() const
        {
            assert(head_ != NULL);
            return head_->value;
        }

        void pop_front()
        {
            assert(size_ != 0);

            Node* node_to_pop = head_;
            head_ = node_to_pop->next;

            delete node_to_pop;
            --size_;
        }
    private:
        size_t size_;
        Node* head_;

        // FwdList is non-copyable
        FwdList(const FwdList&);
        FwdList& operator=(const FwdList&);
    };
}

namespace ModernCpp
{
    template <typename T>
    class FwdList
    {
    private:
        struct Node
        {
            T value;
            std::unique_ptr<Node> next;

            Node(T value, std::unique_ptr<Node> next = nullptr)
                : value(std::move(value)), next(std::move(next))
            {
            }
        };
    public:
        FwdList() : size_{0}, head_{nullptr}
        {
        }

        FwdList(const FwdList&) = delete;
        FwdList& operator=(const FwdList&) = delete;

        FwdList(FwdList&&) = default;
        FwdList& operator=(FwdList&&) = default;

        ~FwdList()
        {
            while (head_)
            {
                head_ = std::move(head_->next);
            }
        }

        bool empty() const
        {
            return size_ == 0;
        }

        size_t size() const
        {
            return size_;
        }

        void push_front(const T& item)
        {
            auto new_node = std::make_unique<Node>(item, std::move(head_));
            head_ = std::move(new_node);
            ++size_;
        }

        T& front() const
        {
            assert(head_ != NULL);
            return head_->value;
        }

        void pop_front()
        {
            assert(size_ != 0);

            std::unique_ptr<Node> node_to_pop = std::move(head_);
            head_ = std::move(node_to_pop->next);

            --size_;
        }
    private:
        size_t size_;
        std::unique_ptr<Node> head_;
    };
}

#endif