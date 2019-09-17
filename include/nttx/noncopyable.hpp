#ifndef NTTX_DETAIL_INCLUDE_NONCOPYABLE
#define NTTX_DETAIL_INCLUDE_NONCOPYABLE

#include <nttx/detail/get_pretty_name.hpp>

#include <stdexcept>
#include <string>
#include <typeinfo>

namespace nttx {
////////////////////////////////////////////////////////////////////////////////////////////////////

struct noncopyable
: std::logic_error
{
    explicit
    noncopyable(std::string const& type_name)
    : std::logic_error("noncopyable type: " + type_name)
    {}

    explicit
    noncopyable(std::type_info const& type_info)
    : noncopyable(detail::get_pretty_name(type_info))
    {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif
