#include <iostream>
#include "bplus.h"
#include <vector>
using namespace std;

class CC
{
    int a;

public:
    // CC() : a(0){};
    CC(int s) : a(s){};
    friend ostream &operator<<(ostream &o, const CC &x)
    {
        return o << x.a;
    }
    friend bool operator<(const CC x1,const CC x2)
    {
        return x1.a < x2.a;
    }
};

int main()
{
#if DEBUG
    cout << "DEBUGGING TURNED ON\n";
#endif
    B_Plus_tree<int, 2> a({1, 2, 3, 4, 5, 6, 7, 8});
    B_Plus_tree<int, 3> b({1, 2, 3, 4, 5, 6, 7, 8});
    B_Plus_tree<int, 4> c({1, 2, 3, 4, 5, 6, 7, 8});
    B_Plus_tree<int, 5> d({1, 2, 3, 4, 5, 6, 7, 8});
    B_Plus_tree<int, 6> e({1, 2, 3, 4, 5, 6, 7, 8});
    B_Plus_tree<int, 7> f({1, 2, 3, 4, 5, 6, 7, 8});
    B_Plus_tree<int, 8> g({1, 2, 3, 4, 5, 6, 7, 8});
    B_Plus_tree<CC> kkk({1,1});
}
