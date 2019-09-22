#ifndef NTTX_DETAIL_INCLUDE_DETAIL_TYPE_INDEXER
#define NTTX_DETAIL_INCLUDE_DETAIL_TYPE_INDEXER

#include <cstdint>
#include <atomic>

namespace nttx::detail {
////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename Domain>
struct type_indexer {
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
