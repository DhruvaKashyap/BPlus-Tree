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
    B_Plus_tree<int, 3> b({8, 2, 3, 6, 1, 4, 7, 5});
    for (auto i : b)
        cout << i << "\t";
    cout << "\n";

    for (int i = 1; i <= 8; i++)
    {
        b.delete_key_temp(i);
        for (auto j : b)
            cout << j << "\t";
        cout << "\n";
    }
    cout << "Final\n";
    for (auto i : b)
        cout << i << '\t';
    cout << "\n";
    // auto it = b.end();
    for (auto it = b.find(8); it != b.begin(); --it)
    {
        cout << "hey!\n";
        cout << *it << "\t";
    }
    // auto rit = b.rend();
    // cout<<*rit<<"\n";
    // for (auto i = b.rbegin();
    //     i != b.rend(); ++i ) {
    //         cout<<*i<<"\t";
    //         if(*i==1)
    //         {
    //             break;
    //         }
    // }
    cout << "\n";

    B_Plus_tree<int, 5> bt({1,2,3,4,5,6});
    cout<<"test\n";
    bt.delete_key_temp(8);
    bt.delete_key_temp(2);
}
