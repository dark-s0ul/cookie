## Cookies!
Light and "useful" codes for c++

## Brainfuck interpreter
<details> 
<summary>Example:</summary>
'''c++
#include "bf.h"

bf::eval("+++++++[>+++++++<-]>."); //print 1
'''
</details>

## Dynamic function
<details> 
<summary>Example:</summary>
```c++
#include "func_ptr.h"

//int mul(int a, int b) { return a * b; }
auto mul = func_ptr<int(int, int)>({
	//Put your asm code here
	0x90, 0x01, 0x00, 0xE0,
	0x1E, 0xFF, 0x2F, 0xE1
});
	
int res = mul(23, 3); //69
```	
</details>

## Custom operator
<details> 
<summary>Example:</summary>
```c++
#include "make_operator.h"

//Using struct
struct _in {
	template <typename T>
	inline bool operator ()(T value, std::vector<T> const& vector) const {
		return std::find(vector.begin(), vector.end(), value) != vector.end();
	}
};
auto in = make_operator(_in());

//Using lambda
auto in = make_operator([](auto value, auto vector) -> bool {
	return std::find(vector.begin(), vector.end(), value) != vector.end();
});

std::vector<int> v = { 10 };

bool res = 10 <in> v; //true
res = 11 <in> v; //false
```
</details>
