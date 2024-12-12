#include <iostream>

#include "any.hpp"

namespace {

struct A {
    int v = 123;

    A() = default;
    explicit A(int a): v{a} {}        
};

std::ostream& operator<<(std::ostream& os, const A& a) {
    return os << a.v;
}

} // namespace


int main() {
    using internal::any;

    any a;

    a = 5;
    int a_val = internal::any_cast<int>(a);

    a = 3.144;
    double a_val2 = internal::any_cast<double>(a);

    a = A{};
    A a_val3 = internal::any_cast<A>(a);

    std::cout << "value of a (int) = " << a_val << '\n';
    std::cout << "value of a (double) = " << a_val2 << '\n';
    std::cout << "value of a (A) = " << a_val3 << '\n';

    return 0;
}
