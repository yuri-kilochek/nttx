#ifndef NTTX_DETAIL_INCLUDE_DETAIL_BASIC_TYPE_INDEXER
#define NTTX_DETAIL_INCLUDE_DETAIL_BASIC_TYPE_INDEXER

#include <nttx/detail/get_global_type_index.hpp>

#include <boost/scope_exit.hpp>
#include <boost/config.hpp>
#include <boost/hana/type.hpp>

#include <cstddef>
#include <memory>
#include <limits>
#include <utility>
#include <stdexcept>

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
    auto invalid_index = std::numeric_limits<index_type>::max();

    using allocator_traits = std::allocator_traits<allocator_type>;

public:
    type_indexer()
    noexcept(noexcept(allocator_type()))
    = default;

    explicit
    type_indexer(allocator_type allocator)
    noexcept
    : allocator_(std::move(allocator))
    {}

    type_indexer(type_indexer const& other, allocator_type allocator)
    : allocator_(std::move(allocator))
    , capacity_(other.capacity_)
    , next_(other.next_)
    {
        auto locals2 = allocator_traits::allocate(allocator_, capacity_);
        BOOST_SCOPE_EXIT_ALL(&) {
            if (!locals2) { return; }
            allocator_traits::deallocate(allocator_, locals2, capacity_);
        };
        std::uninitialized_copy_n(locals_, capacity_, locals2);
        BOOST_SCOPE_EXIT_ALL(&) {
            if (!locals2) { return; }
            std::destroy_n(locals2, capacity_);
        };
        std::swap(locals_, locals2);
    }

    type_indexer(type_indexer const& other)
    : type_indexer(other, allocator_traits::select_on_container_copy_construction(other.allocator_))
    {}

    type_indexer(type_indexer&& other)
    noexcept
    : allocator_(other.allocator_)
    {
        std::swap(locals_, other.locals_);
        std::swap(capacity_, other.capacity_);
        std::swap(next_, other.next_);
    }

    auto operator=(type_indexer const& other)
    -> type_indexer&
    {
        if (&other != this) {
            if (allocator_traits::propagate_on_container_copy_assignment::value
             && !allocator_traits::is_always_equal::value && allocator_ != other.allocator_)
            {
                auto allocator2 = other.allocator_;
                auto capacity2 = other.capacity_;
                auto locals2 = allocator_traits::allocate(allocator2, capacity2);
                BOOST_SCOPE_EXIT_ALL(&) {
                    if (!locals2) { return; }
                    allocator_traits::deallocate(allocator2, locals2, capacity2);
                };
                std::uninitialized_copy_n(other.locals_, other.capacity_, locals2);
                BOOST_SCOPE_EXIT_ALL(&) {
                    if (!locals2) { return; }
                    std::destroy_n(locals2, capacity2);
                };
                { using std::swap; swap(allocator_, allocator2); }
                std::swap(locals_, locals2);
                std::swap(capacity_, capacity2);
            } else {
                auto capacity2 = other.capacity_;
                auto locals2 = allocator_traits::allocate(allocator_, capacity2);
                BOOST_SCOPE_EXIT_ALL(&) {
                    if (!locals2) { return; }
                    allocator_traits::deallocate(allocator_, locals2, capacity2);
                };
                std::uninitialized_copy_n(other.locals_, other.capacity_, locals2);
                BOOST_SCOPE_EXIT_ALL(&) {
                    if (!locals2) { return; }
                    std::destroy_n(locals2, capacity2);
                };
                std::swap(locals_, locals2);
                std::swap(capacity_, capacity2);
            }
            next_ = other.next_;
        }
        return *this;
    }

    auto operator=(type_indexer&& other)
    -> type_indexer&
    {
        if (&other != this) {
            //
            //
            // TODO
            //
            //

            //if (allocator_ != other.allocator_ && )
            //std::destroy_n(locals_, capacity_);
            //allocator_traits::deallocate(allocator_, locals_, capacity_);

            //if (allocator_traits::propagate_on_container_move_assignment::value
            // && !allocator_traits::is_always_equal::value && allocator_ != other.allocator_)
            //{

            //    auto allocator2 = other.allocator_;
            //    auto capacity2 = other.capacity_;
            //    auto locals2 = allocator_traits::allocate(allocator2, capacity2);
            //    BOOST_SCOPE_EXIT_ALL(&) {
            //        if (!locals2) { return; }
            //        allocator_traits::deallocate(allocator2, locals2, capacity2);
            //    };
            //    std::uninitialized_copy_n(other.locals_, other.capacity_, locals2);
            //    BOOST_SCOPE_EXIT_ALL(&) {
            //        if (!locals2) { return; }
            //        std::destroy_n(locals2, capacity2);
            //    };
            //    { using std::swap; swap(allocator_, allocator2); }
            //    std::swap(locals_, locals2);
            //    std::swap(capacity_, capacity2);
            //} else {
            //}
            next_ = other.next_;
        }
        return *this;
    }

    ~type_indexer() {
        if (!locals_) { return; }
        std::destroy_n(locals_, capacity_);
        allocator_traits::deallocate(allocator_, locals_, capacity_);
    }

    auto get_allocator() const
    -> allocator_type
    { return allocator_; }

    template <typename T>
    BOOST_FORCEINLINE
    auto operator()(boost::hana::basic_type<T> type)
    -> index_type
    {
        auto global = get_global_type_index<domain_type>(type);
        if (BOOST_UNLIKELY(global >= capacity_)) {
            expand(global);
        }
        auto local = locals_[global];
        if (BOOST_UNLIKELY(local == invalid_index)) {
            advance(global, local);
        }
        return local;
    }

private:
    [[no_unique_address]]
    allocator_type allocator_;

    index_type* locals_ = nullptr;
    std::size_t capacity_ = 0;

    std::size_t next_ = 0;

    BOOST_NOINLINE
    void expand(std::size_t global) {
        auto capacity2 = global + 1;
        auto locals2 = allocator_traits::allocate(allocator_, capacity2);
        BOOST_SCOPE_EXIT_ALL(&) {
            if (!locals2) { return; }
            allocator_traits::deallocate(allocator_, locals2, capacity2);
        };
        auto init_end = std::uninitialized_copy_n(locals_, capacity_, locals2);
        BOOST_SCOPE_EXIT_ALL(&) {
            if (!locals2) { return; }
            std::destroy(locals2, init_end);
        };
        init_end = std::uninitialized_fill_n(init_end, capacity2 - capacity_, invalid_index);
        std::swap(capacity_, capacity2);
        std::swap(locals_, locals2);
        init_end = locals2 + capacity2;
    }

    BOOST_NOINLINE
    void advance(std::size_t global, index_type& local) {
        if (BOOST_UNLIKELY(next_ == invalid_index)) {
            throw std::range_error("too many indexed types");
        }
        locals_[global] = local = next_++;
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif
