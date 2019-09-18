#ifndef NTTX_DETAIL_INCLUDE_DETAIL_GET_PRETTY_NAME
#define NTTX_DETAIL_INCLUDE_DETAIL_GET_PRETTY_NAME

#if __has_include(<cxxabi.h>)
    #include <cxxabi.h>

    #ifndef __GABIXX_CXXABI_H__
        #define NTTX_DETAIL_HAVE_CXA_DEMANGLE
    #endif
#endif

#include <typeinfo>
#include <string>
#ifdef NTTX_DETAIL_HAVE_CXA_DEMANGLE
    #include <memory>
    #include <stdlib.h>
    #include <new>
    #include <stdexcept>
#endif

namespace nttx::detail {
////////////////////////////////////////////////////////////////////////////////////////////////////

inline
auto get_pretty_name(std::type_info const& type_info)
-> std::string
{
    #ifdef NTTX_DETAIL_HAVE_CXA_DEMANGLE
        int status;
        std::unique_ptr<char, decltype(::free)&>
            pretty(::abi::__cxa_demangle(type_info.name(), nullptr, nullptr, &status), ::free);
        if (!pretty) {
            if (status == -1) { throw std::bad_alloc(); }
            throw std::runtime_error("::abi::__cxa_demangle failed");
        }
        return pretty.get();
    #else
        return type_info.name();
    #endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif
