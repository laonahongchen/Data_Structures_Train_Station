#include <iostream>
#include <fstream>
#include <cstring>
#include "string.h"
#include "read_user_command.h"
#include "user.h"
#include "file_operations.h"

#define maxlen 1000

using namespace std;

void init() {
	fout.open("UserShelf.txt", ios::binary | ios::in | ios::out);
	fout.seekg(0, ios::end);
	user_number = fout.tellg() / user_block;
	fout.seekg(0, ios::beg);
	return;
}

int main() {
	init();
	fstream fin("test.txt", fstream::in);
	while (1) {
		char command[maxlen], *it = command;
		fin.getline(command, maxlen);
		if (fin.eof()) break;
		int usertype = readUSER_TYPE(it);
		if (usertype == REGISTER) {
			mystring name(readUSER_NAME(it), 40);
			mystring password(readUSER_PASSWORD(it), 20);
			mystring email(readUSER_EMAIL(it), 20);
			mystring phone(readUSER_PHONE(it), 20);
			User user(name, password, email, phone);
			file_register(user);
		}
		else if (usertype == LOGIN) {
			int intid = readUSER_INTID(it);
			mystring password(readUSER_PASSWORD(it), 20);
			file_login(intid, password);
		}
		else if (usertype == QUERY_PROFILE) {
			int intid = readUSER_INTID(it);
			file_query_profile(intid);
		}
		else if (usertype == MODIFY_PROFILE) {
			int intid = readUSER_INTID(it);
			mystring name(readUSER_NAME(it), 40);
			mystring password(readUSER_PASSWORD(it), 20);
			mystring email(readUSER_EMAIL(it), 20);
			mystring phone(readUSER_PHONE(it), 20);
			file_modify_profile(intid, name, password, email, phone);
		}
		else if (usertype == MODIFY_PRIVILEGE) {
			int id1 = readUSER_INTID(it);
			int id2 = readUSER_INTID(it);
			int privilege = readUSER_PRIVILEGE(it);
			file_modify_priviledge(id1, id2, privilege);
		}
		else if (usertype == EXIT) {
			exit(0);
		}
		else continue;
	}
	system("pause");
	return 0;
}