#include "BigInteger.hpp"

#include <iostream>

int main()
{

    BigInteger a = 1024;
    BigInteger b = 42;

    // basic arithmetics
    std::cout << "sum = " << a + b << "\n";

    std::cout << "mul = " << a * b << "\n";

    std::cout << "mod = " << a % b << "\n";

    // addition assigment
    BigInteger c = 1024;

    c += 1024;
    std::cout << "\n";
    std::cout << "c = " << c << "\n";

    // postfix decrement
    std::cout << "\n";
    std::cout << "c-- = " << c-- << "\n";
    std::cout << "c = " << c << "\n";

    return 0;
}
