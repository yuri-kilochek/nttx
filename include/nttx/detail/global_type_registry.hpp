#ifndef NTTX_DETAIL_INCLUDE_DETAIL_GLOBAL_TYPE_REGISTRY
#define NTTX_DETAIL_INCLUDE_DETAIL_GLOBAL_TYPE_REGISTRY

#include <cstdint>
#include <atomic>

namespace nttx::detail {
////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename Domain>
struct global_type_registry {
    using index_type = std::uint_least32_t;

    template <typename>
    static
    auto get_index()
    -> index_type
    {
        static
        auto const index = index_limit_.fetch_add(1, std::memory_order_relaxed);

        return index;
    }

    static
    auto get_index_limit()
    -> index_type
    { return index_limit_.load(std::memory_order_relaxed); }

private:
    static inline
    std::atomic<index_type> index_limit_;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif
