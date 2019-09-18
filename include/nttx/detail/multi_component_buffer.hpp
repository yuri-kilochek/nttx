#ifndef NTTX_DETAIL_INCLUDE_DETAIL_MULTI_COMPONENT_BUFFER
#define NTTX_DETAIL_INCLUDE_DETAIL_MULTI_COMPONENT_BUFFER

#include <atomic>
#include <cstddef>

namespace nttx::detail {
////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename Allocator>
struct multi_component_buffer {
    [[no_unique_address]]
    allocator_type allocator;

    std::byte *bytes;
    multi_component_buffer* views;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif
