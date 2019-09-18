#include <nttx/detail/erased_component_type.hpp>

#include <iostream>

int main() {
    struct V {
        int x, y;

        V(V const&) = delete;
        V(V&&) = default;
    };
    auto& x = nttx::detail::erased_component_type::of<V>;
    x.copy_construct_at(nullptr, nullptr);
    std::cout << x.size;
}
