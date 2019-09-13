#ifndef NTTX_DETAIL_INCLUDE_DETAIL_SHAPE
#define NTTX_DETAIL_INCLUDE_DETAIL_SHAPE

#include <atomic>
#include <cstddef>

namespace nttx::detail {
////////////////////////////////////////////////////////////////////////////////////////////////////

struct component_storage_view {
    std::byte* base;
    std::size_t stride;

    void (*default_construct)(void *at, std::size_t count);
    void (*move_assign)(void *from, void *to, std::size_t count);
    void (*destruct)(void *at, std::size_t count);

    ~component_storage_view() {

    }
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
