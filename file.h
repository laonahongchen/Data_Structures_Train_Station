#pragma once
#include <fstream>
#include "user.h"
#include "string.h"

#define user_block (sizeof(char) * 120 + sizeof(int))

int user_number = 0;
fstream fout;

void file_register(User &user) {
	fout.seekp(0, ios::end);
	user.intid = 2018 + user_number;
	user.id = IntToString<20>(user.intid);
	if (user.intid == 2018) user.privilege = 2;
	else user.privilege = 1;
	fout.write(reinterpret_cast<char *>(user.id.str), sizeof(char) * 20);
	fout.write(reinterpret_cast<char *>(user.name.str), sizeof(char) * 40);
	fout.write(reinterpret_cast<char *>(user.password.str), sizeof(char) * 20);
	fout.write(reinterpret_cast<char *>(user.email.str), sizeof(char) * 20);
	fout.write(reinterpret_cast<char *>(user.phone.str), sizeof(char) * 20);
	fout.write(reinterpret_cast<const char *>(&user.privilege), sizeof(int));
	fout.seekp(0, ios::beg);
	++user_number;
	cout << user.id << '\n';
	return;
}

void file_login(const int cintid, const mystring<20> &cpassword) {
	if (cintid < 2018 || cintid >= 2018 + user_number) {
		cout << 0 << '\n';
		return;
	}
	fout.seekg((cintid - 2018) * user_block + sizeof(char) * 60);
	char charpassword[21];
	fout.read(reinterpret_cast<char *>(&charpassword), sizeof(char) * 20);
	charpassword[20] = '\0';
	mystring<20> password(charpassword);
	fout.seekg(0, ios::beg);
	if (password == cpassword) cout << 1 << '\n';
	else cout << 0 << '\n';
	return;
}

void file_query_profile(const int cintid) {
	if (cintid < 2018 || cintid >= 2018 + user_number) {
		cout << 0 << '\n';
		return;
	}
	fout.seekg((cintid - 2018) * user_block + sizeof(char) * 20);
	char charname[41], charpassword[21], charemail[21], charphone[21];
	int privilege;
	fout.read(reinterpret_cast<char *>(&charname), sizeof(char) * 40);
	fout.read(reinterpret_cast<char *>(&charpassword), sizeof(char) * 20);
	fout.read(reinterpret_cast<char *>(&charemail), sizeof(char) * 20);
	fout.read(reinterpret_cast<char *>(&charphone), sizeof(char) * 20);
	fout.read(reinterpret_cast<char *>(&privilege), sizeof(int));
	charname[40] = '\0';
	charpassword[20] = '\0';
	charemail[20] = '\0';
	charphone[20] = '\0';
	mystring<40> name(charname);
	mystring<20> password(charpassword);
	mystring<20> email(charemail);
	mystring<20> phone(charphone);
	cout << name <<  ' ' << email << ' ' << phone << ' ' << privilege << '\n';
	fout.seekg(0, ios::beg);
	return;
}

void file_modify_profile(const int cintid, const mystring<40> &cname, const mystring<20> &cpassword, const mystring<20> &cemail, const mystring<20> &cphone) {
	if (cintid < 2018 || cintid >= 2018 + user_number) {
		cout << 0 << '\n';
		return;
	}
	fout.seekp((cintid - 2018) * user_block + sizeof(char) * 20);
	fout.write(reinterpret_cast<const char *>(cname.str), sizeof(char) * 40);
	fout.write(reinterpret_cast<const char *>(cpassword.str), sizeof(char) * 20);
	fout.write(reinterpret_cast<const char *>(cemail.str), sizeof(char) * 20);
	fout.write(reinterpret_cast<const char *>(cphone.str), sizeof(char) * 20);
	fout.seekp(0, ios::beg);
	cout << 1 << '\n';
	return;
}

void file_modify_priviledge(const int &id1, const int &id2, const int &privilege) {
	if (id1 < 2018 || id1 >= 2018 + user_number || id2 < 2018 || id2 >= 2018 + user_number) {
		cout << 0 << '\n';
		return;
	}
	fout.seekg((id1 - 2018) * user_block + sizeof(char) * 120);
	int privilege1, privilege2;
	fout.read(reinterpret_cast<char *>(&privilege1), sizeof(int));
	fout.seekg(0, ios::beg);
	fout.seekg((id2 - 2018) * user_block + sizeof(char) * 120);
	fout.read(reinterpret_cast<char *>(&privilege2), sizeof(int));
	if (privilege1 == 2 && privilege2 != 2) {
		//fout.seekg(-4, ios::cur);
		fout.seekp((id2 - 2018) * user_block + sizeof(char) * 120);
		fout.write(reinterpret_cast<const char *>(&privilege), sizeof(int));
		cout << 1 << '\n';
	}
	else cout << 0 << '\n';
	fout.seekg(0, ios::beg);
	return;
}