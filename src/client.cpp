#include <iostream>
#include "bplus.h"
#include <vector>
#include <deque>
#include <set>
using namespace std;

class Date
{
private:
    int dd_;
    int mm_;
    int yy_;

public:
    Date(int dd, int mm, int yy)
        : dd_(dd), mm_(mm), yy_(yy)
    {
    }
    friend ostream &operator<<(ostream &o, const Date &d);
    friend bool operator<(const Date &lhs, const Date &rhs)
    {
        if (lhs.yy_ < rhs.yy_)
            return true;
        if (lhs.yy_ == rhs.yy_ && lhs.mm_ < rhs.mm_)
            return true;
        if (lhs.yy_ == rhs.yy_ && lhs.mm_ == rhs.mm_ && lhs.dd_ < rhs.dd_)
            return true;
        return false;
    }
    bool compare_month(const Date &rhs) const
    {
        return mm_ < rhs.mm_;
    }
};

ostream &operator<<(ostream &o, const Date &d)
{
    return o << d.dd_ << "-" << d.mm_ << "-" << d.yy_;
}

struct SameMonth
{
    bool operator()(const Date &lhs, const Date &rhs)
    {
        return lhs.compare_month(rhs);
    }
};

template <typename T>
void display(T container)
{
    copy(begin(container), end(container), ostream_iterator<typename T::iterator::value_type>(cout, "\t"));
    cout << '\n';
}
template <typename T>
void rev_display(T container)
{
    copy(rbegin(container), rend(container), ostream_iterator<typename T::value_type>(cout, "\t"));
    cout << '\n';
}

int main()
{
    cout << boolalpha;
    // Initializing a B+ tree using initializer lists

    B_Plus_tree<int, 3> a({8, 2, 6, 1, 4, 7, 5});
    cout << "tree of a\n";
    a.print_tree();

    cout << "a:\n";
    display(a);
    cout << "a.size(): " << a.size() << "\n";
    cout << "---------------------------------------------------------------\n\n\n\n";

    cout << "Using reverse iterators\n";
    rev_display(a);
    cout << "---------------------------------------------------------------\n\n\n\n";

    // Standard insert (insert(key))
    a.insert(200);
    a.insert(3);
    a.insert(25);

    cout << "inserted 3; 200; 25 to the tree\n";

    cout << "tree of a\n";
    a.print_tree();
    display(a);
    cout << "---------------------------------------------------------------\n\n\n\n";

    // Insert using a pair of iterators
    vector<int> myvec({14, 15, 16, 12, 13});

    a.insert(myvec.begin(), myvec.end());

    cout << "inserting elements using a pair of iterators\n";

    cout << "tree of a\n";
    a.print_tree();

    cout << "a:\n";
    display(a);
    cout << "---------------------------------------------------------------\n\n\n\n";

    // Delete
    a.delete_key(6);
    a.delete_key(7);
    a.delete_key(14);
    a.delete_key(200);

    // Deleting a key that does not exist
    a.delete_key(120);

    cout << "Deleting 6; 7; 14; 200\n";

    cout << "tree of a\n";
    a.print_tree();

    cout << "a:\n";
    display(a);
    cout << "a.size(): " << a.size() << "\n";
    cout << "---------------------------------------------------------------\n\n\n\n";

    // To show what the delete returns
    auto del_key = a.delete_key(12);
    cout << "Deleting 12; Iterator returns " << *del_key << "\n";

    cout << "\n\ntree of a\n";
    a.print_tree();

    // Deleting elements using a pair of iterators
    a.delete_key(myvec.begin(), myvec.end());
    cout << "Deleting elements using a pair of iterators\n";
    cout << "tree of a\n";
    a.print_tree();
    cout << "a:\n";
    display(a);
    cout << "---------------------------------------------------------------\n\n\n\n";

    // Clearing all elements
    cout << "Clear tree\n";
    a.clear();
    cout << "tree of a\n";
    a.print_tree();

    cout << "a.empty(): " << a.empty() << "\n";
    cout << "---------------------------------------------------------------\n\n\n\n";

    cout << "Clearing all elements and inserting 100; 400; 200; 300\n";
    a.insert(100);
    a.insert({400, 200, 300});
    a.print_tree();

    cout << "a:\n";
    display(a);
    cout << "a.size(): " << a.size() << "\n";
    cout << "---------------------------------------------------------------\n\n\n\n";

    // Member find function
    cout << "Finding 2\n";
    auto x = a.find(2);
    if (x != a.end())
    {
        cout << "2 found\n";
    }
    else
    {
        cout << "2 not found\n";
    }

    x = a.find(100);

    if (x != a.end())
    {
        cout << "100 found\n";
    }
    else
    {
        cout << "100 not found\n";
    }

    x = a.find(400);
    if (x != a.end())
    {
        cout << "400 found\n";
    }
    else
    {
        cout << "400 not found\n";
    }
    cout << "---------------------------------------------------------------\n\n\n\n";

    // Copy Constructor
    B_Plus_tree<int, 3> b(a);
    cout << "Copied tree b(a)\n";
    display(b);
    cout << "tree of a\n";
    a.print_tree();
    cout << "tree of b\n";
    b.print_tree();
    cout << "a==b is " << (a == b) << "\n";
    cout << "---------------------------------------------------------------\n\n\n\n";

    b.insert(3);
    b.insert(10);
    b.insert(8);
    cout << "3; 10; 8 added to b\n";
    cout << "tree of a\n";
    a.print_tree();
    cout << "tree of b\n";
    b.print_tree();
    cout << "Copied tree after Old tree is changed: b\n";
    display(b);
    cout << "Old tree a:\n";
    display(a);
    cout << "---------------------------------------------------------------\n\n\n\n";

    B_Plus_tree<int, 3> d({4, 5, 6, 0, 9});
    cout << "d:\n";
    display(d);

    // Copy Assignment Operator
    cout << "tree of d\n";
    d.print_tree();
    cout << "tree of b\n";
    b.print_tree();
    d = b;
    cout << "after d=b\n";
    cout << "tree of d\n";
    d.print_tree();
    cout << "tree of b\n";
    b.print_tree();
    display(d);
    cout << "---------------------------------------------------------------\n\n\n\n";

    d.insert(25);
    d.insert(35);
    cout << "Insert 25; 35 to d\n";
    d.print_tree();
    display(d);
    cout << "tree of b\n";
    b.print_tree();
    cout << "b:\n";
    display(b);
    cout << "b.size(): " << b.size() << "\n";
    cout << "---------------------------------------------------------------\n\n\n\n";

    B_Plus_tree<int> e({-4, 33, 64, 0, 29});
    // Move Ctor
    cout << "Move ctor\n";
    cout << "e (of degree 16):\n";
    e.print_tree();
    display(e);
    cout << "min element: " << *std::min_element(e.begin(), e.end()) << "\n";
    cout << "max element: " << *std::max_element(e.begin(), e.end()) << "\n";
    cout << "tree of e\n";
    e.print_tree();
    cout << "---------------------------------------------------------------\n\n\n\n";

    B_Plus_tree<int> g(move(e));
    cout << "tree of e\n";
    e.print_tree();
    cout << "tree of g\n";
    g.print_tree();
    cout << "g:\n";
    display(g);
    cout << "\n";
    cout << "---------------------------------------------------------------\n\n\n\n";

    auto find_x = std::find(g.begin(), g.end(), 0);
    cout << "STL Find\n";
    if (find_x != g.end())
    {
        cout << "0 found\n";
    }
    else
    {
        cout << "0 not found\n";
    }
    cout << "---------------------------------------------------------------\n\n\n\n";

    vector<int> k(b.size(), 0);
    std::copy(b.begin(), b.end(), k.begin());
    cout << "STL copy\n";
    display(k);
    cout << "---------------------------------------------------------------\n\n\n\n";

    cout << "Custom types\n";
    B_Plus_tree<Date> dates_tree({{11, 9, 2001},
                                  {26, 1, 2001},
                                  {11, 1, 1966},
                                  {30, 1, 1948},
                                  {26, 12, 2004}});
    B_Plus_tree<Date, 4, SameMonth> dates_tree_c({{11, 9, 2001},
                                                  {26, 1, 2001},
                                                  {11, 1, 1966},
                                                  {30, 1, 1948},
                                                  {26, 12, 2004}});
    dates_tree.print_tree();
    display(dates_tree);
    cout << "Finding 9/11\n";
    cout << *dates_tree.find({11, 9, 2001}) << "\n\n\n";
    cout << "Custom predicate\n";
    dates_tree_c.print_tree();
    display(dates_tree_c);
}
