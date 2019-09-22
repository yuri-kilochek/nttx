#ifndef NTTX_DETAIL_INCLUDE_DETAIL_COMPONENT_BUFFER
#define NTTX_DETAIL_INCLUDE_DETAIL_COMPONENT_BUFFER

#include <nttx/detail/erased_component_type.hpp>

#include <cstddef>

namespace nttx::detail {
////////////////////////////////////////////////////////////////////////////////////////////////////

struct component_buffer_view
: strided_ptr<void>
{
    erased_component_type const& type;
    intrusive_list_link<component_buffer_view> list_link;
};

template <typename Allocator>
struct component_buffer {
    template <typename TypesForwardIterator>
    explicit
    component_buffer(TypesForwardIterator types_begin, TypesForwardIterator const& types_end,
                     Allocator const& allocator)
    : views_(std::distance(types_begin, types_end), allocator)
    {
        for (auto& view : views_) {
            view.type = &*types;
            ++types;
        }
    }


    ~component_buffer() {
        for (auto& view : views_) {
            if (view.stride != view.type.size) { continue; }
            view.type.destroy_n(view.raw, size_);
        }
        allocator_.deallocate(views_[0].raw);
    }

    auto get_allocator() const
    -> Allocator 
    { return views_.get_allocator(); }

private:
    dynamic_array<component_buffer_view, Allocator> views_;
    std::size_t size_ = 0;
    std::size_t capacity_ = 0;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif
