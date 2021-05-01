#ifndef CONCEPT_H
#define CONCEPT_H
template <int N>
struct BPLUSREQ
{
    constexpr static bool value = N > 2;
};

template <typename T>
struct BPLUSVAL
{
    constexpr static int value = 4;
};

template <>
struct BPLUSVAL<int>
{
    constexpr static int value = 16;
};

template <>
struct BPLUSVAL<double>
{
    constexpr static int value = 8;
};

template <int N>
concept BPLUSMIN = BPLUSREQ<N>::value;


#endif