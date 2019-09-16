#ifndef NTTX_DETAIL_INCLUDE_DETAIL_COMPONENT_TYPE_DESCRIPTION
#define NTTX_DETAIL_INCLUDE_DETAIL_COMPONENT_TYPE_DESCRIPTION

#include <cstddef>
#include <memory>

namespace nttx::detail {
////////////////////////////////////////////////////////////////////////////////////////////////////

struct component_type_description {
    std::size_t size;
    std::size_t alignment;

    void (*uninitialized_move_n)(void *from, std::size_t count, void *to) noexcept;
    void (*destroy_n)(void *at, std::size_t count) noexcept;

    template <typename T>
    static constexpr
    component_type_description const of = {
        .size = sizeof(T),
        .alignment = alignof(T),

        .uninitialized_move_n = [](void *from, std::size_t count, void *to) noexcept {
            std::uninitialized_move_n(static_cast<T*>(from), count, static_cast<T*>(to));
        },

        .destroy_n = [](void *at, std::size_t count) noexcept {
            std::destroy_n(static_cast<T*>(at), count);
        },
    };
};

////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif
