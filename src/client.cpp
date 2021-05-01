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
#if DEBUG
    cout << "DEBUGGING TURNED ON\n";
#endif
/*
    B_Plus_tree<int, 3> b({8, 2, 6, 1, 4, 7, 5});
    for (auto i : b)
    {
        cout << i << "\t";
        auto k = b.find(i);
        if(k!=b.end())
        {
            cout<<i<<" found\n";
        }
        else{
            cout<<"not found\n";
        }
    }
    auto x = b.find(2);
    // ++x;
    // cout<<*x<<"\n";
    // ++x;
    // cout<<*x<<"\n";
    // ++x;
    // cout<<*x<<"\n";
    // --x;
    // cout<<*x<<"\n";
    // --x;
    // cout<<*x<<"\n";
    // --x;
    // cout<<*x<<"\n";
    // --x;
    // cout<<"x:"<<*x<<"\n";
    // ++x;
    // cout<<*x<<"\n";
    // ++x;
    // cout<<*x<<"\n";
    // ++x;
    // cout<<*x<<"\n";
    // ++x;
    // cout<<*x<<"\n";
    x = b.find(100);
    if(x==b.end())
    {
        cout<<"nnot found\n";
    }
    // return 0;
    B_Plus_tree<int, 3> c(b);
    // B_Plus_tree<int, 3> c({8, 2, 3, 6, 1, 4, 7, 5});
    // c = b;
    cout<<"copied tree\n";
    for (auto i : c)
        cout << i << "\t";
    cout << "\n";
    cout<<boolalpha;
    cout<<"b==c is "<< (b==c) <<"\n";
    // b.clear();
    // b.delete_key_temp(6);
    b.insert(3);
    b.insert(10);
    c.insert(8);
    cout<<"copied tree after old tree is changed\n";
    for (auto i : c)
        cout << i << "\t";
    cout << "\n";
    cout<<"old tree\n";
    for (auto i : b)
        cout << i << "\t";
    cout << "\n";

    B_Plus_tree<int,3> d({4,5,6,0,9});
    cout<<"d:\n"; 
    for (auto i : d)
    {
        cout << i << "\t";
        auto k = d.find(i);
        if(k!=d.end())
        {
            cout<<i<<" found\n";
        }
    }
    cout << "\n";
    d = b;
    cout<<"after d=b\n";
    for (auto i : d)
        cout << i << "\t";
    cout << "\n";
    d.insert(25);
    d.insert(35);
    cout<<"updated d\n";
    for (auto i : d)
        cout << i << "\t";
    cout << "\n";
    cout<<"old b\n";
    for (auto i : b)
        cout << i << "\t";
    cout << "\n";
    B_Plus_tree<int,3> e({4,5});
    B_Plus_tree<int,3> f (e);
    cout<<"f\n";
    for (auto i : f)
        cout << i << "\t";
    cout << "\n"; 
    f.delete_key_temp(4);
    cout<<"f after 4 is deleted\n";
    for (auto i : f)
        cout << i << "\t";
    cout << "\n"; 
    cout<<"e after 4 is deleted in f\n";
    for (auto i : e)
        cout << i << "\t";
    cout << "\n"; 

    B_Plus_tree<int,3> g(move(e));
    cout<<"g:\n";
    for (auto i : g)
        cout << i << "\t";
    cout << "\n"; 
    cout<<"e:\n";
    for (auto i : e)
        cout << i << "\t";
    cout << "\n"; 
    f = move(b);
    cout<<"f after move(b):\n";
    for (auto i : f)
        cout << i << "\t";
    cout << "\n"; 
    cout<<"d.size(): "<<d.size()<<"\n";

    
    // B_Plus_tree<int, 3> b({8, 2});
    // for (auto i : b)
    //     cout << i << "\t";
    // cout << "\n";
    // B_Plus_tree<int, 3> c(b);
    // // B_Plus_tree<int, 3> c({8, 2, 3, 6, 1, 4, 7, 5});
    // cout<<"copied tree\n";
    // for (auto i : c)
    //     cout << i << "\t";
    // cout << "\n";
    // // b.clear();
    // // b.delete_key_temp(6);
    // b.insert(20);
    // b.insert(50);
    // cout<<"copied tree after old tree is changed\n";
    // for (auto i : c)
    //     cout << i << "\t";
    // cout << "\n";
    // cout<<"old tree\n";
    // for (auto i : b)
    //     cout << i << "\t";
    // cout << "\n";

    // for (int i = 1; i <= 8; i++)
    // {
    //     b.delete_key_temp(i);
    //     for (auto j : b)
    //         cout << j << "\t";
    //     cout << "\n";
    // }
    // cout << "Final\n";
    // for (auto i : b)
    //     cout << i << '\t';
    // cout << "\n";
    // // auto it = b.end();
    // for (auto it = b.find(8); it != b.begin(); --it)
    // {
    //     cout << "hey!\n";
    //     cout << *it << "\t";
    // }
    // // auto rit = b.rend();
    // // cout<<*rit<<"\n";
    // // for (auto i = b.rbegin();
    // //     i != b.rend(); ++i ) {
    // //         cout<<*i<<"\t";
    // //         if(*i==1)
    // //         {
    // //             break;
    // //         }
    // // }
    // cout << "\n";
*/
    B_Plus_tree<int, 3> bt({1,2,3,4,5,6});
    cout<<"test\n";
    cout << *bt.delete_key(3) << "\n";
    cout << *bt.delete_key(5) << "\n";
    cout << (bt.delete_key(6)==bt.end()) <<"\n";
    cout << (bt.delete_key(7)==bt.end()) <<"\n";
    cout << (bt.delete_key(4)==bt.end()) << "\n";
    cout << *bt.delete_key(1) << "\n";

    bt.clear();
    cout << (bt.delete_key(2)==bt.end()) << "\n";
}
