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

template<typename T>
class weak{
	controlBlock<T>* ptr = nullptr;
	
    public: 
	weak(){}
	
	weak(const shared<T>& other) : ptr{other.ptr} {
		if (ptr) ++(ptr->weak_count);
	}
	weak(const weak& other) : ptr{other.ptr} {
		if (ptr) ++(ptr->weak_count);
	}
	weak& operator=(const weak& other) {
		if (this != &other) {
			ptr = other.ptr;
			if(ptr) ++(ptr->weak_count);	
		}
		return *this;
	}
	~weak() {
		reset;
	}

	bool expired() const {
        	return !ptr || ptr->shared_count == 0;
    	}
	int use_count() {
		return ptr ? ptr->shared_count : 0;
	}
	void reset() {
        	if (ptr) {
            		--(ptr->weak_count);
            	if (ptr->shared_count == 0 && ptr->weak_count == 0) {
                	delete ptr;
            	}
            	ptr = nullptr;
        	}
    	}

	shared<T> lock() {
		if(expired()) {	
			return shared<T>();
		}else {
			return shared<T>(ptr);
		}
	}
	
};

