#pragma once
#include <iostream>
#include <cstring>
#include <cmath>
#include "predifined.h"

using namespace std;

template <int _size>
struct mystring {
	friend ostream & operator << (ostream &os, const mystring &s) {
		for (int i = 0; i < s.len; ++i) os << s.str[i];
		return os;
	}
	char str[_size + 1];
	int len, size;

	mystring() {
		len = 0;
		size = _size;
		for (int i = 0; i < size; ++i) str[i] = ' ';
		str[0] = '\0';
	}

	mystring(const char *other) {
		len = strlen(other);
		size = _size;
		for (int i = 0; i < strlen(other); ++i) str[i] = other[i];
		len = strlen(other);
		str[len] = '\0';
	}

	mystring(const mystring &other) {
		size = _size;
		len = other.len;
		for (int i = 0; i < other.len; ++i) str[i] = other.str[i];
		str[len] = '\0';
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
		if (len != tmp.len) return false;
		else {
			for (int i = 0; i < len; ++i)
				if (str[i] != tmp.str[i]) return false;
		}
		return true;
	}

	char & operator [] (const int pos) { return str[pos]; }

	mystring operator += (char st) {
		str[len++] = st;
		str[len] = '\0';
		return *this;
	}

	mystring<_size> & operator =(const mystring<_size> &s) {
		for (int i = 0; i < s.len; ++i)
			str[i] = s.str[i];
		for (int i = s.len; i < size; ++i)
			str[i] = ' ';
		str[_size] = '\0';
		len = s.len;
		return *this;
	}

	bool operator <(const mystring<_size> &s) {
		return strcmp(str, s.str) < 0;
	}

	operator const char*() const {
		return str;
	}

	operator key_t() const {
		return key_t(str);
	}
};

template<int _size>
mystring<_size> IntToString(int cnt) {
	int c = 1, tmp = cnt;
	while (tmp / 10 != 0) {
		tmp /= 10;
		c++;
	}
	mystring<_size> str;
	while (c--) {
		tmp = pow(10, c);
		str += cnt / tmp + '0';
		cnt = cnt % tmp;
	}
	return str;

}

template<int _size>
int readdate(const mystring<_size> &s) {
	int sum = 0, cnt = 0;
	for (int i = 0; i < s.len; ++i) {
		if (s.str[i] == '-') {
			cnt++;
			continue;
		}
		if (cnt == 2) {
			sum = sum * 10 + s.str[i] - '0';
		}
	}
	return sum;
}

template<int _size>
int readuser_id(const mystring<_size> &s) {
	int sum = 0;
	for (int i = 0; i < s.len; ++i) {
		sum = sum * 10 + s.str[i] - '0';
	}
	return sum;
}

template<int size1, int size2>
mystring<size1 + size2> operator +(const mystring<size1> &str1, const mystring<size2> &str2) {
	mystring<size1 + size2> s;
	for (int i = 0; i < str1.len; ++i)	s.str[i] = str1.str[i];
	for (int i = size1; i < size1 + str2.len; ++i)	s.str[i] = str2.str[i - size1];
	s.str[size1 + size2] = '\0';
	s.size = size1 + size2;
	s.len = s.size;
	return s;
}