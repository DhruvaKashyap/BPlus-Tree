#include <iostream>
using namespace std;
#include "bplus.h"
#include <vector>

int main()
{
#if DEBUG
    cout << "DEBUGGING TURNED ON\n";
#endif
    B_Plus_tree<int, 8> c = {1, 2, 3, 4, 5, 6, 7, 8};
}
