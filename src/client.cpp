#include <iostream>
#include "bplus.h"
#include <vector>
using namespace std;

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
}
