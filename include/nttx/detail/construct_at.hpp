#ifndef NTTX_DETAIL_INCLUDE_DETAIL_CONSTRUCT_AT
#define NTTX_DETAIL_INCLUDE_DETAIL_CONSTRUCT_AT

#include <new>
#include <utility>

namespace nttx::detail {
////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T, typename... Args>
auto construct_at(T* location, Args&&... args)
-> T*
{ return ::new(static_cast<void*>(location)) T(std::forward<Args>(args)...); }

////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif
