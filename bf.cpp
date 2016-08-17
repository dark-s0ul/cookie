#include <stdio.h>
#include <string>
#include <vector>

std::vector<char> mem(30000);
auto p = mem.begin();

void bf_eval(std::string str){
	auto s = str.begin();
	while(s != str.end()){
		if(*s == '>' && p != mem.end()) p++;
		if(*s == '<' && p != mem.begin()) p--;
		if(*s == '+') ++*p;
		if(*s == '-') --*p;
		if(*s == '.') putc(*p, stdout);
		if(*s == ',') *p = getc(stdin);
		if(*s == '['){
			int br = 0;
			while(!*p){
				if(*s == '[') br++;
				if(*s == ']' && br-- == 1) break;
				s++;
			}
		}
		if(*s == ']'){
			int br = 0;
			while(*p){
				if(*s == ']') br++;
				if(*s == '[' && br-- == 1) break;
				s++;
			}
		}
		s++;
	}
}
