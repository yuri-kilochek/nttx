#ifndef NTTX_DETAIL_INCLUDE_DETAIL_GET_GLOBAL_TYPE_INDEX
#define NTTX_DETAIL_INCLUDE_DETAIL_GET_GLOBAL_TYPE_INDEX

#include <nttx/detail/type_index.hpp>

#include <boost/hana/type.hpp>

#include <atomic>

namespace nttx::detail {
////////////////////////////////////////////////////////////////////////////////////////////////////

inline
std::atomic<type_index> next_global_type_index;

template <typename T>
inline
auto get_global_type_index(boost::hana::basic_type<T>)
-> type_index 
{
    static
    auto const index = next_global_type_index.fetch_add(1, std::memory_order_relaxed);
    return index;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif
