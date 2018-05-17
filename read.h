#pragma once
#include <iostream>
#include <fstream>
#include <cstring>
#include "string.h"

using namespace std;

enum USER_TYPE { REGISTER, LOGIN, QUERY_PROFILE, MODIFY_PROFILE, MODIFY_PRIVILEGE, INVALID_USERTYPE, EXIT, CLEAN };

USER_TYPE readUSER_TYPE(char* &it) {
	mystring<50> token;
	for (; *it != '\0' && *it == ' ' && *it != '\n'; ++it);
	for (; *it != '\0' && *it != ' ' && *it != '\n'; token += *it, ++it);
	if (token == "register") return REGISTER;
	else if (token == "login") return LOGIN;
	else if (token == "query_profile") return QUERY_PROFILE;
	else if (token == "modify_profile") return MODIFY_PROFILE;
	else if (token == "modify_privilege") return MODIFY_PRIVILEGE;
	else if (token == "exit") return EXIT;
	else if (token == "clean") return CLEAN;
	else return INVALID_USERTYPE;
}

mystring<20> readUSER_ID(char* &it) {
	mystring<20> token;
	for (; *it != '\0' && *it == ' ' && *it != '\n'; ++it);
	for (; *it != '\0' && *it != ' ' && *it != '\n'; token += *it, ++it);
	return token;
}

int readUSER_INTID(char* &it) {
	int sum = 0;
	for (; *it != '\0' && *it == ' ' && *it != '\n'; ++it);
	for (; *it != '\0' && *it != ' ' && *it != '\n'; ++it) sum = sum * 10 + *it - '0';
	return sum;
}

mystring<40> readUSER_NAME(char* &it) {
	mystring<40> token;
	for (; *it != '\0' && *it == ' ' && *it != '\n'; ++it);
	for (; *it != '\0' && *it != ' ' && *it != '\n'; token += *it, ++it);
	return token;
}

mystring<20> readUSER_PASSWORD(char* &it) {
	mystring<20> token;
	for (; *it != '\0' && *it == ' ' && *it != '\n'; ++it);
	for (; *it != '\0' && *it != ' ' && *it != '\n'; token += *it, ++it);
	return token;
}

mystring<20> readUSER_EMAIL(char* &it) {
	mystring<20> token;
	for (; *it != '\0' && *it == ' ' && *it != '\n'; ++it);
	for (; *it != '\0' && *it != ' ' && *it != '\n'; token += *it, ++it);
	return token;
}

mystring<20> readUSER_PHONE(char* &it) {
	mystring<20> token;
	for (; *it != '\0' && *it == ' ' && *it != '\n'; ++it);
	for (; *it != '\0' && *it != ' ' && *it != '\n'; token += *it, ++it);
	return token;
}

int readUSER_PRIVILEGE(char* &it) {
	int sum = 0;
	for (; *it != '\0' && *it == ' ' && *it != '\n'; ++it);
	for (; *it != '\0' && *it != ' ' && *it != '\n'; ++it) sum = sum * 10 + *it - '0';
	return sum;
}

