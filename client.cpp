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
    // B_Plus_tree<int, 3> c1 = {1,2,3};
    // cout<<"\n";
    // B_Plus_tree<int, 3> c2 = {1,3,2};
    // cout<<"\n";
    // B_Plus_tree<int, 3> c3 = {2,1,3};
    // cout<<"\n";
    // B_Plus_tree<int, 3> c4 = {2,3,1};
    // cout<<"\n";
    // B_Plus_tree<int, 3> c5 = {3,1,2};
    // cout<<"\n";
    // B_Plus_tree<int, 3> c6 = {3,2,1};
    // cout<<"\n";
    B_Plus_tree<int,8> c = {1,2,3,4,5,6,7,8};
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
