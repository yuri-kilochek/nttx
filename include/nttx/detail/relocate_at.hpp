#ifndef NTTX_DETAIL_INCLUDE_DETAIL_RELOCATE_AT
#define NTTX_DETAIL_INCLUDE_DETAIL_RELOCATE_AT

#include <utility>
#include <new>

namespace nttx::detail {
////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
auto relocate_at(T* from, T* to)
-> T*
{
    ::new(static_cast<void*>(to)) T(std::move(*from));
    from->~T();
    return std::launder(to);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif
