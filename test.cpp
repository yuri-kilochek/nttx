#include <nttx/detail/dynamic_array.hpp>

#include <iostream>

int main() {
    nttx::detail::dynamic_array<int> x;
    x.data();


    //try {
    //    struct V {
    //        int x, y;

    //        //V(V const&) = delete;
    //        //V(V&&) = default;
    //    };
    //    auto& x = nttx::detail::erased_component_type::get<V>();
    //    x.copy_construct_at(nullptr, nullptr);
    //    std::cout << x.index;
    //} catch (std::exception const& e) {
    //    std::cerr << e.what();
    //}
}
