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
#include "concepts.h"
using namespace std;

template <int N>
concept BPLUSMIN = BPLUSREQ<N>::value;

template <typename T, int N = BPLUSVAL<T>::value, typename Compare = less<T>, class Alloc = allocator<T>>
requires BPLUSMIN<N> class B_Plus_tree
{
public:
    class bpiterator;
    class bpriterator;

private:
    struct Node
    {
        T *key; // 78896 could be a vector. could keep ptrs to keys to allow non default ctorable, more mem usage
        // vector<T> key; //80384 allows non default constructable
        // array<T, N> key; //78896
        Node *children[N + 1];
        Node *parent = nullptr;
        Node *next = nullptr;
        Node *prev = nullptr;
        int active_keys = 0;
        bool is_leaf = false;
        Alloc &a;
        Node(Alloc x) : a(x)
        {
            // key.reserve(N);
            key = a.allocate(N);
            fill(children, children + N + 1, nullptr);
        }
        ~Node()
        {
            a.deallocate(key, N);
        }
        Node(const Node &n) = delete;
        Node &operator=(const Node &rhs) = delete;
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
                copy(n->key, n->key + n->active_keys, ostream_iterator<T>(o, "\t"));
                // copy(std::begin(n->key), std::begin(n->key) + n->active_keys, ostream_iterator<T>(o, "\t"));
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
    Alloc alloc = Alloc();

    Node *myMerge(Node *left, Node *right, int leftNodePos)
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

        if (leaf_end == right)
            leaf_end = left;
        delete right;
        return left;
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
            if (left->children[left->active_keys])
                left->children[left->active_keys]->parent = left;

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
            right->children[right->active_keys] = nullptr;

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
            if (right->children[0])
                right->children[0]->parent = right;
            left->children[left->active_keys] = nullptr;

            left->parent->key[leftNodePos] = left->key[left->active_keys - 1];
            left->active_keys--;
        }
    }

    bpiterator delete_rec(Node *node, T key, int nodePos)
    {
        int flag = 0;
        int res;
        int resExists = 0;
        bpiterator it(nullptr, leaf_end, 0);
        if (!node)
            return it;
        int firstKey = node->key[0];

        if (!node->is_leaf)
        {
            for (int i = 0; i < node->active_keys; ++i)
            {
                if (Compare()(key, node->key[i]))
                {
                    it = delete_rec(node->children[i], key, i);
                    flag = 1;
                    break;
                }
            }
            if (!flag)
            {
                it = delete_rec(node->children[node->active_keys], key, node->active_keys);
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

                    if (i < node->active_keys)
                    {
                        res = node->key[i];
                        resExists = 1;
                    }
                    else
                    {
                        if (node->next)
                        {
                            res = node->next->key[0];
                            resExists = 1;
                        }
                    }

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
                if (resExists)
                {
                    it = find(res);
                }
                return it;
            }
            else
            {
                if (!node->active_keys)
                {
                    root = node->children[0];
                    delete node;
                    root->parent = nullptr;
                    return it;
                }
            }
        }

        if (node->parent && node->active_keys < ((N + 1) / 2 - 1))
        {
            Node *n1;
            Node *n2;
            Node *nb;
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
                node = myMerge(n1, n2, i1);
            }
            else
            {
                reDistribute(n1, n2, i1, i2);
            }
        }

        if (node->is_leaf)
        {
            Node *temp = node->parent;
            while (temp)
            {
                for (int i = 0; i < temp->active_keys; ++i)
                {
                    if (!Compare()(temp->key[i], firstKey) && !Compare()(firstKey, temp->key[i]))
                    {
                        temp->key[i] = node->key[0];
                        break;
                    }
                }
                temp = temp->parent;
            }
        }

        if (resExists)
        {
            it = find(res);
        }
        return it;
    }

    int insert_key_node_at(T key, Node *p, int loc = 0)
    {
        int i(loc);
        while (i < p->active_keys && Compare()(p->key[i], key))
            ++i;
        rotate(p->key + i, p->key + N - 1, p->key + N);
        // rotate(std::begin(p->key) + i, std::begin(p->key) + N - 1, std::begin(p->key) + N);
        p->key[i] = key;
        p->active_keys++;
        return i;
    }

    void split_push_up(Node *target, T median)
    {
        Node *nsibling = new Node(alloc);
        nsibling->is_leaf = target->is_leaf;
        nsibling->next = target->next;
        nsibling->active_keys = N / 2 + N % 2;
        int pos(0);
        if (target->parent == nullptr)
        {
            Node *np = new Node(alloc);
            np->key[0] = median;
            np->active_keys = 1;
            root = target->parent = np;
            ++nums;
            target->parent->children[0] = target;
        }
        else
        {
            pos = insert_key_node_at(median, target->parent);
            rotate(target->parent->children + pos + 1, target->parent->children + N, target->parent->children + N + 1); // faster than copy
        }
        nsibling->parent = target->parent;
        target->parent->children[pos + 1] = nsibling;
        if (target->is_leaf)
        {
            // should median placement depend on predicate?
            // if yes change here and target->active_keys
            copy((target->key) + N / 2, (target->key) + N, (nsibling->key));
            // copy(std::begin(target->key) + N / 2, std::begin(target->key) + N, std::begin(nsibling->key));
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
                copy((target->key) + N / 2 + 1, (target->key) + N, (nsibling->key));
                // copy(std::begin(target->key) + N / 2 + 1, std::begin(target->key) + N, std::begin(nsibling->key));
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

    pair<bpiterator, bool> insert_key(T key)
    {
        if (!root)
        {
            root = new Node(alloc);
            root->key[0] = key;
            leaf_start = root;
            leaf_end = root;
            root->active_keys = 1;
            root->is_leaf = true;
            ++nums;
            return make_pair(bpiterator(root, leaf_end, 0), true);
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
                    return make_pair(bpiterator(p, leaf_end, i), false); //return bpiterator(p,leaf_end,i),false
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
            return make_pair(bpiterator(p, leaf_end, i), true);
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

    void recursive_copy(Node *src, Node *dst, Node **nr)
    {
        if (src)
        {
            dst->active_keys = src->active_keys;
            dst->is_leaf = src->is_leaf;
            copy(src->key, src->key + N, dst->key);
            // copy(std::begin(src->key), std::begin(src->key) + N, dst->key);
            if (src->is_leaf)
            {
                if (src->prev == nullptr)
                {
                    leaf_start = dst;
                }
                else
                {
                    dst->prev = *nr;
                    (*nr)->next = dst;
                }
                if (src->next == nullptr)
                {
                    leaf_end = dst;
                }
                *nr = dst;
            }
            int i = 0;
            while (src->children[i] != nullptr)
            {
                dst->children[i] = new Node(alloc);
                recursive_copy(src->children[i], dst->children[i], nr);
                dst->children[i]->parent = dst;
                ++i;
            }
        }
    }

public:
    class bpiterator
    {
        Node *ptr;
        Node *end;
        int index;
        bpiterator(Node *n, Node *e, int i) : ptr(n), end(e), index(i)
        {
        }
        friend class B_Plus_tree<T, N, Compare, Alloc>;

    public:
        using value_type = T;
        using iterator_category = bidirectional_iterator_tag;
        using difference_type = ptrdiff_t;
        using pointer = T *;
        using reference = const T &;
        friend bool operator==(const bpiterator &lhs, const bpiterator &rhs)
        {
            return lhs.ptr == rhs.ptr && lhs.index == rhs.index;
        }
        friend bool operator!=(const bpiterator &lhs, const bpiterator &rhs)
        {
            return !(lhs == rhs);
        }
        bpiterator &operator++()
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
        bpiterator operator++(int)
        {
            bpiterator temp(*this);
            ++*this;
            return temp;
        }
        bpiterator &operator--()
        {
            if (ptr)
            {
                if (index > 0)
                {
                    --index;
                }
                else
                {
                    ptr = ptr->prev;
                    index = ptr ? ptr->active_keys - 1 : 0;
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
        bpiterator operator--(int)
        {
            bpiterator temp(*this);
            --*this;
            return temp;
        }
        reference operator*()
        {
            return ptr->key[index];
        }
    };

    class bpriterator
    {
        bpiterator base;

    protected:
        bpriterator(Node *n, Node *e, int j) : base{n, e, j}
        {
        }
        friend class B_Plus_tree<T, N, Compare, Alloc>;

    public:
        using value_type = T;
        using iterator_category = bidirectional_iterator_tag;
        using difference_type = ptrdiff_t;
        using pointer = T *;
        using reference = const T &;
        friend bool operator==(const bpriterator &lhs, const bpriterator &rhs)
        {
            return lhs.base == rhs.base;
        }
        friend bool operator!=(const bpriterator &lhs, const bpriterator &rhs)
        {
            return !(lhs == rhs);
        }
        bpriterator &operator++()
        {
            --base;
            return *this;
        }
        bpriterator operator++(int)
        {
            bpriterator temp(*this);
            --base;
            return temp;
        }
        bpriterator &operator--()
        {
            ++base;
            return *this;
        }
        bpriterator operator--(int)
        {
            bpriterator temp(*this);
            ++base;
            return temp;
        }
        reference operator*()
        {
            return *base;
        }
    };

    using value_type = T;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using value_compare = Compare;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = std::allocator_traits<Alloc>::pointer;
    using iterator = bpiterator;
    using reverse_iterator = bpriterator;
    using const_iterator = const iterator;
    using const_reverse_iterator = const reverse_iterator;

    B_Plus_tree()
    {
    }

    explicit B_Plus_tree(std::initializer_list<T> l)
    {
        for (T i : l)
        {
            insert_key(i);
        }
    }

    B_Plus_tree(const B_Plus_tree<T, N> &copy)
    {
        if (copy.root)
        {
            __degree = copy.__degree;
            nums = copy.nums;
            root = new Node(alloc);
            Node *temp;
            recursive_copy(copy.root, root, &temp);
        }
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

    B_Plus_tree<T, N, Compare, Alloc> &operator=(const B_Plus_tree<T, N, Compare, Alloc> &rhs)
    {
        if (this != &rhs)
        {
            delete_tree(root);
            __degree = rhs.__degree;
            nums = rhs.nums;
            if (rhs.root)
            {
                root = new Node(alloc);
                Node *temp;
                recursive_copy(rhs.root, root, &temp);
            }
        }
        return *this;
    }

    //move ctor
    B_Plus_tree(B_Plus_tree<T, N, Compare, Alloc> &&copy)
    {

        root = copy.root;
        copy.root = nullptr;
        __degree = copy.__degree;
        nums = copy.nums;
        leaf_start = copy.leaf_start;
        leaf_end = copy.leaf_end;
        copy.leaf_end = nullptr;
        copy.leaf_start = nullptr;
    }

    //move ass
    B_Plus_tree<T, N, Compare, Alloc> &operator=(B_Plus_tree<T, N, Compare, Alloc> &&rhs)
    {

        if (this != &rhs)
        {
            delete_tree(root);
            root = rhs.root;
            rhs.root = nullptr;

            __degree = rhs.__degree;
            nums = rhs.nums;

            leaf_start = rhs.leaf_start;
            leaf_end = rhs.leaf_end;
            rhs.leaf_start = nullptr;
            rhs.leaf_end = nullptr;
        }
        return *this;
    }

    //dtor
    ~B_Plus_tree()
    {
        delete_tree(root);
    }

    pair<iterator, bool> insert(T key)
    {
        return insert_key(key);
    }

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

    iterator delete_key(T key)
    {
        return delete_rec(root, key, 0);
    }

    iterator delete_key(iterator it)
    {
        return delete_rec(root, *it, 0);
    }

    void delete_key(iterator begin, iterator end)
    {
        for (iterator it = begin; it != end; ++it)
        {
            delete_rec(root, *it, 0);
        }
    }

    void clear()
    {
        delete_tree(root);
        leaf_start = root = nullptr;
    }

    iterator find(T key) const
    {
        Node *temp = root;
        int i(0);
        while (temp)
        {
            i = 0;
            while (i < temp->active_keys && !Compare()(key, temp->key[i])) //predicates
            {
                if (temp->is_leaf && !Compare()(temp->key[i], key) && !Compare()(key, temp->key[i]))
                {
                    return iterator(temp, leaf_end, i);
                }
                ++i;
            }
            temp = temp->children[i];
        }
        return iterator(nullptr, leaf_end, 0);
    }

    iterator begin() const
    {
        return iterator(leaf_start, leaf_end, 0);
    }

    iterator end() const
    {
        return iterator(nullptr, leaf_end, 0);
    }

    const_iterator cbegin() const
    {
        return iterator(leaf_start, leaf_end, 0);
    }

    const_iterator cend() const
    {
        return iterator(nullptr, leaf_end, 0);
    }

    reverse_iterator rbegin() const
    {
        return reverse_iterator(leaf_end, leaf_start, leaf_end->active_keys - 1);
    }

    reverse_iterator rend() const
    {
        return reverse_iterator(nullptr, leaf_start, 0);
    }

    const_reverse_iterator crbegin() const
    {
        return reverse_iterator(leaf_end, leaf_start, leaf_end->active_keys - 1);
    }

    const_reverse_iterator crend() const
    {
        return reverse_iterator(nullptr, leaf_start, 0);
    }

    // std::reverse_iterator<iterator> rbegin()
    // {
    //     return std::reverse_iterator<iterator>(iterator(leaf_end, leaf_start, leaf_end->active_keys));
    // }
    // std::reverse_iterator<iterator> rend()
    // {
    //     return std::reverse_iterator<iterator>(iterator(leaf_start, leaf_start, 0));
    // }

    size_type size() const
    {
        return nums;
    }

    bool empty() const
    {
        return root == nullptr;
    }

    friend bool operator==(const B_Plus_tree<T, N, Compare, Alloc> &lhs, const B_Plus_tree<T, N, Compare, Alloc> &rhs)
    {
        auto lhs_it = lhs.begin();
        auto rhs_it = rhs.begin();
        while (lhs_it != lhs.end() && rhs_it != rhs.end() && *lhs_it == *rhs_it)
        {
            ++lhs_it;
            ++rhs_it;
        }
        return lhs_it == lhs.end() && rhs_it == rhs.end();
    }

    friend bool operator!=(const B_Plus_tree<T, N, Compare, Alloc> &lhs, const B_Plus_tree<T, N, Compare, Alloc> &rhs)
    {
        return !(lhs == rhs);
    }
};

#endif