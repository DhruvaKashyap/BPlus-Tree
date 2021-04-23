#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;
int main()
{
    vector<int> v;
    v.reserve(20);
    v.push_back(1);
    v.resize(2);
    v[1] = 3;
    // v.insert(begin(v)+1,3);
    cout << v.capacity() << "\n";
    copy(begin(v), end(v), ostream_iterator<int>(cout, "\t"));
    cout << "\n";
}