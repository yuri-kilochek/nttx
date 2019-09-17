#ifndef NTTX_DETAIL_INCLUDE_DETAIL_DEMANGLE
#define NTTX_DETAIL_INCLUDE_DETAIL_DEMANGLE

#if __has_include(<cxxabi.h>)
    #include <cxxabi.h>

    #include <new>
    #include <stdexcept>
#endif

#include <type_traits>
#include <string_view>

namespace nttx::detail {
////////////////////////////////////////////////////////////////////////////////////////////////////

namespace abi_scope {
    #if __has_include(<cxxabi.h>)
        using namespace ::abi;
    #endif

    template <typename = int, typename = void>
    struct have_cxa_demandle
    : std::false_type
    {};

    template <typename Int>
    struct have_cxa_demandle<Int, std::void_t<decltype(
        __cxa_demangle((char const*)0, 0, 0, (Int*)0)
    )>>
    : std::true_type
    {};
}

constexpr auto have_abi_cxa_demangle_v = abi_scope::have_cxa_demandle<>::value;

inline
auto demangle(char const* mangled)
-> std::string
{
    if constexpr (have_abi_cxa_demangle_v) {
        int status;
        std::unique_ptr demangled(abi::__cxa_demangle(mangled, nullptr, nullptr, &status), free);
        if (!demangled) {
            switch (status) {
            case -1: 
                throw std::bad_alloc();
            case -2:
            case -3:
                throw std::invalid_argument();
            default:
            }
        }
    } else {
        return mangled;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif
