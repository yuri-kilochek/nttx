#ifndef NTTX_DETAIL_INCLUDE_DETAIL_UNINITIALIZED_RELOCATE_N
#define NTTX_DETAIL_INCLUDE_DETAIL_UNINITIALIZED_RELOCATE_N

#include <nttx/detail/relocate_at.hpp>

#include <utility>
#include <iterator>
#include <type_traits>
#include <memory>

namespace nttx::detail {
////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename FromForwardIterator, typename Count, typename ToForwardIterator>
auto uninitialized_relocate_n(FromForwardIterator from, Count count, ToForwardIterator to)
-> std::pair<FromForwardIterator, ToForwardIterator>
{
    using from_value_type = typename std::iterator_traits<FromForwardIterator>::value_type;
    using to_value_type = typename std::iterator_traits<ToForwardIterator>::value_type;
    if constexpr (std::is_nothrow_constructible_v<to_value_type, from_value_type&&> &&
                  std::is_nothrow_destructible_v<from_value_type>)
    {
        for (; count > 0; ++to, (void)++from, --count) {
            relocate_at(std::addressof(*from), std::addressof(*to));
        }
        return {std::move(from), std::move(to)};
    } else {
        auto iterators = std::uninitialized_move_n(from, count, to);
        std::destroy_n(from, count);
        return iterators;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif
