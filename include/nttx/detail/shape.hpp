#ifndef NTTX_DETAIL_INCLUDE_DETAIL_SHAPE
#define NTTX_DETAIL_INCLUDE_DETAIL_SHAPE

#include <atomic>
#include <cstddef>

namespace nttx::detail {
////////////////////////////////////////////////////////////////////////////////////////////////////

struct component_meta_t {
    std::size_t size;
    std::size_t alignment;

    void (*default_construct)(void *at, std::size_t count);
    void (*move_assign)(void *from, void *to, std::size_t count) noexcept;
    void (*destruct)(void *at, std::size_t count) noexcept;
};

template <typename ComponentType>
constexpr
component_meta_t component_meta = {
    .size = sizeof(ComponentType),
    .alignment = alignof(ComponentType),
    .destruct = [](void *at, std::size_t count) noexcept {
        std::destroy_n(reinterpret_cast<ComponentType*>(at), count);
    },
};

struct components_view {
    std::byte* base;
    component_meta_t const* meta;
};

template <typename Allocator>
struct multi_component_storage {
    [[no_unique_address]]
    allocator_type allocator;

    std::byte *buffer;
    component_storage_view* views;
};

template <typename ComponentTypeIndex, typename Allocator>
struct shape {
    using component_type_index_type = ComponentTypeIndex;

    using allocator_traits = std::alocator_traits<Allocator>;

    dynamic_array<component_type_index_type, allocator_traits::template rebind_alloc<component_type_index_type>> component_type_indices;

    type_map<allocator_traits::template rebind_alloc<void>> components;

    std::vector<shape*, allocator_traits::template rebind_alloc<shape*>> transitions;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif
