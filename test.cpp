#include <nttx/detail/erased_component_type.hpp>

#include <iostream>

int main() {
    struct V {
        int x, y;

        V(V const&) = delete;
        V(V&&) = default;
    };
    auto& x = nttx::detail::erased_component_type::of<V>;
    x.uninitialized_copy_n(nullptr, 0, nullptr);
    std::cout << x.size;
}
