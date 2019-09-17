#ifndef NTTX_DETAIL_INCLUDE_DETAIL_ARG
#define NTTX_DETAIL_INCLUDE_DETAIL_ARG

namespace nttx::detail {
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename ArgTag, typename Default, typename... Args>
struct template_arg
: Default
{};

template <typename Default, typename Arg, typename... Args>
struct template_arg<Arg::tag, Default, Arg, Args...>
: Arg
{};

template <typename ArgTag, typename Default, typename Arg, typename... Args>
struct template_arg<ArgTag, Default, Arg, Args...>
: template_arg<ArgTag, Default, Args...>
{};


struct no_default_arg {};

template <typename ArgTag, typename... Args>
using required_arg_t = typename template_arg<ArgTag, no_default_arg, Args...>::type;

template <typename ArgTag, auto DefaultValue, typename... Args>
constexpr auto required_arg_v = template_arg<ArgTag, no_default_arg, Args...>::value;

template <typename Type>
struct default_arg_type {
    using type = Type;
};

template <auto Value>
struct default_arg_value {
    constexpr auto value = Value;
};

template <typename ArgTag, typename DefaultType, typename... Args>
using arg_t = typename template_arg<ArgTag, default_arg_type<DefaultType>, Args...>::type;

template <typename ArgTag, auto DefaultValue, typename... Args>
constexpr auto arg_v = template_arg<ArgTag, default_arg_value<DefaultValue>, Args...>::value;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif
