#include <string>
#include <vector>
#include <conio.h>

namespace bf {
	std::vector<char> mem(30000);
	auto p = mem.begin();

	void eval(std::string str) {
		auto s = str.begin();
		while (s != str.end()) {
			if (*s == '>' && p != mem.end()) p++;
			else if (*s == '<' && p != mem.begin()) p--;
			else if (*s == '+')++*p;
			else if (*s == '-')--*p;
			else if (*s == '.') _putch(*p);
			else if (*s == ',') _putch(*p = _getch());
			else if (*s == '[') {
				int br = 0;
				while (!*p) {
					if (*s == '[') br++;
					if (*s == ']' && br-- == 1) break;
					s++;
				}
			} else if (*s == ']') {
				int br = 0;
				while (*p) {
					if (*s == ']') br++;
					if (*s == '[' && br-- == 1) break;
					s--;
				}
			}
			s++;
		}
	}
}
