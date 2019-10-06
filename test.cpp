#include "balanced_reduction.h"
#include "eop-code/intrinsics.h"
#include "eop-code/type_functions.h"
#include "eop-code/drivers.h"
#include "eop-code/print.h"
#include "eop-code/assertions.h"
#include "eop-code/tests.h"

template <typename L>
    requires(List(L))
void algorithm_sort()
{
    typedef ValueType(L) Z;
    typedef IteratorType(L) I;
    typedef DistanceType(I) N;
    const int n = 500;
    array<Z> a(n, n, Z(0));
    typedef IteratorType(array<Z>) Ia;

    Ia f = begin(a);
    iota(n, f);
    L la(a);

    reverse(la);
    sort_iterative(la, less<Z>());
    Assert(size(la) == n);
    Assert(equal_iota(begin(la), end(la)));
}

void algorithm_reverse()
{
    typedef int T;
    const int k = 50;
    array_k<k, T> ca;
    typedef ptrdiff_t N;
    slist<T> l;
    extend_sequence_n(l, k, T(-1));
    iota(k, begin(l));
    reverse_n_adaptive_iterative(begin(l), k, begin(ca), half_nonnegative(k));
    equal_iota_reverse(begin(l), end(l));
}

void algorithm_partition()
{
    const int k = 20;
    array_k<k, int> b;
    const int n = 113;
    array_k<n, int> a;
    iota(n, begin(a));
    /* print_range(begin(a), end(a)); print_eol(); */
    pair<int*, int*> x = partition_stable_n_adaptive_iterative(
        begin(a), size(a), begin(b), size(b), even_int);
    /* print_range(begin(a), end(a)); print_eol(); */
    Assert(x.m1 == end(a));
    Assert(partitioned(begin(a), end(a), even<int>));
    Assert(partitioned_at_point(begin(a), x.m0, end(a), even<int>));
}

int main()
{
    algorithm_sort< slist<int> >();
    algorithm_sort< list<int> >();
    algorithm_reverse();
    algorithm_partition();
}
