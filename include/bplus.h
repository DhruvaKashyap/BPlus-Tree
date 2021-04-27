#ifndef B_PLUS_H
#define B_PLUS_H
#ifndef DEBUG
#define DEBUG 0
#endif
#include <initializer_list>
#include <functional>
#include <memory>
#include <algorithm>
// #include <concepts>
#include <iterator>
#include <vector>
#include <array>
#include <iostream>
#include <cassert>
#include <cstring>
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
public:
    class iterator;
    class reverse_iterator;

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
        Node &operator=(const Node &rhs)
        {
            cout << "operator= called\n";
            if (this != rhs)
            {
                ;
            }
            return *this;
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
                o << "(" << n->active_keys << "," << n->is_leaf << ")\t";
            }
            return o;
        }
    };
    int __degree = N;
    Node *root = nullptr;
    Node *leaf_start = nullptr;
    Node *leaf_end = nullptr;
    size_t nums = 0;

    void myMerge(Node *left, Node *right, int leftNodePos)
    {
        if (!left->is_leaf)
        {
            left->key[left->active_keys] = left->parent->key[leftNodePos];
            left->active_keys++;
        }

        for (int j = 0; j < right->active_keys; ++j)
        {
            left->key[left->active_keys + j] = right->key[j];
            left->children[left->active_keys + j] = right->children[j];
        }
        left->active_keys += right->active_keys;
        left->children[left->active_keys] = right->children[right->active_keys];

        for (int j = leftNodePos; j < left->parent->active_keys - 1; ++j)
        {
            left->parent->key[j] = left->parent->key[j + 1];
            left->parent->children[j + 1] = left->parent->children[j + 2];
        }
        left->parent->children[left->parent->active_keys] = nullptr;
        left->parent->active_keys--;

        for (int i = 0; i <= left->active_keys && left->children[i]; ++i)
        {
            left->children[i]->parent = left;
        }

        left->next = right->next;
        if (left->next)
        {
            left->next->prev = left;
        }

        delete right;
        cout << "merge\n";
    }

    void reDistribute(Node *left, Node *right, int leftNodePos, int curr)
    {
        if (curr == 0)
        {
            if (left->is_leaf)
            {
                left->key[left->active_keys] = right->key[0];
            }
            else
            {
                left->key[left->active_keys] = left->parent->key[leftNodePos];
            }
            left->active_keys++;
            left->children[left->active_keys] = right->children[0];
            if (left->is_leaf)
            {
                left->parent->key[leftNodePos] = right->key[1]; 
            }
            else
            {
                left->parent->key[leftNodePos] = right->key[0]; 
            }
            for (int j = 0; j < right->active_keys - 1; ++j)
            {
                right->key[j] = right->key[j + 1];
                right->children[j] = right->children[j + 1];
            }
            right->children[right->active_keys - 1] = right->children[right->active_keys];
            right->active_keys--;
        }
        else
        {
            for (int j = right->active_keys - 1; j >= 0; j--)
            {
                right->key[j + 1] = right->key[j];
                right->children[j + 2] = right->children[j + 1];
            }
            right->children[1] = right->children[0];
            if (left->is_leaf)
            {
                right->key[0] = left->key[left->active_keys - 1];
            }
            else
            {
                right->key[0] = left->parent->key[leftNodePos];
            }
            right->active_keys++;
            right->children[0] = left->children[left->active_keys];
            left->children[left->active_keys] = nullptr;
            left->parent->key[leftNodePos] = left->key[left->active_keys - 1];
            left->active_keys--;
        }
        cout << "redistribute\n";
    }

    void delete_rec(Node *node, T key, int nodePos)
    {
        int flag = 0;
        //int idk = node->key[0];

        if (!node->is_leaf)
        {
            for (int i = 0; i < node->active_keys; ++i)
            {
                if (Compare()(key, node->key[i]))
                {
                    delete_rec(node->children[i], key, i);
                    flag = 1;
                    break;
                }
            }
            if (!flag)
            {
                delete_rec(node->children[node->active_keys], key, node->active_keys);
            }
        }
        else
        {
            for (int i = 0; i < node->active_keys; ++i)
            {
                if (!Compare()(key, node->key[i]) && !Compare()(node->key[i], key))
                {
                    for (int j = i; j < node->active_keys - 1; ++j)
                    {
                        node->key[j] = node->key[j + 1];
                    }
                    node->active_keys--;
                    break;
                }
            }
        }

        if (node->parent == nullptr)
        {
            if (node->is_leaf)
            {
                if (!node->active_keys)
                {
                    delete node;
                    leaf_start = leaf_end = root = nullptr;
                }
                return;
            }
            else
            {
                if (!node->active_keys)
                {
                    root = node->children[0];
                    delete node;
                    root->parent = nullptr;
                    return;
                }
            }
        }

        if (node->parent && node->active_keys < ((N + 1) / 2 - 1))
        {
            Node *n1;
            Node *n2;
            Node* nb;
            int i1, i2;
            if (nodePos == 0)
            {
                nb = node->parent->children[1];
                n1 = node;
                n2 = node->parent->children[1];
                i1 = 0;
                i2 = 0;
            }
            else
            {
                nb = node->parent->children[nodePos - 1];
                n1 = node->parent->children[nodePos - 1];
                n2 = node;
                i1 = nodePos - 1;
                i2 = 1;
            }

            if ((node->is_leaf && nb->active_keys <= (N / 2 + 1)) ||
                (!node->is_leaf && nb->active_keys <= N / 2))
            {
                myMerge(n1, n2, i1);
            }
            else
            {
                reDistribute(n1, n2, i1, i2);
            }
        }
        // where are the deletions in internal nodes happening?
        //TODO - test redistribute
        /*TODO
        Node* temp = node->parent;
        while(temp)
        {
            for(int i=0; i<temp->active_keys; ++i)
            {
                if(!Compare()(temp->key[i], idk) && !Compare()(idk, temp->key[i]))
                {
                    temp->key[i] = node->key[0];
                    break;
                }
            }
            temp = temp->parent;
        }
        */
    }

    int insert_key_node_at(T key, Node *p, int loc = 0)
    {
        int i(loc);
        while (i < p->active_keys && Compare()(p->key[i], key))
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
            target->parent->children[0] = target;
        }
        else
        {
            pos = insert_key_node_at(median, target->parent);
            // rotate(target->children + pos, target->children + N , target->children + N + 1);
            for (int j = N; j > pos; --j)
                target->parent->children[j] = target->parent->children[j - 1];
        }
        nsibling->parent = target->parent;
        target->parent->children[pos + 1] = nsibling;
        if (target->is_leaf)
        {
            // should median placement depend on predicate?
            // if yes change here and target->active_keys
            copy(std::begin(target->key) + N / 2, std::begin(target->key) + N, std::begin(nsibling->key)); //end begin no work bottleneck if vector
            if (target->next)
                target->next->prev = nsibling;
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
    pair<iterator, bool> insert_key(T key)
    {
        if (!root)
        {
            root = new Node;
            root->key[0] = key;
            leaf_start = root;
            leaf_end = root;
            root->active_keys = 1;
            root->is_leaf = true;
            ++nums;
            return make_pair(iterator(root, leaf_end, 0), false);
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
                if (i < p->active_keys && !Compare()(p->key[i], key) && !Compare()(key, p->key[i]))
                {
                    return make_pair(iterator(p, leaf_end, i), false); //return iterator(p,leaf_end,i),false
                }
                target = p;
                p = p->children[i];
            }
            insert_key_node_at(key, target, i);
            if (target->active_keys == N)
            {
                split_push_up(target, target->key[N / 2]);
            }
            ++nums;
            return make_pair(iterator(p, leaf_end, i), false);
        }
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
    class iterator : public std::iterator<std::bidirectional_iterator_tag, T>
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
            // cout<<"operator--\n";
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

    class reverse_iterator : public iterator
    {
    protected:
        reverse_iterator(Node *n, Node *e, int j) : iterator{n, e, j}
        {
        }
        friend class B_Plus_tree<T, N, Compare, Alloc>;

    public:
        iterator &operator++()
        {
            return iterator::operator--();
        }
        iterator operator++(int)
        {
            // return it--;
            return iterator::operator--(1);
        }
        reverse_iterator &operator--()
        {
            // return ++i;
            return iterator::operator++();
        }
        reverse_iterator operator--(int)
        {
            // return ++i;
            return iterator::operator++(1);
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
    B_Plus_tree(const B_Plus_tree<T, N> &copy)
    {
        __degree = copy.__degree;
        root = new Node;
        std::memcpy(root, copy.root, sizeof(Node *));
        nums = copy.nums;
        // recursive_copy(copy.root);
    }

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

    pair<iterator, bool> insert(T key)
    {
        return insert_key(key);
    } // inserts elements
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

    void delete_key_temp(T key)
    {
        delete_rec(root, key, 0);
        print_tree(root);
    }

    iterator delete_key(T key) {}
    iterator delete_key(iterator it) {}
    void delete_key(iterator begin, iterator end) {}

    void clear()
    {
        delete_tree(root);
        leaf_start = root = nullptr;
    }

    iterator find(T key)
    {
        Node *temp = root;
        int i;
        while (temp)
        {
            i = 0;
            while (i < temp->active_keys && temp->key[i] <= key)
            {
                if (temp->key[i] == key && temp->is_leaf)
                {
                    return iterator(temp, leaf_end, i);
                }
                ++i;
            }
            temp = temp->children[i];
        }
        return iterator(nullptr, leaf_end, 0);
    }

    iterator begin()
    {
        return iterator(leaf_start, leaf_end, 0);
    }
    iterator end()
    {
        return iterator(nullptr, leaf_end, 0);
    }
    reverse_iterator rbegin()
    {
        return reverse_iterator(leaf_end, nullptr, 0);
    }
    reverse_iterator rend()
    {
        // return reverse_iterator(iterator(leaf_start, leaf_end, 0));
        return reverse_iterator(nullptr, leaf_start, 0);
    }
    // // ...
    // // begin,end => in_begin,in_end
    // // rbegin, rend

    // // Performing a range query with k elements
    // // find_if
    int size();
};

#endif