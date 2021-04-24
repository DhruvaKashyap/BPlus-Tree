#ifndef B_PLUS_H
#define B_PLUS_H
#ifndef DEBUG
#define DEBUG 0
#endif
#include <initializer_list>
#include <functional>
#include <memory>
#include <algorithm>
#include <concepts>
#include <iterator>
#include <vector>
#include <array>
#include <iostream>
// #include <assert.h>
using namespace std;
template <int N>
struct X
{
    constexpr static bool value = N > 1;
};
template <typename T>
struct Y
{
    constexpr static int value = 4;
};
template <>
struct Y<int>
{
    constexpr static int value = 8;
};
// template <int N>
// concept BPLUSMIN = X<N>::value;

template <typename T, int N = Y<T>::value, typename Compare = less<T>, class Alloc = allocator<T>>
// requires BPLUSMIN<N> //&& default_constructible && etcall types of stuff wrt T
class B_Plus_tree
{
private:
    struct Node
    {
        T key[N]; // 78896 could be a vector. could keep ptrs to keys to allow non default ctorable, more mem usage
        // vector<T> key; //80384 allows non default constructable
        // array<T, N> key; //78896
        Node *children[N + 1];
        Node *parent = nullptr;
        Node *next = nullptr;
        Node *prev = nullptr;
        int active_keys = 0;
        bool is_leaf = false;
        Node()
        {
            // key.reserve(N);
            fill(children, children + N + 1, nullptr);
        }
        void *operator new(size_t size)
        {
            return ::operator new(size); //call allocator or something here
        }
        friend ostream &operator<<(ostream &o, Node *n)
        {
            if (!n)
                o << "(nullptr)";
            else
            {
                copy(std::begin(n->key), std::begin(n->key) + n->active_keys, ostream_iterator<T>(o, "\t"));
                o << "(" << n->active_keys << ")";
            }
            return o;
        }
    };
    int __degree = N;
    Node *root = nullptr;
    Node *leaf_start = nullptr;
    Node *leaf_end = nullptr;
    size_t nums = 0;
    int insert_key_node_at(T key, Node *p, int loc = 0)
    {
        int i(loc);
        while (i < p->active_keys && p->key[i] < key)
            ++i;

        rotate(std::begin(p->key) + i, std::begin(p->key) + N - 1, std::begin(p->key) + N);
        p->key[i] = key;
        p->active_keys++;
        return i;
    }
    void split_push_up(Node *target, T median)
    {
        Node *nsibling = new Node;
        nsibling->is_leaf = target->is_leaf;
        nsibling->next = target->next;
        nsibling->active_keys = N / 2 + N % 2;
        int pos(0);
        if (target->parent == nullptr)
        {
            Node *np = new Node;
            np->key[0] = median;
            np->active_keys = 1;
            root = target->parent = np;
            ++nums;
        }
        else
        {
            pos = insert_key_node_at(median, target->parent);
        }
        nsibling->parent = target->parent;
        target->parent->children[pos] = target;
        target->parent->children[pos + 1] = nsibling;
        if (target->is_leaf)
        {
            // should median placement depend on predicate?
            // if yes change here and target->active_keys
            copy(std::begin(target->key) + N / 2, std::begin(target->key) + N, std::begin(nsibling->key)); //end begin no work bottleneck if vector
            target->next = nsibling;
            nsibling->prev = target;
            if (target == leaf_end)
                leaf_end = nsibling;
        }
        else
        {
            if constexpr (N == 2)
            {
                nsibling->key[0] = target->key[1];
            }
            else
            {
                copy(std::begin(target->key) + N / 2 + 1, std::begin(target->key) + N, std::begin(nsibling->key));
                --nsibling->active_keys;
            }
            copy(target->children + N / 2 + 1, target->children + N + 1, std::begin(nsibling->children) + static_cast<int>(N == 2));
            for_each(std::begin(nsibling->children), nsibling->children + N / 2 + 1, [nsibling](auto i) { if(i) i->parent = nsibling; });
        }
        fill(target->children + N / 2 + 1, target->children + N + 1, nullptr);
        target->active_keys = N / 2;
        if (target->parent->active_keys == N)
            split_push_up(target->parent, target->parent->key[N / 2]);
    }

