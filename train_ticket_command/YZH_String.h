#ifndef Mstring
#define Mstring

#include <iostream>
#include <cstring>
#include "model_predefined.h"

using namespace std;

template <int _size>
struct mystring {
	
	char str[_size + 1];
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

template<int size1, int size2>
mystring<size1 + size2> operator +(const mystring<size1> &str1, const mystring<size2> &str2) {
	mystring<size1 + size2> str;
	for (int i = 0; i < str1.len; ++i)
		str.str[i] = str1.str[i];
	for (int i = size1; i < size1 + str2.len; ++i)
		str.str[i] = str2.str[i - size1];
	str.str[size1 + size2] = '\0';
	str.len = str.size;
	return str;
}

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


enum TRAIN_TYPE {ADD, SALE, QUERY, DELETE, MODIFY, INVALID_USERTYPE, EXIT, BUY, REFUND, QUERYTICKET, QUERYTRAINSFER};

TRAIN_TYPE read_TYPE(char* &it) {

	mystring<30> token;
	for (; *it != '\0' && *it == ' '; ++it);
	for (; *it != '\0' && *it != ' '; token += *it, ++it);

	if (token == "add_train") return ADD;

	if (token == "sale_train") return SALE;

	if (token == "query_train") return QUERY;

	if (token == "delete_train") return DELETE;

	if (token == "modify_train") return MODIFY;

	if (token == "buy_ticket") return BUY;

	if (token == "refund_ticket") return REFUND;

	if (token == "exit") return EXIT;

	if (token == "query_ticket") return QUERYTICKET;

	if (token == "query_transfer") return QUERYTRAINSFER;

	return INVALID_USERTYPE;
}

mystring<20> read_id(char* &it) {

	mystring<20> token;

	for (; *it != '\0' && *it == ' '; ++it);
	for (; *it != '\0' && *it != ' '; token += *it, ++it);

	return token;
}

mystring<40> read_name(char* &it) {

	mystring<40> token;

	for (; *it != '\0' && *it == ' '; ++it);
	for (; *it != '\0' && *it != ' '; token += *it, ++it);

	return token;
}

mystring<10> read_catalog(char* &it) {

	mystring<10> token;

	for (; *it != '\0' && *it == ' '; ++it);
	for (; *it != '\0' && *it != ' '; token += *it, ++it);

	return token;
}

mystring<5> read_time(char *&it) {
	mystring<5> token;

	for (; *it != '\0' && *it == ' '; ++it);
	for (; *it != '\0' && *it != ' '; token += *it, ++it);

	return token;
}

int read_num(char*&it) {
	int sum = 0;
	for (; *it != '\0' && *it == ' '; ++it);
	for (; *it != '\0' && *it != ' '; ++it) 
		sum = sum * 10 + *it - '0';
	return sum;
}

double read_price(char *&it) {
	double sum = 0;
	for (; *it != '\0' && *it == ' '; ++it);
	for (; !(*it >= '0' && *it <= '9'); ++it);
	for (; *it != '\0' && *it != ' ' && *it != '.'; ++it) sum = sum * 10 + *it - '0';
	if (*it == '.') {
		double ad;
		++it;
		int cnt = 1;
		for (; *it != '\0' && *it != ' '; ++it, ++cnt) {
			ad = (*it - '0') / pow(10, cnt);
			sum += ad;
		}
	}
	return sum;
}

int read_date(char *&it) {
	int cnt = 0, date = 0;
	for (; *it != '\0' && *it != '-'; ++it);
	if (*it == '-')
		++it;
	for (; *it != '\0' && *it != '-'; ++it);
	if (*it == '-')
		++it;
	for (; *it != '\0' && *it != ' '; ++it)
		date = date * 10 + *it - '0';
	return date;
}


#endif // !Mstring
