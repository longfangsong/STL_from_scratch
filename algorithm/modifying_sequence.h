#ifndef STL_FROM_SCRATCH_MODIFYING_SEQUENCE_H
#define STL_FROM_SCRATCH_MODIFYING_SEQUENCE_H


namespace Readable {
    template<typename BidirIt1, typename BidirIt2>
    BidirIt2 copy_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last) {
        while (first != last) {
            *(--d_last) = *(--last);
        }
        return d_last;
    }

    template<typename BidirIt1, typename BidirIt2>
    BidirIt2 move_backward(BidirIt1 first,
                           BidirIt1 last,
                           BidirIt2 d_last) {
        while (first != last) {
            *(--d_last) = std::move(*(--last));
        }
        return d_last;
    }

    template<typename InputIt, typename OutputIt>
    OutputIt move(InputIt first, InputIt last, OutputIt d_first) {
        while (first != last) {
            *d_first++ = std::move(*first++);
        }
        return d_first;
    }

    template<typename ForwardIt1, typename ForwardIt2>
    void iter_swap(ForwardIt1 a, ForwardIt2 b) {
        using std::swap;
        swap(*a, *b);
    }

    template<typename BidirIt>
    void reverse(BidirIt first, BidirIt last) {
        while ((first != last) && (first != --last)) {
            std::iter_swap(first++, last);
        }
    }
}


#endif //STL_FROM_SCRATCH_MODIFYING_SEQUENCE_H
