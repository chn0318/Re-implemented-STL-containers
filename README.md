# Re-implemented-C-STL-containers

### vector

Re-implement a vector class with the following interfaces:

* constructor, destructor
* overload assignment operator
* visit by index
* `front()`, `back()`, `empty()`, `size()`, `clear()`
* `insert`, `erase`
* `push_back()`, `pop_back()`
* iterator and related operations

### deque

Re-implement a deque class, and the interface is basically consistent with vector.

**Notice**: Deque is required to support the insertion and deletion of **any location** in a time not exceeding **O (sqrt (n))**, but the complexity of random access is required to be reduced to **O (sqrt (n))**

### map

Re-implement a map class with the following interfaces:

* constructor, destructor
* overload assignment operator
* visit by index
* `empty()`, `size()`, `clear()`
* `insert`, `erase`
* `find`
* iterator and related operations

**Notice:** The map is implemented using a **red-black tree**, which has the property of being a balanced tree. This allows the map to maintain efficient performance, even when it contains a large number of elements.
