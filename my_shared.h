#ifndef MY_SHARED_H
#define MY_SHARED_H
#include <iostream>
#include <memory>
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
    explicit shared(controlBlock<T>* pt) : ptr(pt->ptr), p(pt) {
        if (p) ++(p->shared_count);
    }
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
    const T* operator->() const {
    	return ptr;
    }
    const T& operator*() const {
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
                delete p->ptr;
		p->ptr = nullptr;
		if(p->weak_count == 0) delete p;
        
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
#endif

