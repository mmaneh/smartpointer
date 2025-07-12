#include <memory>
#include <iostream>
template <typename T>
class controlBlock {

public:
       T* ptr;
       int shared_count;
       int weak_count;

       controlBlock(T* p) : ptr{p}, shared_count{1}, weak_count{0} {}
       ~controlBlock() {
                delete ptr;
       }

};

template <typename T>

class shared {
    
    T* ptr;
    controlBlock<T>* p;

public:

    shared() : ptr{nullptr}, p{nullptr} {}
    explicit shared(T* pt) : ptr{pt}, p(new controlBlock<T>(pt)) {}
    shared(const shared& other) : ptr{other.ptr}, p{other.p} {
        if(p) ++(p->shared_count);
    }
    shared(shared&& other) noexcept : ptr{other.ptr}, p{other.p} {
        other.ptr = nullptr;
        other.p = nullptr;
    }
 
    template <typename U>
    shared(std::unique_ptr<U> &&other) noexcept {
        ptr = other.release();
        p = new controlBlock<T>(other);
    }
    shared& operator=(const shared& other) {
        if(this != &other) {
            release();
            ptr = other.ptr;
            p = other.p;
            ++(p->shared_count);
        }
        return *this;
    }

    shared& operator=(shared&& other)  noexcept{
        if(this != &other) {
            release();
            ptr = other.ptr;
            p = other.p;
            other.ptr = nullptr;
            other.p = nullptr;
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
        if(p) {
            --(p->shared_count);
            if(p->shared_count == 0) {
		    std::cout << "deleted" << std::endl;
                delete p;
        
            }
        }
        ptr = nullptr;
        p  = nullptr;
    }
    int use_count() const {
        return p ? p->shared_count : 0;
    }
    operator bool() const{
        return ptr != nullptr;
    }
    void reset(T* new_pt) {
        release();
        ptr = new_pt;
        p = new controlBlock<T>(new_pt);
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
