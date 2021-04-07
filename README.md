# Threaded BPlus-Tree

Implementing generic B+ Tree Container
Team Members: 
Dhruva Kashyap - PES1201801457 - Section F
Sumukh Aithal K - PES1201801461 - Section H
Thrivikraman V - PES1201801469 - Section D

Abstract: (5-10 lines)
We plan on implementing the B+ tree data structure. We will be supporting member functions like insert, delete, range, clear. A few STL algorithms like find and find_if will also be made member functions to make it more efficient. The B+ tree data structure will support predicate as a type to support ordering as per client's needs. A constant bidirectional iterator will be supported which will do inorder traversal of the tree. Iterators to support preorder and postorder traversal will also be provided. All Generic algorithms like find, copy, find_if which work with constant bidrectional iterator will be supported. Iterator traits class will be provided for this type.

Interface:
insert - inserts elements
delete - delete element
operator++ - pre and post
clear - remove all elements
find - implemented using binary search
Performing a range query with k elements
find_if
begin,end => in_begin,in_end
rbegin, rend 
post_begin,post_end
pre_begin,pre_begin

Iterator:
const bidirectional iterator -> inorder traversal
Future work: const bidirectional iterator -> preorder and postorder as well?

Supports the following STL functions:
1. find
2. copy(begin(a),++begin(a),begin(b)) -> a,b are b+ tree -> not supported
   copy(begin(a),++begin(a),begin(b)) -> a = b+ tree , b -> vector/similar -> should work
3. find_if


https://www.cs.helsinki.fi/u/mluukkai/tirak2010/B-tree.pdf
https://en.wikipedia.org/wiki/B%2B_tree
http://www.amittai.com/prose/bplustree.html
http://www.amittai.com/prose/bplustree_cpp.html
https://www.cs.usfca.edu/~galles/visualization/BPlusTree.html
