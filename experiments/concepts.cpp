#include <iostream>
#include <concepts>

template <int N>
struct X
{
    const static bool value = N > 1;
};

template <int N>
concept PLUS = X<N>::value;

template <int N>
requires PLUS<N>
struct Y
{
    int z = N;
};

using namespace std;
int main()
{
    Y<0> z;
}
