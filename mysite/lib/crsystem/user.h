#pragma once
#include "string.h"
struct User {
	mystring<20> id;
	mystring<40> name;
	mystring<20> password;
	mystring<20> email;
	mystring<20> phone;
	int intid;
	int privilege;
	User() : intid(-1), privilege(0) {}
	User(const mystring<40> &cname, const mystring<20> &cpassword, const mystring<20> &cemail, const mystring<20> &cphone) : name(cname), password(cpassword), email(cemail), phone(cphone), intid(-1), privilege(0) {}
};