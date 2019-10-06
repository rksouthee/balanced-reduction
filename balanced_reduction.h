#ifndef BALANCED_REDUCTION_H
#define BALANCED_REDUCTION_H
/*
 * Page 202
 *
 * Implement an iterative version of sort_linked_nonempty_n from Chapter 8,
 * using reduce_balanced.
 *
 * Implement an iterative version of reverse_n_adaptive from Chapter 10, using
 * reduce_balanced.
 *
 * Use reduce_balanced to implement an iterative and memory-adaptive version of
 * partition_stable_n.
 */

#include "eop-code/eop.h"

template <typename I, typename S, typename R>
struct merge_linked_ranges
{
    S set_link;
    R r;

    merge_linked_ranges(const S& set_link, const R& r) :
        set_link(set_link), r(r) { }

    pair<I, I> operator()(const pair<I, I>& x, const pair<I, I>& y) const
    {
        return merge_linked_nonempty(x.m0, x.m1, y.m0, y.m1, r, set_link);
    }
};

template <typename I, typename S, typename R>
struct input_type<merge_linked_ranges<I, S, R>, 0>
{
    typedef pair<I, I> type;
};

template <typename I, typename S>
struct sort_trivial
{
    I f;
    I l;
    S set_link;

    sort_trivial(const I& f, const I& l, const S& set_link) :
        f(f), l(l), set_link(set_link) { }

    pair<I, I> operator()(DistanceType(I))
    {
        I t = successor(f);
        set_link(f, l);
        pair<I, I> p(f, l);
        f = t;
        return p;
    }
};

template<typename I, typename S, typename R>
    requires(Readable(I) &&
        ForwardLinker(S) && I == IteratorType(S) &&
        Relation(R) && ValueType(I) == Domain(R))
pair<I, I> sort_linked_n_iterative(I f, DistanceType(I) n, R r, S set_link)
{
    // Precondition: $\property{counted\_range}(f, n) \wedge
    //                n > 0 \wedge \func{weak\_ordering}(r)$
    typedef DistanceType(I) N;
    I l = f + n;
    return reduce_balanced(
        N(0), n,
        merge_linked_ranges<I, S, R>(set_link, r),
        sort_trivial<I, S>(f, l, set_link),
        pair<I, I>(l, l));
}

template <typename T, typename R>
    requires(Regular(T) && Relation(R) && Domain(R) == T)
void sort_iterative(list<T>& x, R r)
{
    // Precondition: $\func{weak\_ordering}(r)$
    typedef IteratorType(list<T>) I;
    pair<I, I> p = sort_linked_n_iterative(begin(x), size(x), r, forward_linker<I>());

    bidirectional_linker<I>()(x.dummy, p.m0);
    I f = p.m0;
    while (f != p.m1) {
        backward_linker<I>()(f, successor(f));
        f = successor(f);
    }
}

template <typename T, typename R>
    requires(Regular(T) && Relation(R) && Domain(R) == T)
void sort_iterative(slist<T>& x, R r)
{
    // Precondition: $\func{weak\_ordering}(r)$
    typedef IteratorType(slist<T>) I;
    pair<I, I> p = sort_linked_n_iterative(begin(x), size(x), r, forward_linker<I>());
    x.first = p.m0;
}

template <typename I, typename B>
struct reverse_trivial_with_buffer
{
    typedef DistanceType(I) N;
    I f_i;
    B f_b;
    N n_b;

    reverse_trivial_with_buffer(const I& f_i, const B& f_b, const N& n_b) :
        f_i(f_i), f_b(f_b), n_b(n_b) { }

    pair<I, I> operator()(const N&)
    {
        I l_i = reverse_n_with_buffer(f_i, n_b, f_b);
        pair<I, I> p(f_i, l_i);
        f_i = l_i;
        return p;
    }
};

template<typename I, typename B>
    requires(Mutable(I) && ForwardIterator(I) &&
        Mutable(B) && BidirectionalIterator(B) &&
        ValueType(I) == ValueType(B))
I reverse_n_adaptive_iterative(I f_i, DistanceType(I) n_i,
                               B f_b, DistanceType(I) n_b)
{
    // Precondition: $\property{mutable\_counted\_range}(f_i, n_i)$
    // Precondition: $\property{mutable\_counted\_range}(f_b, n_b)$
    typedef DistanceType(I) N;
    N m = n_i / n_b;
    pair<I, I> p = reduce_balanced(
        N(0), m,
        combine_ranges<I>,
        reverse_trivial_with_buffer<I, B>(f_i, f_b, n_b),
        pair<I, I>(f_i, f_i));
    n_i = n_i % n_b;
    I l_i = reverse_n_with_buffer(p.m1, n_i, f_b);
    rotate(p.m0, p.m1, l_i);
    return l_i;
}

template <typename I, typename B, typename P>
struct partition_stable_trivial_with_buffer
{
    typedef DistanceType(I) N;
    I f_i;
    B f_b;
    N n_b;
    P p;

    partition_stable_trivial_with_buffer(const I& f_i, const B& f_b,
                                         const N& n_b, const P& p) :
        f_i(f_i), f_b(f_b), n_b(n_b), p(p) { }

    pair<I, I> operator()(const N&)
    {
        I l_i = f_i + n_b;
        I m_i = partition_stable_with_buffer(f_i, l_i, f_b, p);
        pair<I, I> x(m_i, l_i);
        f_i = l_i;
        return x;
    }
};

template<typename I, typename B, typename P>
    requires(Mutable(I) && ForwardIterator(I) &&
        UnaryPredicate(P) && ValueType(I) == Domain(P))
pair<I, I> partition_stable_n_adaptive_iterative(I f_i, DistanceType(I) n_i,
                                                 B f_b, DistanceType(I) n_b,
                                                 P p)
{
    // Precondition: $\property{mutable\_counted\_range}(f_i, n_i)$
    // Precondition: $\property{mutable\_counted\_range}(f_b, n_b)$
    typedef DistanceType(I) N;
    pair<I, I> x = reduce_balanced(
        N(0), n_i / n_b,
        combine_ranges<I>,
        partition_stable_trivial_with_buffer<I, B, P>(f_i, f_b, n_b, p),
        pair<I, I>(f_i, f_i));
    I l_i = x.m1 + n_i % n_b;
    I m_i = partition_stable_with_buffer(x.m1, l_i, f_b, p);
    return pair<I, I>(rotate(x.m0, x.m1, m_i), l_i);
}

#endif
