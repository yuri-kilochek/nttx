#ifndef NTTX_DETAIL_INCLUDE_DETAIL_COMPONENT_BUFFER
#define NTTX_DETAIL_INCLUDE_DETAIL_COMPONENT_BUFFER

#include <cstddef>

namespace nttx::detail {
////////////////////////////////////////////////////////////////////////////////////////////////////

struct component_buffer_view {
    std::byte* bytes;
    component_type const* type;
};

template <typename Allocator = std::allocator<void>>
struct component_buffer
{
    explicit

private:
    [[no_unique_address]]
    allocator_type allocator_;

    component_array_view *views;

    std::size_t size;
    std::size_t capacity;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif
