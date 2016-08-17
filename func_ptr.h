#include <sys/mman.h>
#include <vector>

template<typename T>
class func_ptr;

template<typename T, typename ...Args>
class func_ptr{
public:
	func_ptr(std::vector<char> code) : size(code.size()){
		ptr = mmap(0, size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		memcpy(ptr, &code[0], size);
	}
	inline ~func_ptr(){ munmap(ptr, size); }
	
	inline T operator (Args... args){
		return ((T(*)(Args ...))ptr)(args ...);
	}
	
private:
	size_t size;
	void *ptr;
};
