//
// Created by 龙方淞 on 2017/12/20.
//

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
}
#endif //STL_FROM_SCRATCH_MODIFYING_SEQUENCE_H
