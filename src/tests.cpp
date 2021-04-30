#include <iostream>
#include "bplus.h"
#include "tests.h"
using namespace std;

template <typename T>
struct pp
{
    bool operator()(const T lhs, const T rhs) const
    {
        return lhs < rhs;
    }
};
using type = int;
int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    test<type, pp<type>> ts;
    // ts.sortedInsertionTest();
    // ts.reverseSortedTest();
    // ts.randomInsertionTest();
    // ts.nonOccuringDelete();
    // ts.RandomDelete();
    ts.insertdelete();
    return 0;
}