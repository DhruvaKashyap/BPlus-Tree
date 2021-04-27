#include "bplus.h"
#include <vector>
#include <deque>
#include <set>
#include <random>
#include <algorithm>
#include <chrono>
using namespace std;
using namespace std::chrono;
template<class T> struct TypeIsInt
{
    static const bool value = false;
};

template <>
struct TypeIsInt<int>
{
    static const bool value = true;
};

template <typename Type, typename pp, int d = Y<Type>::value>
class test
{
    const int N = 100000;
    const int trials = 1000;
    const int Tmin = 1;
    const int Tmax = 1000;

public:
    test()
    {
    }
    test(int range, int trial, int min, int max) : N(range), trials(trial), Tmin(min), Tmax(max)
    {
    }
    bool sortedInsertionTest()
    {
        int T(Tmin);
        auto start = high_resolution_clock::now();
        while (T < Tmax)
        {
            vector<Type> v(T);
            iota(begin(v), end(v), 0);
            B_Plus_tree<Type, d, pp> b(begin(v), end(v));
            if (!is_sorted(begin(b), end(b), pp()))
            {
                cout << "Test(Sort) :" << T << " Failed\n";
                cout << "E:";
                sort(begin(v), end(v), pp());
                for (auto i : v)
                    cout << i << "\t";
                cout << "\nB:";
                for (auto i : b)
                    cout << i << "\t";
                cout << "\n";
                return 0;
            }
            ++T;
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Test(Sort) Passed " << duration.count() << "ms\n";
        return 0;
    }
    bool reverseSortedTest()
    {
        int T(Tmin);
        auto start = high_resolution_clock::now();
        while (T < Tmax)
        {
            vector<Type> v(T);
            iota(begin(v), end(v), 0);
            B_Plus_tree<Type, d, pp> b(rbegin(v), rend(v));
            if (!is_sorted(begin(b), end(b), pp()))
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
                return 0;
            }
            ++T;
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Test(Rev) Passed " << duration.count() << "ms\n";
        return 1;
    }
    bool randomInsertionTest()
    {
        set<Type, pp> s;
        B_Plus_tree<Type, d, pp> b;
        long i(1);
        default_random_engine generator(chrono::system_clock::now().time_since_epoch().count());
        uniform_int_distribution<Type> distribution(-N, N);
        int T(Tmin);
        while (T < Tmax)
        {
            vector<Type> v;
            auto start = high_resolution_clock::now();
            for (int j = 0; j < T; ++j)
                v.push_back(distribution(generator));
            for (auto i : v)
            {
                b.insert(i);
                s.insert(i);
            }
            if (!is_sorted(begin(b), end(b), pp()))
            {
                vector<Type> t1(begin(s), end(s));
                vector<Type> t2(begin(b), end(b));
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
                    sort(begin(v), end(v), pp());
                    for (auto i : v)
                        cout << i << "\t";
                    cout << "\nS:";
                    for (auto i : s)
                        cout << i << "\t";
                    cout << "\nB:";
                    for (auto i : b)
                        cout << i << "\t";
                    cout << "\n";
                    return 0;
                }
            }
            i = (i + 1) % trials;
            if (!(i % trials))
            {
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(stop - start);
                cout << "Test(Rand): " << T << " Passed " << duration.count() << "ms\n";
                ++T;
            }
            v.clear();
            b.clear();
            s.clear();
        }
        return 1;
    }
};
