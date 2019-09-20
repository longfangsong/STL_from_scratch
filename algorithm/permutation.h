#ifndef STL_FROM_SCRATCH_PERMUTATION_H
#define STL_FROM_SCRATCH_PERMUTATION_H

#include "algorithm.h"

namespace Readable {
    template<typename BidirectionIterator>
    bool next_permutation(BidirectionIterator first, BidirectionIterator last) {
        if (first == last)
            return false;
        BidirectionIterator cursor = last;
        --cursor;
        if (first == cursor) return false;

        BidirectionIterator before_cursor = next(cursor, -1);
        // find last pair which (*before_cursor < *cursor)
        while (*before_cursor >= *cursor && before_cursor != first) {
            --cursor;
            --before_cursor;
        }
        if (*before_cursor < *cursor) {
            BidirectionIterator last_larger_than_before_cursor = next(last, -1);
            // find last element > *before_cursor
            while (*last_larger_than_before_cursor < *before_cursor)
                --last_larger_than_before_cursor;
            iter_swap(before_cursor, last_larger_than_before_cursor);
            reverse(cursor, last);
            return true;
        }
        if (before_cursor == first) {
            Readable::reverse(first, last);
            return false;
        }
    }

    template<typename BidirectionIterator>
    bool prev_permutation(BidirectionIterator first, BidirectionIterator last) {
        if (first == last)
            return false;
        BidirectionIterator cursor = last;
        --cursor;
        if (first == cursor) return false;

        BidirectionIterator before_cursor = next(cursor, -1);
        // find last pair which (*before_cursor < *cursor)
        while (*before_cursor < *cursor && before_cursor != first) {
            --cursor;
            --before_cursor;
        }
        if (*before_cursor >= *cursor) {
            BidirectionIterator last_larger_than_before_cursor = next(last, -1);
            // find last element > *before_cursor
            while (*last_larger_than_before_cursor > *before_cursor)
                --last_larger_than_before_cursor;
            iter_swap(before_cursor, last_larger_than_before_cursor);
            reverse(cursor, last);
            return true;
        }
        if (before_cursor == first) {
            Readable::reverse(first, last);
            return false;
        }
    }
}
#endif //STL_FROM_SCRATCH_PERMUTATION_H
