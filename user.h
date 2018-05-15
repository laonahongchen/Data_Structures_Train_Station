#pragma once
#include <cstring>
#include "string.h"

using namespace std;

struct User {
	mystring name;
	mystring password;
	mystring email;
	mystring phone;
	mystring id;
	int intid;
	int privilege;
	User() : intid(-1), privilege(0) {}
	User(const mystring &cname, const mystring &cpassword, const mystring &cemail, const mystring &cphone) : name(cname, 40), password(cpassword, 20), email(cemail, 20), phone(cphone, 20), intid(-1), privilege(0) {}
};