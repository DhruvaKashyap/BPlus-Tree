#include <iostream>
using namespace std;
#include "bplus.h"
#include <vector>
template <typename T>
void f(T a)
{
    typename T::iterator::value_type b(3);
}

int main()
{
    const int a(2);
    // B_Plus_tree<int, a> b;
    // B_Plus_tree<int, 2> c = {1, 2};
    // f(c);
    // B_Plus_tree<int, 2>::iterator::value_type xx(3);
    // vector<int>::value_type xxx(3);
    // vector<int>::iterator::value_type xxxxx(3);
    // iterator_traits<vector<int>::iterator>::iterator_category v;
    // c.insert({1,2,3});
    // it=c.find(2);
    // c.delete(it)
    // *it;
}
