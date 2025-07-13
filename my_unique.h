#ifndef MY_UNIQUE_H
#define MY_UNIQUE_H

#include <iostream>
template <typename T>
class myunique_ptr {
	T* ptr;
public:
	
	explicit myunique_ptr(T* pt = nullptr) noexcept : ptr{pt} {}
	
	~myunique_ptr() { delete ptr;}
	
	myunique_ptr(const myunique_ptr& otr) = delete;

	myunique_ptr& operator=(const myunique_ptr& otr) = delete;
	
	myunique_ptr& operator=(myunique_ptr&& otr) noexcept {
		if(this != &otr) {
			delete ptr;
			ptr = otr.ptr;
			otr.ptr = nullptr;
		}
		return *this;
	}

	myunique_ptr(myunique_ptr&& otr) noexcept : ptr{otr.ptr} {
		otr.ptr = nullptr;
	}

	T* operator->() {return ptr;}
	T& operator*() {return *ptr;}
	const T* operator->() const {return ptr;}
	const T& operator*() const {return *ptr;}	
	T* get() { return ptr;}
	T* release() {
		T* tmp = ptr;
		ptr = nullptr;
		return tmp;
	}
	
	void  reset(T* pt = nullptr) { 
		delete ptr;
		ptr = pt; 
	}

	explicit operator bool() const noexcept {
        	return ptr != nullptr;
    	}
	
};
#endif
