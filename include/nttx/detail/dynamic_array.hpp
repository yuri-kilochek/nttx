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

private:
    using allocator_traits = std::allocator_traits<allocator_type>;

public:
    using pointer = typename allocator_traits::pointer;
    using const_pointer = typename allocator_traits::const_pointer;
    using reference = value_type&;
    using const_reference = value_type const&;
    using size_type = typename allocator_traits::size_type;
    using difference_type = typename allocator_traits::difference_type;
    using iterator = value_type*;
    using const_iterator = value_type const*;

    dynamic_array()
    noexcept(noexcept(allocator_type()))
    : dynamic_array(allocator_type())
    {}

    explicit
    dynamic_array(allocator_type const& allocator)
    noexcept
    : allocator_(allocator)
    {}

    explicit
    dynamic_array(size_type size, allocator_type const& allocator = allocator_type())
    : dynamic_array(allocator)
    { resize(size); }

    explicit
    dynamic_array(size_type size, value_type const& value,
                  allocator_type const& allocator = allocator_type())
    : dynamic_array(allocator)
    { assign(size, value); }

    template <typename ForwardIterator>
    explicit
    dynamic_array(ForwardIterator const& begin, ForwardIterator const& end,
                  allocator_type const& allocator = allocator_type())
    : dynamic_array(allocator)
    { assign(begin, end); }

    dynamic_array(dynamic_array const& other, allocator_type const& allocator)
    : dynamic_array(other.begin(), other.end(), allocator)
    {}

    dynamic_array(dynamic_array const& other)
    : dynamic_array(other, other.allocator_)
    {}

    dynamic_array(dynamic_array&& other, allocator_type const& allocator)
    noexcept(allocator_traits::is_always_equal::value)
    : allocator_(allocator)
    {
        if constexpr (!allocator_traits::is_always_equal::value) {
            if (allocator_ != other.allocator_) {
                assign(std::make_move_iterator(other.begin()),
                       std::make_move_iterator(other.end()));
                return;
            }
        }
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    }

    dynamic_array(dynamic_array&& other)
    noexcept
    : data_(std::exchange(other.data_, nullptr))
    , size_(std::exchange(other.size_, 0))
    , allocator_(other.allocator_)
    {}

    ~dynamic_array() {
        clear();
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

    static_assert(std::is_same_v<value_type, typename allocator_traits::value_type>);

    pointer data_ = nullptr;
    size_type size_ = 0;

    [[no_unique_address]]
    allocator_type allocator_;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif
