#include <iostream>
#include <string>
#include "func_ptr.h"
#include "bf.h"

//int mul(int a, int b) { return a * b; }
auto mul = func_ptr<int(int, int)>({
	0x90, 0x01, 0x00, 0xE0,
	0x1E, 0xFF, 0x2F, 0xE1
});

int main(){
	std::cout « mul(23, 3);
	
	bf::eval("+++++++[>+++++++<-]>."); //print 1
}
