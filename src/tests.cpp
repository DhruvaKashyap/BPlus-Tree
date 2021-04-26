#include <iostream>
#include "bplus.h"
#include <vector>
#include <deque>
#include <set>
#include <random>
#include <algorithm>
#include <chrono>
using namespace std;
const int N = 100000;
const int trials = 1000;
const int Tmin = 1;
const int Tmax = 1000;
constexpr int d = 8;
using type = int;
template <typename T>
struct pp
{
    bool operator()(const T lhs, const T rhs) const
    {
        return lhs < rhs;
    }
};

int main()
{
    // Insertion Tests
    {
        /*
        Sorted insertion test
        */
        int T(Tmin);
        while (T < Tmax)
        {
            vector<type> v(T);
            iota(begin(v), end(v), 0);
            B_Plus_tree<type, d, pp<type>> b(begin(v), end(v));
            if (!is_sorted(begin(b), end(b), pp<double>()))
            {
                cout << "Test(Sort) :" << T << " Failed\n";
                cout << "E:";
                sort(begin(v), end(v), pp<type>());
                for (auto i : v)
                    cout << i << "\t";
                cout << "\nB:";
                for (auto i : b)
                    cout << i << "\t";
                cout << "\n";
                return 1;
            }
            ++T;
        }
        cout << "Test(Sort) Passed\n";
    }
    {
        /*
            Reverse Sorted insertion test
        */
        int T(Tmin);
        while (T < Tmax)
        {
            vector<type> v(T);
            iota(begin(v), end(v), 0);
            B_Plus_tree<type, d, pp<type>> b(rbegin(v), rend(v));
            if (!is_sorted(begin(b), end(b), pp<type>()))
            {
                cout << "Test(Rev) :" << T << " Failed\n";
                cout << "E:";
                reverse(begin(v), end(v));
                for (auto i : v)
                    cout << i << "\t";
                cout << "\nB:";
                for (auto i : b)
                    cout << i << "\t";
                cout << "\n";
                return 1;
            }
            ++T;
        }
        cout << "Test(Rev) Passed\n";
    }
    {
        /*
            Test for random numbers
        */
        set<type, pp<type>> s;
        B_Plus_tree<type, d, pp<type>> b;
        long i(1);
        default_random_engine generator(chrono::system_clock::now().time_since_epoch().count());
        uniform_int_distribution<type> distribution(-N, N);
        int T(Tmin);
        while (T < Tmax)
        {
            vector<type> v;
            for (int j = 0; j < T; ++j)
                v.push_back(distribution(generator));
            for (auto i : v)
            {
                b.insert(i);
                s.insert(i);
            }
            if (!is_sorted(begin(b), end(b), pp<type>()))
            {
                vector<type> t1(begin(s), end(s));
                vector<type> t2(begin(b), end(b));
                sort(begin(t1), end(t1));
                sort(begin(t2), end(t2));
                if (t1 != t2)
                {
                    cout << "Test(Rand) :" << i << "/" << trials << ",N:" << T << " Failed\n";
                    cout << "I:";
                    for (auto i : v)
                        cout << i << "\t";
                    cout << "\n";
                    cout << "E:";
                    sort(begin(v), end(v), pp<int>());
                    for (auto i : v)
                        cout << i << "\t";
                    cout << "\nS:";
                    for (auto i : s)
                        cout << i << "\t";
                    cout << "\nB:";
                    for (auto i : b)
                        cout << i << "\t";
                    cout << "\n";
                    return 1;
                }
            }
            i = (i + 1) % trials;
            if (!(i % trials))
            {
                cout << "Test(Rand): " << T << " Passed\n";
                ++T;
            }
            v.clear();
            b.clear();
            s.clear();
        }
    }
    return 0;
}