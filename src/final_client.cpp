#include <iostream>
#include "bplus.h"
#include <vector>
#include <deque>
#include <set>
using namespace std;

class CC
{
    int a;

public:
    CC(int s) : a(s){};
    friend ostream &operator<<(ostream &o, const CC &x)
    {
        return o << x.a;
    }
    friend bool operator<(const CC x1, const CC x2)
    {
        return x1.a < x2.a;
    }
};

int main()
{
    // Initializing a B+ tree using initializer lists
    B_Plus_tree<int, 3> a({8, 2, 6, 1, 4, 7, 5});
    cout << "a:\n";
    for (auto i : a)
    {
        cout << i << "\t";
    }
    cout << "\n";
    cout << "a.size(): " << a.size() << "\n";
    cout << "Using reverse iterators\n";
    for (auto i = a.rbegin(); i != a.rend(); ++i)
    {
        cout << *i << "\t";
    }
    cout << "\n";
    // Standard insert (insert(key))
    a.insert(200);
    a.insert(3);
    a.insert(25);
    cout << "inserted 3,200,25 to the tree\n";
    for (auto i : a)
    {
        cout << i << "\t";
    }
    cout << "\n";

    // Insert using a pair of iterators
    vector<int> myvec({14, 15, 16, 12, 13});
    a.insert(myvec.begin(), myvec.end());
    cout << "inserting elements using a pair of iterators\n";
    cout << "a:\n";
    for (auto i : a)
    {
        cout << i << "\t";
    }
    cout << "\n";

    // Delete
    a.delete_key(6);
    a.delete_key(7);
    a.delete_key(14);
    a.delete_key(200);
    // Deleting a key that does not exist
    a.delete_key(120);
    cout << "Deleting 6,7,14,200\n";
    cout << "a:\n";
    for (auto i : a)
    {
        cout << i << "\t";
    }
    cout << "\n";

    // To show what the delete returns
    auto del_key = a.delete_key(12);
    cout << "Deleting 12; Iterator returns " << *del_key << "\n";

    // Deleting elements using a pair of iterators
    a.delete_key(myvec.begin(), myvec.end());
    cout << "Deleting elements using a pair of iterators\n";
    cout << "a:\n";
    for (auto i : a)
    {
        cout << i << "\t";
    }
    cout << "\n";

    // Clearing all elements
    a.clear();
    cout << boolalpha;
    cout << "a.empty(): " << a.empty() << "\n";
    a.insert(100);
    a.insert({400, 200, 300});
    cout << "Clearing all elements and inserting\n";
    cout << "a:\n";
    for (auto i : a)
    {
        cout << i << "\t";
    }
    cout << "\n";
    // Member find function
    auto x = a.find(2);
    if (x != a.end())
    {
        cout << 2 << " found\n";
    }
    else
    {
        cout << 2 << " not found\n";
    }
    x = a.find(100);
    if (x != a.end())
    {
        cout << 100 << " found\n";
    }
    else
    {
        cout << 100 << " not found\n";
    }
    x = a.find(400);
    if (x != a.end())
    {
        cout << 400 << " found\n";
    }
    else
    {
        cout << 400 << " not found\n";
    }
    // Copy Constructor
    B_Plus_tree<int, 3> b(a);
    cout << "Copied tree b(a)\n";
    for (auto i : b)
        cout << i << "\t";
    cout << "\n";
    cout << boolalpha;
    cout << "a==b is " << (a == b) << "\n";

    b.insert(3);
    b.insert(10);
    b.insert(8);
    cout << "Copied tree after Old tree is changed: b\n";
    for (auto i : b)
        cout << i << "\t";
    cout << "\n";
    cout << "Old tree a:\n";
    for (auto i : a)
        cout << i << "\t";
    cout << "\n";

    B_Plus_tree<int, 3> d({4, 5, 6, 0, 9});
    cout << "d:\n";
    for (auto i : d)
    {
        cout << i << "\t";
    }
    cout << "\n";
    // Copy Assignment Operator
    d = b;
    cout << "after d=b\n";
    for (auto i : d)
        cout << i << "\t";
    cout << "\n";
    d.insert(25);
    d.insert(35);
    cout << "updated d\n";
    for (auto i : d)
        cout << i << "\t";
    cout << "\n";
    cout << "b:\n";
    for (auto i : b)
        cout << i << "\t";
    cout << "\n";

    B_Plus_tree<int, 3> e({-4, 33, 64, 0, 29});
    // Move Ctor
    B_Plus_tree<int, 3> g(move(e));
    cout << "g:\n";
    for (auto i : g)
        cout << i << "\t";
    cout << "\n";

    
}
