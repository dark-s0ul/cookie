#pragma once

unsigned strlen(const char *);
void strncpy(char *, const char *, unsigned);
void strcpy(char *, const char *source);

class string {
private:
	unsigned length;
	char *buffer;

public:
	string();
	string(const char *);

	~string();

	string& operator=(const string &);

	friend string operator+(const string &, const string &);
	friend string operator+(const string &, char);
	friend string operator+(char c, const string &);

	inline void operator+=(const string &);
	inline void operator+=(const char);

	char& operator[] (int);

	unsigned size();
	bool empty();
	char *c_str();
};
