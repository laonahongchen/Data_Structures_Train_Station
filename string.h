#pragma once
#include <iostream>
#include <cstring>

using namespace std;

struct mystring {
	friend ostream & operator << (ostream &os, const mystring &s);
	char *str;
	int len, size;

	void doublespace() {
		char *tmp = str;
		str = new char[size * 2 + 1];
		for (int i = 0; i < len; i++) str[i] = tmp[i];
		delete tmp;
		size *= 2;
		str[size] = '\0';
	}

	mystring(int initsize = 20) {
		len = 0, size = initsize;
		str = new char[size + 1];
		for (int i = 0; i < size; ++i) str[i] = ' ';
		str[size] = '\0';
	}

	mystring(const char *other, int initsize = 20) {
		size = initsize;
		str = new char[size + 1];
		for (int i = 0; i < strlen(other); ++i) str[i] = other[i];
		for (int i = strlen(other); i < size; ++i) str[i] = ' ';
		len = strlen(other);
		str[size] = '\0';
	}

	mystring(const mystring &other, int initsize = 20) {
		size = initsize;
		str = new char[size + 1];
		for (int i = 0; i < other.len; ++i) str[i] = other.str[i];
		for (int i = other.len; i < size; ++i) str[i] = ' ';
		len = other.len;
		str[size] = '\0';
	}

	bool operator == (const char *tmp) {
		if (len != strlen(tmp)) return false;
		else {
			for (int i = 0; i < len; ++i)
				if (str[i] != tmp[i]) return false;
		}
		return true;
	}

	bool operator == (const mystring &tmp) {
		if (size != tmp.size) return false;
		else {
			for (int i = 0; i < size; ++i)
				if (str[i] != tmp.str[i]) return false;
		}
		return true;
	}

	char & operator [] (const int pos) { return str[pos]; }

	mystring operator += (char st) {
		if (len == size) doublespace();
		str[len++] = st;
		return *this;
	}
};

ostream & operator << (ostream &os, const mystring &s) {
	for (int i = 0; i < s.size; ++i) os << s.str[i];
	return os;
}

mystring IntToString(int cnt) {
	int c = 1, tmp = cnt;
	while (tmp / 10 != 0) {
		tmp /= 10;
		c++;
	}
	mystring str;
	while (c--) {
		tmp = pow(10, c);
		str += cnt / tmp + '0';
		cnt = cnt % tmp;
	}
	return str;
}