#include "bplus.h"
#include <vector>
#include <deque>
#include <set>
#include <random>
#include <algorithm>
#include <iostream>
#include <chrono>
using namespace std;
using namespace std::chrono;
template <class T>
struct TypeIsInt
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
                cout << "Insert(Sort) :" << T << " Failed\n";
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
        cout << "Insert(Sort) Passed " << duration.count() << "us\n";
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
                cout << "Insert(Rev) :" << T << " Failed\n";
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
        cout << "Insert(Rev) Passed " << duration.count() << "us\n";
        return 1;
    }
    bool randomInsertionTest()
    {
        set<Type, pp> s;
        B_Plus_tree<Type, d, pp> b;
        long i(1);
        mt19937 generator(chrono::system_clock::now().time_since_epoch().count());
        uniform_int_distribution<Type> distribution(-N, N);
        int T(Tmin);
        while (T < Tmax)
        {
            vector<Type> v;
            for (int j = 0; j < T; ++j)
                v.push_back(distribution(generator));
            auto start = high_resolution_clock::now();
            for (auto i : v)
            {
                b.insert(i);
                s.insert(i);
            }
            auto stop = high_resolution_clock::now();
            if (!is_sorted(begin(b), end(b), pp()))
            {
                vector<Type> t1(begin(s), end(s));
                vector<Type> t2(begin(b), end(b));
                sort(begin(t1), end(t1));
                sort(begin(t2), end(t2));
                if (t1 != t2)
                {
                    cout << "Insert(Rand) :" << i << "/" << trials << ",N:" << T << " Failed\n";
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
                auto duration = duration_cast<microseconds>(stop - start);
                cout << "Insert(Rand): " << T << " Passed " << duration.count() << "us\n";
                ++T;
            }
            v.clear();
            b.clear();
            s.clear();
        }
        return 1;
    }
    bool nonOccuringDelete()
    {
        mt19937 generator(chrono::system_clock::now().time_since_epoch().count());
        uniform_int_distribution<Type> distribution(-N, N);
        int T(Tmin);
        while (T < Tmax)
        {
            set<Type, pp> v;
            vector<Type> dd;
            for (int j = 0; j < T; ++j)
                v.insert(distribution(generator));
            for (int j = 0; j < 10 * T; ++j)
                dd.push_back(distribution(generator));
            int k(0);
            B_Plus_tree<Type, d, pp> b(begin(v), end(v));
            for (Type i : dd)
            {
                if (find(begin(v), end(v), i) == v.end())
                    b.delete_key_temp(i);
                else
                    ++k;
                if (!equal(begin(v), end(v), begin(b), end(b)))
                {
                    cout << "Non Occuring delete Failed " << T << " deleting " << i << "\n";
                    for (auto i : b)
                        cout << i << '\t';
                    cout << '\n';
                    for (auto i : v)
                        cout << i << '\t';
                    cout << '\n';
                    return 0;
                }
            }
            // cout << k << " misses " << T << "/" << 10 * T << '\n';
            v.clear();
            dd.clear();
            ++T;
        }
        cout << "Non Occuring Delete Passed\n";
        return 1;
    }
    bool RandomDelete()
    {
        mt19937 generator(chrono::system_clock::now().time_since_epoch().count());
        uniform_int_distribution<Type> distribution(-N, N);
        int T(Tmin);
        while (T < Tmax)
        {
            set<Type, pp> s;
            for (int j = 0; j < T; ++j)
                s.insert(distribution(generator));
            vector<Type> dd(begin(s), end(s));
            B_Plus_tree<Type, d, pp> b(begin(s), end(s));
            shuffle(begin(dd), end(dd), generator);
            vector<Type> temp(begin(s), end(s));
            auto start = high_resolution_clock::now();
            for (Type i : dd)
            {
                b.delete_key_temp(i);
                s.erase(i);
                if (!equal(begin(s), end(s), begin(b), end(b)))
                {
                    cout << "Random delete Failed " << T << " deleting " << i << "\n";
                    for (auto i : s)
                        cout << i << '\t';
                    cout << '\n';
                    for (auto i : temp)
                        cout << i << '\t';
                    cout << '\n';
                    for (auto i : dd)
                        cout << i << '\t';
                    cout << "\n\n\n";
                    for (auto i : b)
                        cout << i << '\t';
                    cout << '\n';
                    return 0;
                }
            }
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            cout << "Delete(Random) " << T << " Passed " << duration.count() << "us\n";
            s.clear();
            dd.clear();
            ++T;
        }
        return 1;
    }
};
