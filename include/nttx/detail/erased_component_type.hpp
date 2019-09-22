#ifndef NTTX_DETAIL_INCLUDE_DETAIL_ERASED_COMPONENT_TYPE
#define NTTX_DETAIL_INCLUDE_DETAIL_ERASED_COMPONENT_TYPE

#include <nttx/detail/component_type_indexer.hpp>
#include <nttx/detail/construct_at.hpp>
#include <nttx/noncopyable.hpp>
#include <nttx/detail/relocate_at.hpp>
#include <nttx/detail/uninitialized_relocate_n.hpp>

#include <cstddef>
#include <type_traits>
#include <memory>
#include <typeinfo>

namespace nttx::detail {
///////////////////////////////////////////////////////////////////////////////////////////////////

struct erased_component_type {
    std::size_t size;
    std::size_t alignment;

    component_type_indexer::index_type index;

    void (*copy_construct_at)(void *at, void const *from);
    void (*uninitialized_copy_n)(void const *from, std::size_t count, void *to);

    void (*relocate_at)(void* from, void* to);
    void (*uninitialized_relocate_n)(void *from, std::size_t count, void *to);

    void (*destroy_at)(void *at) noexcept;
    void (*destroy_n)(void *at, std::size_t count) noexcept;

    template <typename T>
    static
    auto get()
    -> erased_component_type const&
    {
        static
        erased_component_type const instance = {
            .size = sizeof(T),
            .alignment = alignof(T),

            .index = component_type_indexer::get_index<T>(),

            .copy_construct_at = [](void *at, void const *from) {
                if constexpr (std::is_copy_constructible_v<T>) {
                    detail::construct_at(static_cast<T*>(at), *static_cast<T const*>(from));
                } else {
                    (void)at;
                    (void)from;
                    throw noncopyable(typeid(T));
                }
            },
            .uninitialized_copy_n = [](void const *from, std::size_t count, void *to) {
                if constexpr (std::is_copy_constructible_v<T>) {
                    std::uninitialized_copy_n(static_cast<T const*>(from), count,
                                              static_cast<T*>(to));
                } else {
                    (void)from;
                    (void)count;
                    (void)to;
                    throw noncopyable(typeid(T));
                }
            },

            .relocate_at = [](void *from, void *to) {
                detail::relocate_at(static_cast<T*>(from), static_cast<T*>(to));
            },
            .uninitialized_relocate_n = [](void *from, std::size_t count, void *to) {
                detail::uninitialized_relocate_n(static_cast<T*>(from), count,
                                                 static_cast<T*>(to));
            },

            .destroy_at = [](void *at) noexcept {
                std::destroy_at(static_cast<T*>(at));
            },
            .destroy_n = [](void *at, std::size_t count) noexcept {
                std::destroy_n(static_cast<T*>(at), count);
            },
        };
        return instance;
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif
