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
}
