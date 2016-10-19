#pragma once
#include <iostream>

class string {
private:
	unsigned length;
	char *buffer;
	
public:
	string() : length(0), buffer(0) {}

	string(const char *data) : length(strlen(data)), buffer(new char[length]) {
		strncpy(buffer, data, length);
	}
	
	~string() {
		delete [] buffer;
		length = 0;
	}
	
	string& operator=(const string &str) {
		if (this != &str) {
			delete [] buffer;
			length = str.length;
			buffer = new char[length];
			strncpy(buffer, str.buffer, length);
		}
		return *this;
	}
	
	friend string operator+(const string& str1, const string& str2) {
		string str;
		str.length = str1.length + str2.length;
		str.buffer = new char[str.length];
		strncpy(str.buffer, str1.buffer, str1.length);
		strncpy(str.buffer + str1.length, str2.buffer, str2.length);
		return str;
	}
	
	inline void operator+=(const string &str) {
		*this = operator+(*this, str);
	}
	
	inline void operator+=(const char c) {
		*this = operator+(*this, c);
	}
	
	friend string operator+(const string& str1, char c) {
		string str;
		str.length = str1.length + 1;
		str.buffer = new char[str.length];
		str.buffer[str1.length] = c;
		strncpy(str.buffer, str1.buffer, str1.length);
		return str;
	}
	
	friend string operator+(char c, const string& str1) {
		string str;
		str.length = str1.length + 1;
		str.buffer = new char[str.length];
		str.buffer[0] = c;
		strncpy(str.buffer + 1, str1.buffer, str1.length);
		return str;
	}
	
	friend std::ostream& operator<< (std::ostream& os, const string& str) {
		for(unsigned i = 0; i < str.length; i++) {
			os.put(str.buffer[i]);
		}
		return os;
	}
	
	char& operator[] (int index) {
		return buffer[index];
	}
	
	unsigned size() { return length; }
	bool empty() { return length == 0; }
};
