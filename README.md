CPPItertools
============

range-based for loop add-ons inspired by the Python builtins and itertools
library.  Like itertools and the Python3 builtins, this library uses lazy
evaluation wherever possible.

*Note*: Everthing is inside the `iter` namespace.

#### Requirements
This library is **header-only** and relies only on the C++ standard
library. The only exception is `zip_longest` which uses `boost::optional`.
`#include <cppitertools/itertools.hpp>` will include all of the provided
tools except for `zip_longest` which must be included separately.  You may
also include individual pieces with the relevant header
(`#include <cppitertools/enumerate.hpp>` for example).

#### Table of Contents
[range](#range)<br />
[enumerate](#enumerate)<br />
[zip](#zip)<br />
[zip_longest](#zip)<br />
[imap](#imap)<br />
[filter](#filter)<br />
[filterfalse](#filterfalse)<br />
[unique_everseen](#unique_everseen)<br />
[unique_justseen](#unique_justseen)<br />
[takewhile](#takewhile)<br />
[dropwhile](#dropwhile)<br />
[cycle](#cycle)<br />
[repeat](#repeat)<br />
[count](#count)<br />
[groupby](#groupby)<br />
[accumulate](#accumulate)<br />
[compress](#compress)<br />
[sorted](#sorted)<br />
[chain](#chain)<br />
[chain.from\_iterable](#chainfrom_iterable)<br />
[reversed](#reversed)<br />
[slice](#slice)<br />
[sliding_window](#sliding_window)<br />
[grouper](#grouper)<br />

##### Combinatoric fuctions
[product](#product)<br />
[combinations](#combinations)<br />
[combinations_with_replacement](#combinations_with_replacement)<br />
[permutations](#permutations)<br />
[powerset](#powerset)<br />

range
-----

Uses an underlying iterator to acheive the same effect of the python range
function.  `range` can be used in three different ways:

Only the stopping point is provided.  Prints `0 1 2 3 4 5 6 7 8 9`
```c++
for (auto i : range(10)) {
    cout << i << '\n';
}
```

The start and stop are both provided.  Prints `10 11 12 13 14`
```c++
for (auto i : range(10, 15)) {
    cout << i << '\n';
}
```

The start, stop, and step are all provided.  Prints `20 22 24 26 28`
```c++
for (auto i : range(20, 30, 2)) {
    cout << i << '\n';
}
```

Negative values are allowed as well.  Prints `2 1 0 -1 -2`
```c++
for (auto i : range(2, -3, -1)) {
    cout << i << '\n';
}
```

In addition to normal integer range operations, doubles and
other numeric types are supported through the template

Prints: `5.0 5.5 6.0` ... `9.5`
```c++
for(auto i : range(5.0, 10.0, 0.5)) {
    cout << i << '\n';
}
```

enumerate
---------

Can be used with any class with an iterator.  Continually "yields" containers
similar to pairs.  They are basic structs with a .index and a .element.  Usage
appears as:

```c++
vector<int> vec{2, 4, 6, 8};
for (auto&& e : enumerate(vec)) {
    cout << e.index
         << ": "
         << e.element
         << '\n';
}
```

filter
------
Called as `filter(predicate, iterable)`.  The predicate can be any callable.
`filter` will only yield values that are true under the predicate.

Prints values greater than 4:  `5 6 7 8`
```c++
vector<int> vec{1, 5, 4, 0, 6, 7, 3, 0, 2, 8, 3, 2, 1};
for (auto&& i : filter([] (int i) { return i > 4; }, vec)) {
    cout << i <<'\n';
}

```

If no predicate is passed, the elements themselves are tested for truth

Prints only non-zero values.
```c++
for(auto&& i : filter(vec)) {
    cout << i << '\n';
}
```

filterfalse
-----------
Similar to filter, but only prints values that are false under the predicate.

Prints values not greater than 4: `1 4 3 2 3 2 1 `
```c++
vector<int> vec{1, 5, 4, 0, 6, 7, 3, 0, 2, 8, 3, 2, 1};
for (auto&& i : filterfalse([] (int i) { return i > 4; }, vec)) {
    cout << i <<'\n';
}

```

If no predicate is passed, the elements themselves are tested for truth.

Prints only zero values.
```c++
for(auto&& i : filterfalse(vec)) {
    cout << i << '\n';
}

```
unique_everseen
---------------
This is a filter adaptor that only generates values that have never been seen
before. For this to work your object must be specialized for `std::hash`.

Prints `1 2 3 4 5 6 7 8 9`
```c++
vector<int> v {1,2,3,4,3,2,1,5,6,7,7,8,9,8,9,6};
for (auto&& i : unique_everseen(v)) {
    cout << i << ' ';
}
```

unique_justseen
--------------
Another filter adaptor that only omits consecutive duplicates.

Prints `1 2 3 4 3 2 1`
Example Usage:
```c++
vector<int> v {1,1,1,2,2,3,3,3,4,3,2,1,1,1};
for (auto&& i : unique_justseen(v)) {
    cout << i << ' ';
}
```

takewhile
---------
Yields elements from an iterable until the first element that is false under
the predicate is encountered.

Prints `1 2 3 4`.  (5 is false under the predicate)
```c++
vector<int> ivec{1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1};
for (auto&& i : takewhile([] (int i) {return i < 5;}, ivec)) {
    cout << i << '\n';
}
```

dropwhile
---------
Yields all elements after and including the first element that is true under
the predicate.

Prints `5 6 7 1 2`
```c++
vector<int> ivec{1, 2, 3, 4, 5, 6, 7, 1, 2};
for (auto&& i : dropwhile([] (int i) {return i < 5;}, ivec)) {
    cout << i << '\n';
}
```

cycle
-----

Repeatedly produces all values of an iterable.  The loop will be infinite, so a
`break` or other control flow structure is necessary to exit.

Prints `1 2 3` repeatedly until `some_condition` is true
```c++
vector<int> vec{1, 2, 3};
for (auto&& i : cycle(vec)) {
    cout << i << '\n';
    if (some_condition) {
        break;
    }
}
```

repeat
------
Repeatedly produces a single argument forever, or a given number of times.
`repeat` will bind a reference when passed an lvalue and move when given
an rvalue.  It will then yield a reference to the same item until completion.

The below prints `1` five times.
```c++
for (auto&& e : repeat(1, 5)) {
    cout << e << '\n';
}
```

The below prints `2` forever
```c++
for (auto&& e : repeat(2)) {
    cout << e << '\n';
}
```

count
-----
Effectively a `range` without a stopping point.<br />
`count()` with no arguments will start counting from 0 with a positive
step of 1.<br />
`count(i)` will start counting from `i` with a positive step of 1.<br />
`count(i, st)` will start counting from `i` with a step of `st`.

*Technical limitations*: Unlike Python which can use its long integer
types when needed, count() will eventually exceed the
maximum possible value for its type (or minimum with a negative step).
When using a signed type it is up to the API user to ensure this does
not happen.  If the limit is exceeded for signed types, the result is
undefined (as per the C++ standard).

The below will print `0 1 2` ... etc
```c++
for (auto&& i : count()) {
    cout << i << '\n';
}
```

groupby
-------
Separate an iterable into groups sharing a common key.  The following example
creates a new group whenever a string of a different length is encountered.
```c++
vector<string> vec = {
    "hi", "ab", "ho",
    "abc", "def",
    "abcde", "efghi"
};

for (auto&& gb : groupby(vec, [] (const string &s) {return s.length(); })) {
    cout << "key: " << gb.first << '\n';
    cout << "content: ";
    for (auto&& s : gb.second) {
        cout << s << "  ";
    }
    cout << '\n';
}
```
*Note*: Just like Python's `itertools.groupby`, this doesn't do any sorting.
It just iterates through, making a new group each time there is a key change.
Thus, if the group is unsorted, the same key may appear multiple times.

accumulate
-------
Differs from `std::accumulate` (which in my humble opinion should be named
`std::reduce` or `std::foldl`).  It is similar to a functional reduce where one
can see all of the intermediate results.  By default, it keeps a running sum.
Prints: `1 3 6 10 15`
```c++
for (auto&& i : accumulate(range(1, 6))) {
    cout << i << '\n';
}
```
A second, optional argument may provide an alternative binary function
to compute results.  The following example multiplies the numbers, rather
than adding them.
Prints: `1 2 6 24 120`

```c++
for (auto&& i : accumulate(range(1, 6), std::multiplies<int>{})) {
    cout << i << '\n';
}
```

Note: The intermediate result type must support default construction
and assignment.

zip
---
Takes an arbitrary number of ranges of different types and efficiently iterates
over them in parallel (so an iterator to each container is incremented
simultaneously).  When you dereference an iterator to "zipped" range you get a
tuple of the elements the iterators were holding. 

Example usage:
```c++
array<int,4> i{{1,2,3,4}};
vector<float> f{1.2,1.4,12.3,4.5,9.9};
vector<string> s{"i","like","apples","alot","dude"};
array<double,5> d{{1.2,1.2,1.2,1.2,1.2}};

for (auto&& e : zip(i,f,s,d)) {
    cout << std::get<0>(e) << ' '
         << std::get<1>(e) << ' '
         << std::get<2>(e) << ' '
         << std::get<3>(e) << '\n';
    std::get<1>(e)=2.2f; // modifies the underlying 'f' array
}
```

zip_longest
-----------
Terminates on the longest sequence instead of the shortest.
Repeatedly yields a tuple of `boost::optional<T>`s where `T` is the type
yielded by the sequences' respective iterators.  Because of its boost
dependency, `zip_longest` is not in `itertools.hpp` and must be included
separately.
The following loop prints either "Just <item>" or "Nothing" for each
element in each tuple yielded.

```c++
vector<int> v1 = {0, 1, 2, 3};
vector<int> v2 = {10, 11};
for (auto&& t : zip_longest(v1, v2)) {
    cout << '{';
    if (std::get<0>(t)) {
        cout << "Just " << *std::get<0>(t);
    } else {
        cout << "Nothing";
    }
    cout << ", ";
    if (std::get<1>(t)) {
        cout << "Just " << *std::get<1>(t);
    } else {
        cout << "Nothing";
    }
    cout << "}\n";
}
```

The output is:
```
{Just 0, Just 10}
{Just 1, Just 11}
{Just 2, Nothing}
{Just 3, Nothing}
```

imap
----

Takes a function and one or more iterables.  The number of iterables must
match the number of arguments to the function.  Applies the function to
each element (or elements) in the iterable(s).  Terminates on the shortest
sequence.

Prints the squares of the numbers in vec: `1 4 9 16 25`
```c++
vector<int> vec{1, 2, 3, 4, 5};
for (auto&& i : imap([] (int x) {return x * x;}, vec)) {
    cout << i << '\n';
}
```

With more than one sequence, the below adds corresponding elements from
each vector together, printing `11 23 35 47 59 71`
```c++
vector<int> vec1{1, 3, 5, 7, 9, 11};
vector<int> vec2{10, 20, 30, 40, 50, 60};
for (auto&& i : imap([] (int x, int y) { return x + y; }, vec1, vec2)) {
    cout << i << '\n';
}
```

*Note*: The name `imap` is chosen to prevent confusion/collision with
`std::map`, and because it is more related to `itertools.imap` than
the python builtin `map`.


compress
--------

Yields only the values corresponding to true in the selectors iterable.
Terminates on the shortest sequence.

Prints `2 6`
```c++
vector<int> ivec{1, 2, 3, 4, 5, 6};
vector<bool> bvec{false, true, false, false, false, true};
for (auto&& i : compress(ivec, bvec) {
    cout << i << '\n';
}
```

sorted
------
Allows iteration over a sequence in sorted order. `sorted` does
**not** produce a new sequence, copy elements, or modify the original
sequence.  It only provides a way to iterate over existing elements.
`sorted` also takes an optional second
[comparator](http://en.cppreference.com/w/cpp/concept/Compare)
argument.  If not provided, defaults to `std::less`. <br />
Iterables passed to sorted are required to have an iterator with
an `operator*() const` member.

The below outputs `0 1 2 3 4`.

```c++
unordered_set<int> nums{4, 0, 2, 1, 3};
for (auto&& i : sorted(nums)) {
    cout << i << '\n';
}
```

chain
-----

This can chain any set of ranges together as long as their iterators
dereference to the same type.

```c++
vector<int> empty{};
vector<int> vec1{1,2,3,4,5,6};
array<int,4> arr1{{7,8,9,10}};

for (auto&& i : chain(empty,vec1,arr1)) {
    cout << i << '\n';
}
```

chain.from_iterable
-------------------

Similar to chain, but rather than taking a variadic number of iterables,
it takes an iterable of iterables and chains the contained iterables together.
A simple example is shown below using a vector of vectors to represent
a 2d ragged array, and prints it in row-major order.
```c++
vector<vector<int>> matrix = {
    {1, 2, 3},
    {4, 5},
    {6, 8, 9, 10, 11, 12}
};

for (auto&& i : chain.from_iterable(matrix)) {
    cout << i << '\n';
}
```

reversed
-------

Iterates over elements of a sequence in reverse order.

```c++
for (auto&& i : reversed(a)) {
    cout << i << '\n';
}
```

slice
-----

Returns selected elements from a range, parameters are start, stop and step.
the range returned is [start,stop) where you only take every step element

This outputs `0 3 6 9 12`
```c++
vector<int> a{0,1,2,3,4,5,6,7,8,9,10,11,12,13};
for (auto&& i : slice(a,0,15,3)) {
    cout << i << '\n';
}
```

sliding_window
-------------
Takes a section from a range and increments the whole section.

Example:
`[1, 2, 3, 4, 5, 6, 7, 8, 9]`

take a section of size 4, output is:
```
1 2 3 4
2 3 4 5
3 4 5 6
4 5 6 7
5 6 7 8
6 7 8 9
```

Example Usage:
```c++
vector<int> v = {1,2,3,4,5,6,7,8,9};
for (auto&& sec : sliding_window(v,4)) {
    for (auto&& i : sec) {
        cout << i << ' ';
        i.get() = 90;
    }
    cout << '\n';
}
```
grouper
------

grouper is very similar to sliding window, except instead of the
section sliding by only 1 it goes the length of the full section.

Example usage:
```c++
vector<int> v {1,2,3,4,5,6,7,8,9};
for (auto&& sec : grouper(v,4))
//each section will have 4 elements
//except the last one may be cut short
{
    for (auto&& i : sec) {
        cout << i << " ";
        i.get() *= 2;
    }
    cout << '\n';
}
```

product
------

Generates the cartesian project of the given ranges put together

Example usage:
```c++
vector<int> v1{1,2,3};
vector<int> v2{7,8};
vector<string> v3{"the","cat"};
vector<string> v4{"hi","what","up","dude"};
for (auto&& t : product(v1,v2,v3,v4)) {
    cout << std::get<0>(t) << ", "
        << std::get<1>(t) << ", "
        << std::get<2>(t) << ", "
        << std::get<3>(t) << '\n';
}
```

combinations
-----------

Generates n length unique sequences of the input range.

Example usage:
```c++
vector<int> v = {1,2,3,4,5};
for (auto&& i : combinations(v,3)) {
    for (auto&& j : i ) cout << j << " ";
    cout << '\n';
}
```

combinations_with_replacement
-----------------------------
Like combinations, but with replacement of each element.  The
below is printed by the loop that follows:
```
{A, A}
{A, B}
{A, C}
{B, B}
{B, C}
{C, C}
```
```c++
for (auto&& v : combinations_with_replacement(s, 2)) {
    cout << '{' << v[0] << ", " << v[1] << "}\n";
}
```


permutations
-----------

Generates all the permutations of a range using `std::next_permutation`.  The
iterators of the sequence passed must have an `operator*() const`

Example usage:
```c++
vector<int> v = {1,2,3,4,5};
for (auto&& vec : permutations(v)) {
    for (auto&& i : vec) {
        cout << i << ' ';
    }
    cout << '\n';
}
```

powerset
-------

Generates every possible subset of a set, runs in O(2^n).

Example usage:
```c++
vector<int> vec {1,2,3,4,5,6,7,8,9};
for (auto&& v : powerset(vec)) {
    for (auto&& i : v) {
        cout << i << " ";
    }
    cout << '\n';
}
```
