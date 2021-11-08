# BPlus-Tree

## Implementing generic B+ Tree Container

Team Members: 

1. Dhruva Kashyap  - PES1201801457 - Section F
2. Sumukh Aithal K - PES1201801461 - Section H
3. Thrivikraman V  - PES1201801469 - Section D

## Abstract:
We implement the B+ tree data structure. We will be supporting member functions like insert, delete, clear. 
A few STL algorithms like find will also be made member functions to make it more efficient. 
The B+ tree data structure will support predicate as a type to support ordering as per client's needs. 
A constant bidirectional iterator will be supported which will do inorder traversal of the tree. 
All Generic algorithms like find and copy which work with constant bidirectional iterator has been supported. 
Iterator traits class has been provided for this type.

## Code Structure
```
include/
    bplus.h - Main file consisting of the definition of the B-Plus-Tree and the functions defined.
    concepts.h - using concepts to make sure that the degree of the tree is valid.
    tests.h  - Testing file
```
```
src/
    client.cpp - Main file
    tests.cpp  - test client
```
## To run the code
```
make
./a.out
```
This runs the src/client.cpp file.

## Functions

1. Class is made canonical.
2. insert\
    a. insert using initializer list\
    b. insert using a pair of iterators\
    c. insert a key - returns an iterator to the inserted element\
3. delete_key\
    a. delete using initializer list\
    b. delete using a pair of iterators\
    c. delete a key - returns an iterator to the next element if any\
    d. delete using iterator - delete the element pointed by the iterator
4. find\
    returns an iterator to the element if found else it returns one past the end.
5. clear\
    Deletes all the elements in the tree.
6. empty\
    Returns true if the tree is empty.
7. size\
    Returns the number of the elements in the tree.
8. operator== and operator!=\
    Checks if the elements of the two trees are same or not.

## Supported Iterators

1. Constant Bidirectional Iterator with begin() and end() and the supported traits.
2. Constant Bidirectional Reverse Iterator with rbegin() and rend() and the supported traits.

The type which corresponds to the the key of the B plus tree should support:
1. operator< by default or the client should provide a comparator.
2. operator<< if print_tree is to be used. 
3. Copy constructor to pass to insert\
Supports all STL algorithms that operate on constant bidirectional iterators


## References

https://en.wikipedia.org/wiki/B%2B_tree \
https://www.cs.usfca.edu/~galles/visualization/BPlusTree.html
