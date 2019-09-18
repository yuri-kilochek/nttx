#ifndef NTTX_DETAIL_INCLUDE_DETAIL_GLOBAL_COMPONENT_TYPE_REGISTRY
#define NTTX_DETAIL_INCLUDE_DETAIL_GLOBAL_COMPONENT_TYPE_REGISTRY

#include <nttx/detail/global_type_registry.hpp>

namespace nttx::detail {
////////////////////////////////////////////////////////////////////////////////////////////////////

struct component_domain;

using global_component_type_registry = global_type_registry<component_domain>;

////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif
