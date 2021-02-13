# Balanced Reduction

I solve three exercises from [Elements of
Programming](https://www.elementsofprogramming.com). Each of the three
algorithms use the idea of balanced reduction to implement iterative versions
of recursive algorithms. A similar technique is used in the Linux kernel for
[`list_sort`](https://github.com/torvalds/linux/blob/master/lib/list_sort.c).

I have written an article on Balanced Reduction which can be found
[here](https://rksouthee.github.io/2019/07/26/balanced-reduction.html)

## Exercises

### Sort List

The first exercise was to sort a linked list.

**Exercise 11.11** Implement an iterative version of `sort_linked_nonempty_n`
from Chapter 8, using `reduce_balanced`.

### Reverse Forward Iterators

There is a recursive algorithm to reverse a range of forward iterators with complexity `O(n * lg(n))`.

**Exercise 11.12** Implement an iterative version of `reverse_n_adaptive` from
Chapter 10, using `reduce_balanced`.

The `adaptive` refers to the fact that this algorithm can use some extra
provided to it, which in the case of the recursive version, there are
potentially fewer recursive calls.

### Stable Partition

**Exercise 11.13** Use `reduce_balanced` to implement an iterative and
memory-adaptive version of `partition_stable_n`.

## Building

To compile you'll need the code from the book which can found at [EoP](http://elementsofprogramming.com/).
