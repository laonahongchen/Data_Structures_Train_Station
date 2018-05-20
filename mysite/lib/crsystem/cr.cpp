#include <iostream>
#include <fstream>
#include <cstring>
#include "string.h"
#include "read.h"
#include "file.h"
#include "user.h"

#define maxlen 300

using namespace std;

namespace CR
{

void init()
{
	fout.open("UserShelf", ios::binary | ios::in | ios::out);
	if (!fout)
	{
		cout << "InitFile" << endl;
		fout.open("UserShelf", ios::binary | ios::out);
		fout.close();
		fout.open("UserShelf", ios::binary | ios::in | ios::out);
	}
	else
		cout << "FileExisted" << endl;
	fout.seekg(0, ios::end);
	user_number = fout.tellg() / user_block;
	fout.seekg(0, ios::beg);
}

void output(char *info, std::string s)
{
	for (auto i : s)
		*info++ = i;
}

void userRegister(char *it, char *info)
{
	init();
	mystring<40> name(readUSER_NAME(it));
	mystring<20> password(readUSER_PASSWORD(it));
	mystring<20> email(readUSER_EMAIL(it));
	mystring<20> phone(readUSER_PHONE(it));
	cout << "C++ DEBUG: " << name.value() << ' ' << password.value() << ' ' << email.value() << ' ' << phone.value() << endl;
	User user(name, password, email, phone);
	output(info, file_register(user));
}

void userLogin(char *it, char *info)
{
	init();
	int intid = readUSER_INTID(it);
	mystring<20> password(readUSER_PASSWORD(it));
	output(info, file_login(intid, password));
}

void userQueryProfile(char *it, char *info)
{
	init();
	int intid = readUSER_INTID(it);
	output(info, file_query_profile(intid));
}

void userModifyProfile(char *it, char *info)
{
	init();
	int intid = readUSER_INTID(it);
	mystring<40> name(readUSER_NAME(it));
	mystring<20> password(readUSER_PASSWORD(it));
	mystring<20> email(readUSER_EMAIL(it));
	mystring<20> phone(readUSER_PHONE(it));
	output(info, file_modify_profile(intid, name, password, email, phone));
}

void userModifyPrivilege(char *it, char *info)
{
	init();
	int id1 = readUSER_INTID(it);
	int id2 = readUSER_INTID(it);
	int privilege = readUSER_PRIVILEGE(it);
	output(info, file_modify_priviledge(id1, id2, privilege));
}

void cleanUserShelf()
{
	fout.close();
	fout.open("UserShelf", ios::out);
	fout.close();
}

void cleanCRSystem()
{
	cleanUserShelf();
}

void systemCheck(char *it, char *info)
{
	mystring<40> name(readUSER_NAME(it));
	cout << name.value() << ' ';
	name = readUSER_NAME(it);
	cout << name.value() << endl;
	/*
	string chs;
	for (int i = 0; *it && ++i <= 5; ++it)
		chs += *it;
	cout << chs << endl; */
	output(info, string("蛟龙 陆战队"));
}

} // namespace CR

extern "C"
{
	void initCRSystem()
	{
		CR::init();
	}

	void userRegister(char *it, char *info)
	{
		CR::userRegister(it, info);
	}
	
	void userLogin(char *it, char *info)
	{
		CR::userLogin(it, info);
	}

	void userQueryProfile(char *it, char *info)
	{
		CR::userQueryProfile(it, info);
	}

	void userModifyProfile(char *it, char *info)
	{
		CR::userModifyProfile(it, info);
	}

	void userModifyPrivilege(char *it, char *info)
	{
		CR::userModifyPrivilege(it, info);
	}

	void cleanCRSystem()
	{
		CR::cleanCRSystem();
	}

	void systemCheck(char *it, char *info)
	{
		CR::systemCheck(it, info);
	}
}