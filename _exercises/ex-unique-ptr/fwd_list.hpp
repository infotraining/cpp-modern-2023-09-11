#ifndef FWD_LIST_HPP
#define FWD_LIST_HPP

#include <memory>
#include <sstream>

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

        friend std::ostream& operator<<(std::ostream& out, const FwdList<T>& lst)
        {
            out << "[";
            Node* node = lst.head_;

            while(node)
            {
                out << node->value;
                node = node->next;

                if (node)
                    out << ", ";
            }

            out << "]";

            return out;
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
        // TODO: use std::unique_ptr & implement FwdList
    };
}

#endif