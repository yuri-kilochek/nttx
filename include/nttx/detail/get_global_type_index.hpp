#ifndef NTTX_DETAIL_INCLUDE_DETAIL_GET_GLOBAL_TYPE_INDEX
#define NTTX_DETAIL_INCLUDE_DETAIL_GET_GLOBAL_TYPE_INDEX

#include <boost/config.hpp>
#include <boost/hana/type.hpp>

#include <atomic>
#include <cstddef>

namespace nttx::detail {
////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename Domain>
inline
std::atomic<std::size_t> next_global_type_index;

template <typename Domain, typename Type>
BOOST_FORCEINLINE
auto get_global_type_index(boost::hana::basic_type<Type>)
-> std::size_t 
{
    static
    auto const index = next_global_type_index<Domain>.fetch_add(1, std::memory_order_relaxed);

    return index;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif
