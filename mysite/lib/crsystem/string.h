#ifndef _string_h
#define _string_h

#include <iostream>
#include <cstring>
#include <cmath>

using namespace std;

template <int _size>
struct mystring
{
	friend ostream &operator<<(ostream &os, const mystring &s)
	{
		for (int i = 0; i < s.len; ++i)
			os << s.str[i];
		return os;
	}

	char str[_size];
	int len, size;

	string value()
	{
		string t;
		for (int i = 0; i < len; ++i)
			t += str[i];
		return t;
	}

	mystring()
	{
		len = 0;
		size = _size;
		for (int i = 0; i < size; ++i)
			str[i] = ' ';
		str[0] = '\0';
	}

	mystring(const string &rhs)
	{
		len = rhs.length();
		size = _size;
		for (int i = 0; i < len; ++i)
			str[i] = rhs[i];
		str[len] = '\0';
	}

	mystring(const char *other)
	{
		len = strlen(other);
		size = _size;
		for (int i = 0; i < len; ++i)
			str[i] = other[i];
		//for (int i = strlen(other); i < size; ++i) str[i] = ' ';
		str[len] = '\0';
	}

	mystring(const mystring &other)
	{
		size = _size;
		len = other.len;
		for (int i = 0; i < other.len; ++i)
			str[i] = other.str[i];
		//for (int i = other.len; i < size; ++i) str[i] = ' ';
		str[len] = '\0';
	}

	bool operator==(const char *tmp)
	{
		if (len != strlen(tmp))
			return false;
		else
		{
			for (int i = 0; i < len; ++i)
				if (str[i] != tmp[i])
					return false;
		}
		return true;
	}

	bool operator==(const mystring &tmp)
	{
		if (len != tmp.len)
			return false;
		else
		{
			for (int i = 0; i < len; ++i)
				if (str[i] != tmp.str[i])
					return false;
		}
		return true;
	}

	char &operator[](const int pos) { return str[pos]; }

	mystring operator+=(char st)
	{
		str[len++] = st;
		str[len] = '\0';
		return *this;
	}
};

/*template<int _size>
ostream & operator << (ostream &os, const mystring<_size> &s) {
	for (int i = 0; i < s.len; ++i) os << s.str[i];
	return os;
}*/

template <int _size>
mystring<_size> IntToString(int cnt)
{
	int c = 1, tmp = cnt;
	while (tmp / 10 != 0)
	{
		tmp /= 10;
		c++;
	}
	mystring<_size> str;
	while (c--)
	{
		tmp = pow(10, c);
		str += cnt / tmp + '0';
		cnt = cnt % tmp;
	}

	return str;
}

#endif