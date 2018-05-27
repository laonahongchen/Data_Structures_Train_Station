#pragma once
#include <iostream>
#include <fstream>
#include <cstring>
#include "string.h"

using namespace std;

enum COMMAND_TYPE { REGISTER, LOGIN, QUERY_PROFILE, MODIFY_PROFILE, MODIFY_PRIVILEGE,
	QUERY_TICKET, QUERY_TRANSFER, BUY_TICKET, QUERY_ORDER, REFUND_TICKET,
	ADD_TRAIN, SALE_TRAIN, QUERY_TRAIN, DELETE_TRAIN, MODIFY_TRAIN,
	EXIT, CLEAN, INVALID_COMMANDTYPE };

COMMAND_TYPE readCOMMAND_TYPE(char* &it) {
	mystring<50> token;
	for (; *it != '\0' && *it == ' ' && *it != '\n'; ++it);
	for (; *it != '\0' && *it != ' ' && *it != '\n'; token += *it, ++it);
	if (token == "register") return REGISTER;
	else if (token == "login") return LOGIN;
	else if (token == "query_profile") return QUERY_PROFILE;
	else if (token == "modify_profile") return MODIFY_PROFILE;
	else if (token == "modify_privilege") return MODIFY_PRIVILEGE;
	else if (token == "query_ticket") return QUERY_TICKET;
	else if (token == "query_transfer") return QUERY_TRANSFER;
	else if (token == "buy_ticket") return BUY_TICKET;
	else if (token == "query_order") return QUERY_ORDER;
	else if (token == "refund_ticket") return REFUND_TICKET;
	else if (token == "add_train") return ADD_TRAIN;
	else if (token == "sale_train") return SALE_TRAIN;
	else if (token == "query_train") return QUERY_TRAIN;
	else if (token == "delete_train") return DELETE_TRAIN;
	else if (token == "modify_train") return MODIFY_TRAIN;
	else if (token == "exit") return EXIT;
	else if (token == "clean") return CLEAN;
	else return INVALID_COMMANDTYPE;
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

int readNUM(char* &it) {
	int sum = 0;
	for (; *it != '\0' && *it == ' ' && *it != '\n'; ++it);
	for (; *it != '\0' && *it != ' ' && *it != '\n'; ++it) sum = sum * 10 + *it - '0';
	return sum;
}

mystring<20> readTRAIN_ID(char* &it) {
	mystring<20> token;
	for (; *it != '\0' && *it == ' ' && *it != '\n'; ++it);
	for (; *it != '\0' && *it != ' ' && *it != '\n'; token += *it, ++it);
	return token;
}

mystring<20> readLOC(char* &it) {
	mystring<20> token;
	for (; *it != '\0' && *it == ' ' && *it != '\n'; ++it);
	for (; *it != '\0' && *it != ' ' && *it != '\n'; token += *it, ++it);
	return token;
}

mystring<10> readDATE(char* &it) {
	mystring<10> token;
	for (; *it != '\0' && *it == ' ' && *it != '\n'; ++it);
	for (; *it != '\0' && *it != ' ' && *it != '\n'; token += *it, ++it);
	return token;
}

mystring<20> readTICKET_KIND(char* &it) {
	mystring<20> token;
	for (; *it != '\0' && *it == ' ' && *it != '\n'; ++it);
	for (; *it != '\0' && *it != ' ' && *it != '\n'; token += *it, ++it);
	return token;
}

mystring<10> readCATALOG(char* &it) {
	mystring<10> token;
	for (; *it != '\0' && *it == ' ' && *it != '\n'; ++it);
	for (; *it != '\0' && *it != ' ' && *it != '\n'; token += *it, ++it);
	return token;
}

mystring<40> readTRAIN_NAME(char* &it) {
	mystring<40> token;
	for (; *it != '\0' && *it == ' ' && *it != '\n'; ++it);
	for (; *it != '\0' && *it != ' ' && *it != '\n'; token += *it, ++it);
	return token;
}

mystring<5> readTIME(char *&it) {
	mystring<5> token;
	for (; *it != '\0' && *it == ' '; ++it);
	for (; *it != '\0' && *it != ' '; token += *it, ++it);
	return token;
}

double readPRICE(char *&it) {
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

int readINTDATE(char *&it) {
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