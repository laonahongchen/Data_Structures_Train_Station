#include <iostream>
#include <fstream>
#include <cstring>
#include "string.h"
#include "read.h"
#include "file.h"
#include "user.h"

#define maxlen 300

using namespace std;

void init() {
	fout.open("UserShelf", ios::binary | ios::in | ios::out);
	if (!fout) {
		fout.open("UserShelf", ios::binary | ios::out);
		fout.close();
		fout.open("UserShelf", ios::binary | ios::in | ios::out);
	}
	fout.seekg(0, ios::end);
	user_number = fout.tellg() / user_block;
	fout.seekg(0, ios::beg);
	return;
}

int main() {
	init();
	while (1) {
		char command[maxlen], *it = command;
		cin.getline(command, maxlen);
		//if (cin.eof()) break;
		int usertype = readUSER_TYPE(it);
		if (usertype == REGISTER) {
			mystring<40> name(readUSER_NAME(it));
			mystring<20> password(readUSER_PASSWORD(it));
			mystring<20> email(readUSER_EMAIL(it));
			mystring<20> phone(readUSER_PHONE(it));
			User user(name, password, email, phone);
			file_register(user);
		}
		else if (usertype == LOGIN) {
			int intid = readUSER_INTID(it);
			mystring<20> password(readUSER_PASSWORD(it));
			file_login(intid, password);
		}
		else if (usertype == QUERY_PROFILE) {
			int intid = readUSER_INTID(it);
			file_query_profile(intid);
		}
		else if (usertype == MODIFY_PROFILE) {
			int intid = readUSER_INTID(it);
			mystring<40> name(readUSER_NAME(it));
			mystring<20> password(readUSER_PASSWORD(it));
			mystring<20> email(readUSER_EMAIL(it));
			mystring<20> phone(readUSER_PHONE(it));
			file_modify_profile(intid, name, password, email, phone);
		}
		else if (usertype == MODIFY_PRIVILEGE) {
			int id1 = readUSER_INTID(it);
			int id2 = readUSER_INTID(it);
			int privilege = readUSER_PRIVILEGE(it);
			file_modify_priviledge(id1, id2, privilege);
		}
		else if (usertype == EXIT) {
			fout.close();
			cout << "BYE" << '\n';
			exit(0);
		}
		else if (usertype == CLEAN) {
			fout.close();
			fout.open("UserShelf", ios::binary | ios::out);
			fout.close();
			cout << 1 << '\n';
		}
		else continue;
	}
	return 0;
}