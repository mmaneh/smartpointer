#include <memory>
#include <iostream>

template <typename T>

class shared {
    
    T* ptr;
    int* count;

public:

    shared() : ptr{nullptr}, count(new int(0)) {}
    explicit shared(T* pt) : ptr{pt}, count(new int(1)) {}
    shared(const shared& other) : ptr{other.ptr}, count{other.count} {
        if(count) ++(*count);
    }
    shared(shared&& other) noexcept : ptr{other.ptr}, count{other.count} {
        other.ptr = nullptr;
        other.count = nullptr;
    }
 
    template <typename U>
    shared(std::unique_ptr<U> &&other) noexcept {
        ptr = other.release();
        count = new int(1);
    }
    shared& operator=(const shared& other) {
        if(this != &other) {
            release();
            ptr = other.ptr;
            count = other.count;
            ++(*count);
        }
        return *this;
    }

    shared& operator=(shared&& other)  noexcept{
        if(this != &other) {
            release();
            ptr = other.ptr;
            count = other.count;
            other.ptr = nullptr;
            other.count = nullptr;
        }
        return *this;
    }
    T* operator->() {
        return ptr;
    }
    T& operator*() {
        return *ptr;
    }
    T* get() const{
        return ptr;
    }
    void release() {
        if(ptr) {
            --(*count);
            if(*count == 0) {
		    std::cout << "deleted" << std::endl;
		delete ptr;
                delete count;
        
            }
        }
        ptr = nullptr;
        count = nullptr;
    }
    int use_count() const {
        return count ? *count : 0;
    }
    operator bool() const{
        return ptr != nullptr;
    }
    void reset(T* new_pt) {
        release();
        ptr = new_pt;
        count = new int(1);
    }
    ~shared() {
        std::cout << "dtor" << std::endl;
        release();
    }
    bool operator==(const shared& other) const {
        return ptr == other.ptr;
    }
    
};

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
