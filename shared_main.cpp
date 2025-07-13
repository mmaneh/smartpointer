#include "my_shared.h"
#include <iostream>
#include <memory>

int main() {
    shared<int> ptr1(new int(5));
    std::cout << *ptr1 << std::endl;
    std::cout << "count" << ptr1.use_count() << std::endl;
    auto ptr2 = ptr1;
    std::cout << "count" << ptr1.use_count() << std::endl;
    if(ptr1 == ptr2) {
        std::cout << "Equal" << std::endl;
    }else {
        std::cout << "Not equal" << std::endl;
    }
    shared<int> ptr3(new int(10));
    if (ptr1 == ptr3) {
        std::cout << "Equal" << std::endl;
    }
    else {
        std::cout << "Not equal" << std::endl;
    }
}

