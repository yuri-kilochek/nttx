#ifndef NTTX_DETAIL_INCLUDE_DETAIL_RELOCATE_AT
#define NTTX_DETAIL_INCLUDE_DETAIL_RELOCATE_AT

#include <nttx/detail/construct_at.hpp>

#include <utility>
#include <memory>

namespace nttx::detail {
////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
auto relocate_at(T* from, T* to)
-> T*
{
    auto result = detail::construct_at(to, std::move(*from));
    std::destroy_at(from);
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif
