#ifndef B_PLUS_H
#define B_PLUS_H
#include <initializer_list>
#include <functional>
#include <memory>
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
        // friend ostream &operator<<(ostream &o, Node *n);
    };

    int __degree = N;

public:
    class iterator
    {
    public:
        using value_type = T;
        using iterator_category = bidirectional_iterator_tag;
        //operator++, etc
    };
    //traits
    //iters
    //ctors
    using value_type = T;

    // B_Plus_tree() {}

    // B_Plus_tree(std::initializer_list<T> l) {}

    // //copy ctor
    // B_Plus_tree(const B_Plus_tree<T, N> &) {}

    // //stl copy-like ctor
    // template <typename it>
    // B_Plus_tree(it begin, it end) {}

    // //ass ctor
    // B_Plus_tree<T, N, Compare, Alloc> &operator=(const B_Plus_tree<T, N, Compare, Alloc> &) {}

    // //move ctor
    // B_Plus_tree(B_Plus_tree<T, N, Compare, Alloc> &&) {}

    // //move ass
    // B_Plus_tree<T, N, Compare, Alloc> &operator=(B_Plus_tree<T, N, Compare, Alloc> &&) {}

    // //dtor
    // ~B_Plus_tree() {}

    // pair<iterator, bool> insert(T key); // inserts elements
    // void insert(std::initializer_list<T> l);
    // template <typename it>
    // void insert(it begin, it end);

    // iterator delete_key(T key);
    // iterator delete_key(iterator it);
    // void delete_key(iterator begin, iterator end);

    // void clear();

    // iterator find(T key);

    // iterator begin();
    // // ...
    // // begin,end => in_begin,in_end
    // // rbegin, rend

    // // Performing a range query with k elements
    // // find_if
    // int size();
};

// template <>
// struct iterator_traits<B_Plus_tree::iterator>
// {

// };
#endif