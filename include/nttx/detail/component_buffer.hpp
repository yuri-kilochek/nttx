#ifndef NTTX_DETAIL_INCLUDE_DETAIL_COMPONENT_BUFFER
#define NTTX_DETAIL_INCLUDE_DETAIL_COMPONENT_BUFFER

#include <nttx/detail/erased_component_type.hpp>

#include <cstddef>

namespace nttx::detail {
////////////////////////////////////////////////////////////////////////////////////////////////////

struct component_buffer_view {
    std::byte* bytes = nullptr;
    erased_component_type const* type = nullptr;
};

struct component_buffer {
    component_array_view *views = nullptr;
    std::size_t size = 0;
    std::size_t capacity = 0;

    template <typename ProtoAllocator>
    void initialize_with(std::span<erased_component_type const*> types,
                         erased_component_type const* new_type,
                         ProtoAllocator const& proto_allocator = ProtoAllocator())
    {
        auto allocator = rebind_allocator<component_buffer_view>(proto_allocator);

    }
};


////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif
