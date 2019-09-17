#ifndef NTTX_DETAIL_INCLUDE_DETAIL_UNINITIALIZED_RELOCATE_N
#define NTTX_DETAIL_INCLUDE_DETAIL_UNINITIALIZED_RELOCATE_N

#include <utility>
#include <iterator>
#include <type_traits>
#include <memory>

namespace nttx::detail {
////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename FromInputIterator, typename Count, typename ToForwardIterator>
auto uninitialized_relocate_n(FromInputIterator from, Count count, ToForwardIterator to)
noexcept
-> std::pair<FromInputIterator, ToForwardIterator>
{
    using value_type = typename std::iterator_traits<ToForwardIterator>::value_type;
    static_assert(!std::is_move_constructible_v<value_type> ||
                  std::is_nothrow_move_constructible_v<value_type>);
    for (; count > 0; ++to, (void)++from, --count) {
        ::new(static_cast<void*>(std::addressof(*to))) value_type(std::move(*from));
        from->~value_type();
    }
    return {std::move(from), std::move(to)};
}

////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif
