#pragma once
#include <iostream>
#include <fstream>
#include <cstring>
#include "string.h"

using namespace std;

enum USER_TYPE { REGISTER, LOGIN, QUERY_PROFILE, MODIFY_PROFILE, MODIFY_PRIVILEGE, INVALID_USERTYPE, EXIT };

USER_TYPE readUSER_TYPE(char* &it) {
	mystring token;
	for (; *it != '\0' && *it == ' '; ++it);
	for (; *it != '\0' && *it != ' '; token += *it, ++it);
	if (token == "register") return REGISTER;
	if (token == "login") return LOGIN;
	if (token == "query_profile") return QUERY_PROFILE;
	if (token == "modify_profile") return MODIFY_PROFILE;
	if (token == "modify_privilege") return MODIFY_PRIVILEGE;
	if (token == "exit") return EXIT;
	return INVALID_USERTYPE;
}

mystring readUSER_ID(char* &it) {
	mystring token;
	for (; *it != '\0' && *it == ' '; ++it);
	for (; *it != '\0' && *it != ' '; token += *it, ++it);
	return token;
}

int readUSER_INTID(char* &it) {
	int sum = 0;
	for (; *it != '\0' && *it == ' '; ++it);
	for (; *it != '\0' && *it != ' '; ++it) sum = sum * 10 + *it - '0';
	return sum;
}

mystring readUSER_NAME(char* &it) {
	mystring token;
	for (; *it != '\0' && *it == ' '; ++it);
	for (; *it != '\0' && *it != ' '; token += *it, ++it);
	return token;
}

mystring readUSER_PASSWORD(char* &it) {
	mystring token;
	for (; *it != '\0' && *it == ' '; ++it);
	for (; *it != '\0' && *it != ' '; token += *it, ++it);
	return token;
}

mystring readUSER_EMAIL(char* &it) {
	mystring token;
	for (; *it != '\0' && *it == ' '; ++it);
	for (; *it != '\0' && *it != ' '; token += *it, ++it);
	return token;
}

mystring readUSER_PHONE(char* &it) {
	mystring token;
	for (; *it != '\0' && *it == ' '; ++it);
	for (; *it != '\0' && *it != ' '; token += *it, ++it);
	return token;
}

int readUSER_PRIVILEGE(char* &it) {
	int sum = 0;
	for (; *it != '\0' && *it == ' '; ++it);
	for (; *it != '\0' && *it != ' '; ++it) sum = sum * 10 + *it - '0';
	return sum;
}

