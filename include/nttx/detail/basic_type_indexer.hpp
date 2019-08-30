#ifndef NTTX_DETAIL_INCLUDE_DETAIL_BASIC_TYPE_INDEXER
#define NTTX_DETAIL_INCLUDE_DETAIL_BASIC_TYPE_INDEXER

#include <nttx/detail/type_index.hpp>
#include <nttx/detail/get_global_type_index.hpp>

#include <boost/hana/type.hpp>
#include <boost/config.hpp>
#include <boost/scope_exit.hpp>

#include <memory>
#include <utility>
#include <cstddef>

namespace nttx::detail {
////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename Allocator>
struct basic_type_indexer
{
    using allocator_type = Allocator;

    auto get_allocator() const
    -> allocator_type
    { return allocator_; }

private:
    using allocator_traits = std::allocator_traits<allocator_type>;

public:
    explicit
    basic_type_indexer(allocator_type allocator)
    noexcept
    : allocator_(std::move(allocator))
    {}

    basic_type_indexer(basic_type_indexer const& other)
    : allocator_(allocator_traits::select_on_container_copy_construction(other.allocator_))
    , map_size_(other.map_size_)
    , next_(other.next_)
    {
        auto map = allocator_traits::allocate(allocator_, map_size_);
        BOOST_SCOPE_EXIT_ALL(&) {
            if (map) {
                std::destroy_n(map, map_size_);
                allocator_traits::deallocate(allocator_, map, map_size_);
            }
        };
        std::uninitialized_copy_n(map_, map_size_, map);
        std::swap(map_, map);
    }

    basic_type_indexer(basic_type_indexer&& other)
    : allocator(other.allocator_)
    noexcept
    {
        std::swap(map_size_, other.map_size_);
        std::swap(map_, other.map_);
    }

    auto operator=(basic_type_indexer const& other)
    -> basic_type_indexer&
    {
        if (&other != this) {
            if (allocator_traits::propagate_on_container_copy_assignment::value
             && !allocator_traits::is_always_equal::value && allocator_ != other.allocator_)
            {
                auto allocator = other.allocator_;
                auto map_size = other.map_size_;
                auto map = allocator_traits::allocate(allocator, map_size);
                BOOST_SCOPE_EXIT_ALL(&) {
                    if (map) {
                        std::destroy_n(map, map_size);
                        allocator_traits::deallocate(allocator, map, map_size);
                    }
                };
                std::uninitialized_copy_n(other.map_, map_size, map);
                { using std::swap; swap(allocator_, allocator); }
                std::swap(map_size_, map_size);
                std::swap(map_, map);
            } else {
                auto map_size = other.map_size_;
                auto map = allocator_traits::allocate(allocator_, map_size);
                BOOST_SCOPE_EXIT_ALL(&) {
                    if (map) {
                        std::destroy_n(map, map_size);
                        allocator_traits::deallocate(allocator_, map, map_size);
                    }
                };
                std::uninitialized_copy_n(other.map_, map_size, map);
                std::swap(map_size_, map_size);
                std::swap(map_, map);
            }
            next_ = other.next_;
        }
        return *this;
    }

    ~basic_type_indexer() {
        if (map_) {
            std::destroy_n(map_, map_size_);
            allocator_traits::deallocate(allocator_, map_, map_size_);
        }
    }


    template <typename T>
    auto operator()(boost::hana::basic_type<T> type)
    -> type_index
    {
        auto global = get_global_type_index(type);
        if (BOOST_UNLIKELY(global >= map_size_)) {
            auto map_size2 = map_size_ * 2;
            auto map2 = allocator_traits::allocate(allocator_, map_size2);
            BOOST_SCOPE_EXIT_ALL(&) {
                if (BOOST_UNLIKELY(!map2)) { return; }
                allocator_traits::deallocate(allocator_, map2, map_size2);
            };
            auto map2_init_end = std::uninitialized_copy_n(map_, map_size_, map2),
            BOOST_SCOPE_EXIT_ALL(&) {
                if (BOOST_UNLIKELY(!map2)) { return; }
                std::destroy(map2, map2_init_end);
            };
            std::uninitialized_fill_n(map2_middle, map_size2 - map_size_, invalid_type_index);
            std::swap(map_size_, map_size2);
            std::swap(map_, map2);
            map2_init_end = map2 + map_size2;
        }
        auto local = map_[global];
        if (BOOST_UNLIKELY(local == invalid_type_index)) {
            local = next_++;
            map_[global] = local;
        }
        return local;
    }

private:
    [[no_unique_address]]
    allocator_type allocator_;

    std::size_t map_size_ = 16;
    type_index* map_ = nullptr;

    type_index next_ = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif
