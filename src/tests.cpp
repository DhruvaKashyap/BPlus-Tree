#include <iostream>
#include "bplus.h"
#include <vector>
#include <deque>
#include <set>
#include <random>
#include <algorithm>
#include <chrono>
using namespace std;

int main()
{
    vector<int> v;
    set<int> s;
    B_Plus_tree<int> b;
    const int N = 100;
    int T = 1;
    default_random_engine generator(chrono::system_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> distribution(-N, N);
    bool ok = true;
    long i(1);
    while (ok)
    {
        for (int j = 0; j < T; ++j)
        {
            v.push_back(distribution(generator));
        }
        // auto ip = unique(begin(v), end(v));
        // v.resize(std::distance(v.begin(), ip));
        for (auto i : v)
        {
            b.insert(i);
            s.insert(i);
        }
        // vector<int> temp(v);
        // for (auto i : v)
        //     cout << i << "\t";
        // cout << "\n";
        // sort(begin(v), end(v));
        ok = equal(begin(s), end(s), begin(b), end(b));
        // v = temp;
        if (ok)
            cout << "Test :" << i << "/" << T << " Passed\n";
        else
        {
            cout << "Test :" << i << "/" << T << " Failed\n";
            sort(begin(v), end(v));
            for (auto i : v)
                cout << i << "\t";
            cout << "\n";
            for (auto i : s)
                cout << i << "\t";
            cout << "\n";
            for (auto i : b)
                cout << i << "\t";
            cout << "\n";
            return 0;
        }
        ++i;
        if (!(i % 100000))
        {
            ++T;
            i = 1;
        }
        v.clear();
        b.clear();
        s.clear();
    }
}