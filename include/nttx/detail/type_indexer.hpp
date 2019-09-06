#ifndef NTTX_DETAIL_INCLUDE_DETAIL_BASIC_TYPE_INDEXER
#define NTTX_DETAIL_INCLUDE_DETAIL_BASIC_TYPE_INDEXER

#include <nttx/detail/get_global_type_index.hpp>

#include <boost/config.hpp>
#include <boost/hana/type.hpp>

#include <cstddef>
#include <memory>
#include <limits>
#include <utility>
#include <stdexcept>
#include <vector>

namespace nttx::detail {
////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename Domain, typename Index = std::size_t,
          typename Allocator = std::allocator<Index>>
struct type_indexer
{
    using domain_type = Domain;

    using index_type = Index;

    using allocator_type = Allocator;

private:
    static constexpr
    auto const invalid_index = std::numeric_limits<index_type>::max();

public:
    type_indexer()
    noexcept(noexcept(allocator_type()))
    = default;

    explicit
    type_indexer(allocator_type const& allocator)
    noexcept
    : indices_(allocator)
    {}

    type_indexer(type_indexer&& other, allocator_type const& allocator)
    : indices_(std::move(other.indices_), allocator)
    , next_index_(other.next_index_)
    {
        if (other.indices_.empty()) {
            other.next_index_ = 0;
        }
    }

    type_indexer(type_indexer&& other)
    noexcept
    : indices_(std::move(other.indices_))
    , next_index_(std::exchange(other.next_index_, 0))
    {}

    type_indexer(type_indexer const& other, allocator_type const& allocator)
    : indices_(other.indices_, allocator)
    , next_index_(other.next_index_)
    {}

    type_indexer(type_indexer const& other)
    = default;

    auto& operator=(type_indexer&& other)
    noexcept(noexcept(this->indices_ = std::move(other.indices_)))
    {
        if (&other != this) {
            indices_ = std::move(other.indices_);
            next_index_ = other.next_index_;
            if (other.indices_.empty()) {
                other.next_index_ = 0;
            }
        }
        return *this;
    }

    auto operator=(type_indexer const& other)
    -> type_indexer&
    = default;

    void swap(type_indexer& other)
    noexcept(noexcept(std::swap(this->indices_, other.indices_)))
    {
        std::swap(indices_, other.indices_);
        std::swap(next_index_, other.next_index_);
    }

    auto get_allocator() const
    -> allocator_type
    {
        return indices_.get_allocator();
    }

    template <typename T>
    BOOST_FORCEINLINE
    auto operator()(boost::hana::basic_type<T> type)
    -> index_type
    {
        auto global_index = get_global_type_index<domain_type>(type);
        if (BOOST_UNLIKELY(global_index >= indices_.size())) {
            grow(global_index);
        }
        auto index = indices_[global_index];
        if (BOOST_UNLIKELY(index == invalid_index)) {
            index = occupy(global_index);
        }
        return index;
    }

    auto limit() const
    -> std::size_t
    {
        return next_index_;
    }

    void reset() {
        indices_.clear();
        next_index_ = 0;
    }

    void tighten() {
        indices_.shrink_to_fit();
    }

private:
    std::vector<index_type, allocator_type> indices_;
    std::size_t next_index_ = 0;

    BOOST_NOINLINE
    void grow(std::size_t global_index) {
        indices_.resize(global_index + 1, invalid_index);
    }

    BOOST_NOINLINE
    auto occupy(std::size_t global_index)
    -> index_type
    {
        if (BOOST_UNLIKELY(next_index_ == invalid_index)) {
            throw std::range_error("too many indexed types");
        }
        auto index = next_index_++;
        indices_[global_index] = index;
        return index;
    }
};

template <typename Domain, typename Index, typename Allocator>
void swap(type_indexer<Domain, Index, Allocator>& a, type_indexer<Domain, Index, Allocator>& b)
noexcept(noexcept(a.swap(b)))
{
    a.swap(b);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif
