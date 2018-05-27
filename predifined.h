#pragma once
#include <cstring>

#define TREE_ORDER 4

struct key_t {
	char k[102];
	key_t(const char *str = "")
	{
		memset(k, 0, sizeof(k));
		strcpy_s(k, str);
	}
};

inline int keycmp(const key_t &a, const key_t &b) {
	return strcmp(a.k, b.k);
}

template <class type>
bool operator< (const key_t &l, const type &r) {
	return keycmp(l, r.key) < 0;
}

template <class type>
bool operator< (const type &l, const key_t &r) {
	return keycmp(l.key, r) < 0;
}

template <class type>
bool operator== (const key_t &l, const type &r) {
	return keycmp(l, r.key) == 0;
}

template <class type>
bool operator== (const type &l, const key_t &r) {
	return keycmp(l.key, r) == 0;
}
