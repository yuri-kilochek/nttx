#ifndef NTTX_DETAIL_INCLUDE_DETAIL_GET_PRETTY_NAME
#define NTTX_DETAIL_INCLUDE_DETAIL_GET_PRETTY_NAME

#include <nttx/detail/demangle.hpp>

#include <typeinfo>
#include <string>

namespace nttx::detail {
////////////////////////////////////////////////////////////////////////////////////////////////////

inline
auto get_pretty_name(std::type_info const& type_info)
-> std::string
{
    return demangle(type_info.name());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif
