#include "my_unique.h"
#include <iostream>
int main() {
    myunique_ptr<int> p1(new int(10));
    std::cout << *p1 << std::endl;

    myunique_ptr<int> p2 = std::move(p1);
    std::cout << *p2 << std::endl;
}

