#ifndef NTTX_DETAIL_INCLUDE_DETAIL_ERASED_COMPONENT_TYPE
#define NTTX_DETAIL_INCLUDE_DETAIL_ERASED_COMPONENT_TYPE

#include <nttx/detail/uninitialized_relocate_n.hpp>
#include <nttx/noncopyable.hpp>

#include <cstddef>
#include <type_traits>
#include <memory>
#include <typeinfo>

namespace nttx::detail {
////////////////////////////////////////////////////////////////////////////////////////////////////

struct erased_component_type {
    std::size_t size;
    std::size_t alignment;

    void (*uninitialized_copy_n)(void const *from, std::size_t count, void *to);
    void (*uninitialized_relocate_n)(void *from, std::size_t count, void *to) noexcept;
    void (*destroy_n)(void *at, std::size_t count) noexcept;

    template <typename T>
    static constexpr
    erased_component_type const of = {
        .size = sizeof(T),
        .alignment = alignof(T),

        .uninitialized_copy_n = [](void const *from, std::size_t count, void *to) {
            if constexpr (std::is_copy_constructible_v<T>) {
                std::uninitialized_copy_n(static_cast<T const*>(from), count, static_cast<T*>(to));
            } else {
                throw noncopyable(typeid(T));
            }
        },

        .uninitialized_relocate_n = [](void *from, std::size_t count, void *to) noexcept {
            uninitialized_relocate_n(static_cast<T*>(from), count, static_cast<T*>(to));
        },

        .destroy_n = [](void *at, std::size_t count) noexcept {
            std::destroy_n(static_cast<T*>(at), count);
        },
    };
};

////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif
