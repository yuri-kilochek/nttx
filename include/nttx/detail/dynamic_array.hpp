#ifndef NTTX_DETAIL_INCLUDE_DETAIL_DYNAMIC_ARRAY
#define NTTX_DETAIL_INCLUDE_DETAIL_DYNAMIC_ARRAY

#include <type_traits>
#include <memory>

namespace nttx::detail {
////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T, typename Allocator = std::allocator<T>>
struct dynamic_array {
    using value_type = T;
    using allocator_type = Allocator;
    using pointer = typename std::allocator_traits<allocator_type>::pointer;
    using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;
    using reference = value_type&;
    using const_reference = value_type const&;
    using size_type = typename std::allocator_traits<allocator_type>::size_type;
    using difference_type = typename std::allocator_traits<allocator_type>::difference_type;
    using iterator = value_type*;
    using const_iterator = value_type const*;

    dynamic_array()
    
    {}

    ~dynamic_array() {
        if (size_ != 0) {
            std::destroy(begin(), end());
            allocator_traits::deallocate(allocator_, data_, size_);
        }
    }

    [[nodiscard]]
    auto get_allocator()
    const
    -> allocator_type
    { return allocator_; }


    [[nodiscard]]
    auto begin()
    noexcept
    -> iterator
    { return data(); }

    [[nodiscard]]
    auto begin()
    const noexcept
    -> const_iterator
    { return data(); }

    [[nodiscard]]
    auto end()
    noexcept
    -> iterator
    { return data() + size_; }

    [[nodiscard]]
    auto end()
    const noexcept
    -> const_iterator
    { return data() + size_; }


    [[nodiscard]]
    auto size()
    const noexcept
    -> size_type
    { return size_; }


    [[nodiscard]]
    auto operator[](size_type i)
    -> reference
    { return data()[i]; }

    [[nodiscard]]
    auto operator[](size_type i)
    const
    -> const_reference
    { return data()[i]; }


    [[nodiscard]]
    auto data()
    noexcept
    -> value_type*
    { return std::to_address(data_); }

    [[nodiscard]]
    auto data()
    const noexcept
    -> value_type const*
    { return std::to_address(data_); }

private:
    using allocator_traits = std::allocator_traits<allocator_type>;

    static_assert(std::is_same_v<value_type, typename allocator_traits::value_type>);

    pointer data_;
    size_type size_;

    [[no_unique_address]]
    allocator_type allocator_;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif
