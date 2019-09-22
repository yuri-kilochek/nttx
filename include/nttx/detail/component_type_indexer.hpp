#ifndef NTTX_DETAIL_INCLUDE_DETAIL_COMPONENT_TYPE_INDEXER
#define NTTX_DETAIL_INCLUDE_DETAIL_COMPONENT_TYPE_INDEXER

#include <nttx/detail/type_indexer.hpp>

namespace nttx::detail {
////////////////////////////////////////////////////////////////////////////////////////////////////

struct component_type_index_domain;

using component_type_indexer = type_indexer<component_type_index_domain>;

////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif
