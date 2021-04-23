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
using namespace std;
template <int N>
struct X
{
    constexpr static bool value = N > 1;
};
template <typename T>
struct Y
{
    constexpr static int value = 2;
};
template <>
struct Y<int>
{
    constexpr static int value = 4;
};
// template <int N>
// concept BPLUSMIN = X<N>::value;

template <typename T, int N = Y<T>::value, typename Compare = less<T>, class Alloc = allocator<T>>
// requires BPLUSMIN<N> && default_constructible && etcall types of stuff wrt T
class B_Plus_tree
{
private:
    struct Node
    {
        T key[N];
        Node *children[N + 1];
        Node *parent = nullptr;
        Node *next = nullptr;
        Node *prev = nullptr;
        int active_keys = 0;
        bool is_leaf = false;
        Node()
        {
            fill(key, key + N, T());
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
                copy(n->key, n->key + N, ostream_iterator<int>(o, "\t"));
                o << "(" << n->active_keys << ")";
            }
            return o;
        }
    };

    int __degree = N;
    Node *root = nullptr;
    Node *leaf_start = nullptr;
    int insert_key_node_at(T key, Node *p, int loc = 0)
    {
        int i(loc);
        while (i < p->active_keys && p->key[i] < key)
            ++i;
        rotate(p->key + i, p->key + N - 1, p->key + N);
        p->key[i] = key;
        p->active_keys++;
        return i;
    }
    void split_push_up(Node *target, T median)
    {
        Node *nsibling = new Node;
        //refactor 4 cases to 3
        if (target->parent == nullptr)
        {
            // new root up
            Node *np = new Node;
            np->is_leaf = false;
            np->key[0] = median;
            np->active_keys = 1;
            target->parent = np;
            np->children[0] = target;
            np->children[1] = nsibling;
            nsibling->parent = np;
            if (target->is_leaf)
            {
                // new leaf
                copy(target->key + N / 2, target->key + N, nsibling->key);
                target->next = nsibling;
                nsibling->prev = target;
                target->key[N / 2] = T();
                nsibling->active_keys = N / 2 + N % 2;
                nsibling->is_leaf = true;
            }
            else
            {
                //new internal
                if (N == 2) // only for 2, special case
                {
                    nsibling->key[0] = target->key[1];
                    nsibling->children[1] = target->children[2];
                    nsibling->children[1]->parent = nsibling;
                    target->key[1] = T();
                    nsibling->active_keys = 1;
                }
                else
                {
                    target->key[N / 2] = T();
                    copy(target->key + N / 2 + 1, target->key + N, nsibling->key);
                    copy(target->children + N / 2 + 1, target->children + N, nsibling->children);
                    for_each(nsibling->children, nsibling->children + N / 2 - 1, [nsibling](auto i) { i->parent = nsibling; });
                    nsibling->active_keys = N / 2 + N % 2 - 1;
                    nsibling->children[N / 2] = target->children[N];
                    nsibling->children[N / 2]->parent = nsibling;
                }
                nsibling->is_leaf = false;
            }
            fill(target->key + N / 2 + 1, target->key + N, T());
            fill(target->children + N / 2 + 1, target->children + N + 1, nullptr);
            target->active_keys = N / 2;
            root = np;
        }
        else
        {
            // new leaf
            int pos = insert_key_node_at(median, target->parent);
            nsibling->parent = target->parent;
            if (target->is_leaf)
            {
                copy(target->key + N / 2, target->key + N, nsibling->key);
                target->next = nsibling;
                nsibling->prev = target;
                target->key[N / 2] = T();
                nsibling->active_keys = N / 2 + N % 2;
                nsibling->is_leaf = true;
            }
            else
            {
                //new internal
                if (N == 2) // only for 2, special case
                {
                    nsibling->key[0] = target->key[1];
                    nsibling->children[1] = target->children[2];
                    nsibling->children[1]->parent = nsibling;
                    target->key[1] = T();
                    nsibling->active_keys = 1;
                }
                else
                {
                    target->key[N / 2] = T();
                    copy(target->key + N / 2 + 1, target->key + N, nsibling->key);
                    copy(target->children + N / 2 + 1, target->children + N, nsibling->children);
                    for_each(nsibling->children, nsibling->children + N / 2 - 1, [nsibling](auto i) { i->parent = nsibling; });
                    nsibling->children[N / 2] = target->children[N];
                    nsibling->children[N / 2]->parent = nsibling;
                    nsibling->active_keys = N / 2 + N % 2 - 1;
                }
                nsibling->is_leaf = false;
            }
            fill(target->key + N / 2 + 1, target->key + N, T());
            fill(target->children + N / 2 + 1, target->children + N + 1, nullptr);
            target->active_keys = N / 2;
            target->parent->children[pos] = target;
            target->parent->children[pos + 1] = nsibling;
            if (target->parent->active_keys == N)
            {
                split_push_up(target->parent, target->parent->key[N / 2]);
            }
        }
    }
    void insert_key(T key)
    {
        if (!root)
        {
            root = new Node;
            root->key[0] = key;
            leaf_start = root;
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
                while (i < p->active_keys && Compare()(p->key[i], key)) //Use compare
                    ++i;
                target = p;
                p = p->children[i];
            }
            insert_key_node_at(key, target, i);
            if (target->active_keys == N)
            {
                split_push_up(target, target->key[N / 2]);
            }
        }
    }
    void print_tree(Node *root)
    {
        if (root)
        {
            cout << root << "\n";
            for (auto i : root->children)
                cout << i << "\t";
            cout << "\n\n";
            for (auto i : root->children)
                print_tree(i);

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
    public:
        using value_type = T;
        using iterator_category = bidirectional_iterator_tag;
        //operator++, etc
        // iterator() {}
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
        }
#if DEBUG
        print_tree(root);
#endif
    }

    //copy ctor
    B_Plus_tree(const B_Plus_tree<T, N> &) {}

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
    B_Plus_tree<T, N, Compare, Alloc> &operator=(const B_Plus_tree<T, N, Compare, Alloc> &) {}

    //move ctor
    B_Plus_tree(B_Plus_tree<T, N, Compare, Alloc> &&) {}

    //move ass
    B_Plus_tree<T, N, Compare, Alloc> &operator=(B_Plus_tree<T, N, Compare, Alloc> &&) {}

    //dtor
    ~B_Plus_tree()
    {
        delete_tree(root);
    }

    pair<iterator, bool> insert(T key) {} // inserts elements
    void insert(std::initializer_list<T> l) {}
    template <typename it>
    void insert(it begin, it end) {}

    iterator delete_key(T key) {}
    iterator delete_key(iterator it) {}
    void delete_key(iterator begin, iterator end) {}

    void clear() {}

    iterator find(T key) {}
    iterator begin() {}
    // // ...
    // // begin,end => in_begin,in_end
    // // rbegin, rend

    // // Performing a range query with k elements
    // // find_if
    int size();
};

#endif