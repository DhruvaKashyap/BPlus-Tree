#ifndef B_PLUS_H
#define B_PLUS_H
#include <initializer_list>
#include <functional>
#include <memory>
#include <algorithm>
using namespace std;
template <typename T, int N, typename Compare = less<T>, class Alloc = allocator<T>>
class B_Plus_tree
{
private:
    struct Node
    {
        T key[N];
        Node *children[N + 1];
        Node *parent;
        Node *next;
        Node *prev;
        int active_keys;
        Node()
        {
            for (auto &i : key)
                i = 0;
            for (auto &i : children)
                i = nullptr;
            next = prev = parent = nullptr;
            active_keys = 0;
        }
        void* operator new(size_t size)
        {
            return ::operator new(size);    //call allocator or something here
        }
        friend ostream &operator<<(ostream &o, Node *n)
        {
            if (!n)
                o << "(nullptr)";
            else
            {
                for (int i = 0; i < N; ++i)
                {
                    o << n->key[i] << "\t";
                }
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
        for (int j = N - 2; j >= i; --j)
            p->key[j + 1] = p->key[j];
        p->key[i] = key;
        p->active_keys++;
        return i;
    }
    void split_push_up(Node *target, T median)
    {
        Node *parent = target->parent;
        Node *nsibling = new Node;
        if (parent == nullptr)
        {
            // new root up
            Node *np = new Node;
            np->key[0] = median;
            np->active_keys = 1;
            target->parent = np;
            np->children[0] = target;
            np->children[1] = nsibling;
            nsibling->parent = np;
            bool internal = true;
            //refactor here
            //write lambda here
            for (auto i : target->children)
                if (!i)
                    internal = false;
            if (!internal)
            {
                // new leaf
                for (int l = N / 2; l < N; ++l)
                {
                    nsibling->key[l - N / 2] = target->key[l];
                    nsibling->children[l - N / 2] = target->children[l];
                    target->key[l] = 0; //do something about this
                    target->children[l] = nullptr;
                }
                nsibling->children[N / 2 + N % 2] = target->children[N];
                nsibling->active_keys = N / 2 + N % 2;
                target->next = nsibling;
                nsibling->prev = target;
                nsibling->prev = target;
            }
            //new internal
            else
            {
                target->key[N / 2] = 0;
                for (int l = N / 2 + 1; l < N; ++l)
                {
                    nsibling->key[l - N / 2 - 1] = target->key[l];
                    nsibling->children[l - N / 2 - 1] = target->children[l];
                    nsibling->children[l - N / 2 - 1]->parent = nsibling;
                    target->key[l] = 0;
                    target->children[l] = nullptr;
                }
                nsibling->children[N / 2] = target->children[N];
                nsibling->children[N / 2]->parent = nsibling;
                nsibling->active_keys = N / 2 + N % 2 - 1;
            }
            target->children[N] = nullptr;
            target->active_keys = N / 2;
            root = np;
        }
        // new leaf
        else
        {
            // add to target->parent
            // if full, push up again
            int pos = insert_key_node_at(median, target->parent);
            // create split
            target->next = nsibling;
            nsibling->parent = target->parent;
            for (int l = N / 2; l < N; ++l)
            {
                nsibling->key[l - N / 2] = target->key[l];
                target->key[l] = 0;
            }
            target->active_keys = N / 2;
            nsibling->active_keys = N / 2 + N % 2;
            target->parent->children[pos] = target;
            target->parent->children[pos + 1] = nsibling;
            nsibling->parent = target->parent;
            if (target->parent->active_keys == N)
                split_push_up(target->parent, parent->key[N / 2]); //new median
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
                T median = target->key[N / 2];
                split_push_up(target, median);
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

public:
    class iterator
    {
    public:
        using value_type = T;
        using iterator_category = bidirectional_iterator_tag;
        //operator++, etc
        iterator() {}
    };
    // traits
    // iters
    // ctors
    // more traits to be added
    using value_type = T;

    B_Plus_tree() {}

    B_Plus_tree(std::initializer_list<T> l)
    {
        for (T i : l)
        {
            // cout << i << "\n";
            insert_key(i);
        }
        print_tree(root);
        cout << "\n\n\n\n\n";
    }

    //copy ctor
    B_Plus_tree(const B_Plus_tree<T, N> &) {}

    //stl copy-like ctor
    template <typename it>
    B_Plus_tree(it begin, it end) {}

    //ass ctor
    B_Plus_tree<T, N, Compare, Alloc> &operator=(const B_Plus_tree<T, N, Compare, Alloc> &) {}

    //move ctor
    B_Plus_tree(B_Plus_tree<T, N, Compare, Alloc> &&) {}

    //move ass
    B_Plus_tree<T, N, Compare, Alloc> &operator=(B_Plus_tree<T, N, Compare, Alloc> &&) {}

    //dtor
    ~B_Plus_tree() {}

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