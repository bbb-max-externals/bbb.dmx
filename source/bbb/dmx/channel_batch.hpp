#pragma once

#include <bbb/dmx/universe.hpp>
#include <cmath>
#include <cstddef>
#include <limits>

namespace bbb::dmx {

// read(index) returns NaN for nonnumeric atoms. No writes until every tuple passes.
template<typename Reader>
const char* channel_batch_error(std::size_t count, bool with_universes, Reader read) {
    const std::size_t stride = with_universes ? 3 : 2;
    if(!count || count % stride) return "incomplete channel batch";
    for(std::size_t i = 0; i < count; ++i) {
        const double number = std::trunc(read(i));
        if(!std::isfinite(number) || number < std::numeric_limits<int>::min() ||
            std::numeric_limits<int>::max() < number) return "channels require numeric integer-convertible values";
        if(i % stride == stride - 2 && (number < 1 || universe_channel_count < number))
            return "channel address outside 1..512";
    }
    return nullptr;
}

}
