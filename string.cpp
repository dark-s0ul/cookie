#include "string.h"

unsigned strlen(const char *str) {
	unsigned index;
	for (index = 0; str[index] != '\0'; index++);
	return index;
}

void strncpy(char *buffer, const char *source, unsigned len) {
	for (unsigned index = 0; index < len; index++) {
		buffer[index] = source[index];
	}
}

void strcpy(char *buffer, const char *source) {
	strncpy(buffer, source, strlen(source));
}

string::string() : length(0), buffer(0) {}

string::string(const char *data) : length(strlen(data)), buffer(new char[length]) {
	strncpy(buffer, data, length);
}

string::~string() {
	delete[] buffer;
	length = 0;
}

string& string::operator=(const string &str) {
	if (this != &str) {
		delete[] buffer;
		length = str.length;
		buffer = new char[length];
		strncpy(buffer, str.buffer, length);
	}
	return *this;
}

string operator+(const string& str1, const string& str2) {
	string str;
	str.length = str1.length + str2.length;
	str.buffer = new char[str.length];
	strncpy(str.buffer, str1.buffer, str1.length);
	strncpy(str.buffer + str1.length, str2.buffer, str2.length);
	return str;
}

string operator+(const string& str1, char c) {
	string str;
	str.length = str1.length + 1;
	str.buffer = new char[str.length];
	str.buffer[str1.length] = c;
	strncpy(str.buffer, str1.buffer, str1.length);
	return str;
}

string operator+(char c, const string& str1) {
	string str;
	str.length = str1.length + 1;
	str.buffer = new char[str.length];
	str.buffer[0] = c;
	strncpy(str.buffer + 1, str1.buffer, str1.length);
	return str;
}

void string::operator+=(const string &str) {
	*this = operator+(*this, str);
}

void string::operator+=(const char c) {
	*this = operator+(*this, c);
}

char& string::operator[] (int index) {
	return buffer[index];
}

unsigned string::size() { return length; }
bool string::empty() { return length == 0; }
char *string::c_str() { return buffer; }