    void insert_key(T key)
    {
        if (!root)
        {
            root = new Node;
            root->key[0] = key;
            leaf_start = root;
            leaf_end = root;
            root->active_keys = 1;
            root->is_leaf = true;
        }
        else
        {
            Node *p = root;
            Node *target = root;
            int i(0);
            while (p)
            {
                i = 0;
                while (i < p->active_keys && Compare()(p->key[i], key))
                {
                    ++i;
                }
                if (!Compare()(p->key[i], key) && !Compare()(key, p->key[i]))
                {
                    return; //return iterator(p,i),false
                }
                target = p;
                p = p->children[i];
            }
            insert_key_node_at(key, target, i);
            if (target->active_keys == N)
            {
                split_push_up(target, target->key[N / 2]);
            }
        }
        ++nums;
        //return iterator; iterator has p and i
    }
    void print_tree(Node *root)
    {
        if (root)
        {
            cout << root << "\n";
            if (!root->is_leaf)
            {
                for (auto i : root->children)
                    cout << i << "\t";
                cout << "\n\n";
                for (auto i : root->children)
                    print_tree(i);
            }
            cout << "\n\n\n";
        }
    }
    void delete_tree(Node *root)
    {
        if (root)
        {
            for (auto p : root->children)
                delete_tree(p);
            delete root;
        }
    }

public:
    class iterator
    {
        Node *ptr;
        Node *end;
        int index;
        iterator(Node *n, Node *e, int i) : ptr(n), end(e), index(i)
        {
        }
        friend class B_Plus_tree<T, N, Compare, Alloc>;

    public:
        using value_type = T;
        using iterator_category = bidirectional_iterator_tag;
        friend bool operator==(const iterator &lhs, const iterator &rhs)
        {
            return lhs.ptr == rhs.ptr && lhs.index == rhs.index;
        }
        friend bool operator!=(const iterator &lhs, const iterator &rhs)
        {
            return !(lhs == rhs);
        }
        iterator &operator++()
        {
            if (ptr)
            {
                if (index < ptr->active_keys - 1)
                {
                    ++index;
                }
                else
                {
                    index = 0;
                    ptr = ptr->next;
                }
            }
            else
            {
                ++index;
            }
            return *this;
        }
        iterator operator++(int)
        {
            iterator temp(*this);
            ++*this;
            return temp;
        }
        iterator &operator--()
        {
            if (ptr)
            {
                if (index > 0)
                {
                    --index;
                }
                else
                {
                    if (ptr->prev)
                    {
                        index = ptr->prev->active_keys - 1;
                        ptr = ptr->prev;
                    }
                }
            }
            else
            {
                if (!index)
                {
                    ptr = end;
                    index = end->active_keys - 1;
                }
                else
                    --index;
            }
            return *this;
        }
        iterator operator--(int)
        {
            iterator temp(*this);
            --*this;
            return temp;
        }
        T operator*()
        {
            return ptr->key[index];
        }
    };
    // traits
    // iters
    // ctors
    // more traits to be added
    using value_type = T;

    B_Plus_tree() {}

    explicit B_Plus_tree(std::initializer_list<T> l)
    {
        for (T i : l)
        {
            insert_key(i);
            // print_tree(root);
            // Node *x = leaf_start;
            // while (x!=leaf_end)
            // {
            //     cout << x;
            //     x = x->next;
            // }
            // cout<<leaf_end;
            // cout << "\n";
        }
#if DEBUG
        print_tree(root);

#endif
    }

    //copy ctor
    B_Plus_tree(const B_Plus_tree<T, N> &copy) {}

    //stl copy-like ctor
    template <typename it>
    B_Plus_tree(it begin, it end)
    {
        auto i = begin;
        while (i != end)
        {
            insert_key(*i);
            ++i;
        }
    }

    //ass ctor
    B_Plus_tree<T, N, Compare, Alloc> &operator=(const B_Plus_tree<T, N, Compare, Alloc> &rhs) {}

    //move ctor
    B_Plus_tree(B_Plus_tree<T, N, Compare, Alloc> &&copy) {}

    //move ass
    B_Plus_tree<T, N, Compare, Alloc> &operator=(B_Plus_tree<T, N, Compare, Alloc> &&rhs) {}

    //dtor
    ~B_Plus_tree()
    {
        delete_tree(root);
    }

    pair<iterator, bool> insert(T key) {} // inserts elements
    void insert(std::initializer_list<T> l)
    {
        for (T i : l)
        {
            insert_key(i);
        }
    }
    template <typename it>
    void insert(it begin, it end)
    {
        auto i = begin;
        while (i != end)
        {
            insert_key(*i);
            ++i;
        }
    }

    iterator delete_key(T key) {}
    iterator delete_key(iterator it) {}
    void delete_key(iterator begin, iterator end) {}

    void clear()
    {
        delete_tree(root);
        leaf_start = root = nullptr;
    }

    iterator find(T key) {}
    iterator begin()
    {
        return iterator(leaf_start, leaf_end, 0);
    }
    iterator end()
    {
        return iterator(nullptr, leaf_end, 0);
    }
    // // ...
    // // begin,end => in_begin,in_end
    // // rbegin, rend

    // // Performing a range query with k elements
    // // find_if
    int size();
};

#endif