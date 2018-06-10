#pragma once
#include <cstring>
#define TREE_ORDER 250
#define _CRT_SECURE_NO_WARNINGS

struct key_type {
	char k[102];

	key_type(const char *str = "")
	{
		memset(k, 0, sizeof(k));
		strcpy(k, str);
	}
};

inline int keycmp(const key_type &a, const key_type &b) {
	return strcmp(a.k, b.k);
}

template <class type>
bool operator< (const key_type &l, const type &r) {
	return keycmp(l, r.key) < 0;
}

template <class type>
bool operator< (const type &l, const key_type &r) {
	return keycmp(l.key, r) < 0;
}

template <class type>
bool operator== (const key_type &l, const type &r) {
	return keycmp(l, r.key) == 0;
}

template <class type>
bool operator== (const type &l, const key_type &r) {
	return keycmp(l.key, r) == 0;
}
