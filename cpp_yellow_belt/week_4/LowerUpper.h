#pragma once

template <typename RandomIt>
RandomIt LowerBound(RandomIt range_begin, RandomIt range_end, char prefix) {
    if (range_begin == range_end) {
        return range_end;
    } 
    if (range_end - range_begin == 1) {
        if (prefix <= (*range_begin)[0]) {
            return range_begin;
        } else {
            return range_end;
        }
    }
    
    if (range_end - range_begin == 2) {
        // Check if we are beyond the first element OR equal to required
        if (prefix <= (*range_begin)[0]) {
            return range_begin;
        // Check if we are beyond the last element
        } else if (prefix > (*next(range_begin))[0]) {
            return range_end;
        } else {
        // If the first doesn't equal to required it is either in between OR the second one
        // i.e. choose Greater or Equal
            return next(range_begin);
        }
    }

    // Find the middle of the range
    auto middle = range_begin + (range_end - range_begin) / 2;
    // Check which direction to take
    if ((*middle)[0] >= prefix) {
        // The required elememnt is on the left-hand side
        return LowerBound(range_begin, next(middle), prefix);
    } else if ((*middle)[0] < prefix) {
        // The required elememnt is on the right-hand side
        return LowerBound(middle, range_end, prefix);
    }
}

template <typename RandomIt>
RandomIt UpperBound(RandomIt range_begin, RandomIt range_end, char prefix) {
    if (range_begin == range_end) {
        return range_end;
    }
    if (range_end - range_begin == 1) {
        if (prefix < (*range_begin)[0]) {
            return range_begin;
        } else {
            return range_end;
        }
    }

    if (range_end - range_begin == 2) {
        // Check if we are beyond the first element
        if (prefix < (*range_begin)[0]) {
            return range_begin;
        // Check if we are beyond the last element
        } else if (prefix > (*next(range_begin))[0]) {
            return range_end;
        // If the first one is the required OR the required is in between choose the second one
        // i.e. choose Greater than the required one
        } else {
            return next(range_begin);
        }
    }

    // Find the middle of the range
    auto middle = range_begin + (range_end - range_begin) / 2;
    // Check which direction to take
    if ((*middle)[0] > prefix) {
        // The required elememnt is on the left-hand side
        return UpperBound(range_begin, next(middle), prefix);
    } else if ((*middle)[0] <= prefix) {
        // The required elememnt is on the right-hand side
        return UpperBound(middle, range_end, prefix);
    }
}