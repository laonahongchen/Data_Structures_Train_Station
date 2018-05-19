#ifndef Mstring
#define Mstring





#include <iostream>
#include <cstring>
#include "model_predefined.h"

using namespace std;

template <int _size>
struct mystring {
	
	char str[_size];
	int len, size;

	mystring() {
		len = 0;
		size = _size;
		for (int i = 0; i < size; ++i) str[i] = ' ';
		str[size] = '\0';
	}

	mystring(const char *other) {
		size = _size;
		for (int i = 0; i < strlen(other); ++i) str[i] = other[i];
		for (int i = strlen(other); i < size; ++i) str[i] = ' ';
		len = strlen(other);
		str[size] = '\0';
		len = strlen(other);
	}

	mystring(const mystring<_size> &other) {
		size = _size;
		for (int i = 0; i < other.len; ++i) str[i] = other.str[i];
		for (int i = other.len; i < size; ++i) str[i] = ' ';
		str[size] = '\0';
		len = other.len;
	}

	bool operator == (const char *tmp) {
		if (len != strlen(tmp)) return false;
		else {
			for (int i = 0; i < len; ++i)
				if (str[i] != tmp[i]) return false;
		}
		return true;
	}

	bool operator == (const mystring<_size> &tmp) {
		if (len != tmp.len) return false;
		else {
			for (int i = 0; i < len; ++i)
				if (str[i] != tmp.str[i]) return false;
		}
		return true;
	}

	char & operator [] (const int pos) { return str[pos]; }

	mystring<_size> &operator += (char st) {
		str[len++] = st;
		return *this;
	}

	mystring<_size> & operator =(const mystring<_size> &s) {
		for (int i = 0; i < s.len; ++i)
			str[i] = s.str[i];
		for (int i = s.len; i < size; ++i) 
			str[i] = ' ';
		str[size] = '\0';
		len = s.len;
		return *this;
	}

	operator char*() const {
		return str;
	}

	operator key_t() const {
		return key_t(str);
	}



};


template<int _size>
ostream & operator << (ostream &os, const mystring<_size> &s) {
	for (int i = 0; i < s.len; ++i) os << s.str[i];
	return os;
}

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

#endif // !Mstring
