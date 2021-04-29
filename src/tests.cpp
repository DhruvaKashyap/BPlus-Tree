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
    test<type, pp<type>, 3> ts;
    // ts.sortedInsertionTest();
    // ts.reverseSortedTest();
    // ts.randomInsertionTest();
    // ts.nonOccuringDelete();
    ts.RandomDelete();
    return 0;
}