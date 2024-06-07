#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>

int main() {
    using namespace boost::multiprecision;

    cpp_int a("12345678901234567890");
    cpp_int b("98765432109876543210");
    cpp_int result = a + b;

    std::cout << "Result: " << result << std::endl;

    return 0;
}