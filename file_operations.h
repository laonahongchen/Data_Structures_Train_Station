#pragma once
#include <fstream>
#include "string.h"
#include "user.h"

#define user_block (sizeof(char) * 120 + sizeof(int))

int user_number = 0;
fstream fout;

void file_register(User &user) {
	//fout.seekp(0, ios::beg);
	fout.seekp(0, ios::end);
	//cout << fout.tellg();
	user.intid = 2018 + user_number;
	user.id = IntToString(user.intid);
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
	/*fstream fin("UserShelf.txt", ios::binary | ios::in);
	int cpr;
	char cid[21], cname[41], cpwd[21], cem[21], cph[21];
	fin.seekg(0, ios::beg);
	fin.read(reinterpret_cast<char *>(&cid), sizeof(char) * 20);
	fin.read(reinterpret_cast<char *>(&cname), sizeof(char) * 40);
	fin.read(reinterpret_cast<char *>(&cpwd), sizeof(char) * 20);
	fin.read(reinterpret_cast<char *>(&cem), sizeof(char) * 20);
	fin.read(reinterpret_cast<char *>(&cph), sizeof(char) * 20);
	fin.read(reinterpret_cast<char *>(&cpr), sizeof(int));
	cid[20] = '\0';
	cname[40] = '\0';
	cpwd[20] = '\0';
	cem[20] = '\0';
	cph[20] = '\0';
	cout << cid << endl;
	cout << cname << endl;
	cout << cpwd << endl;
	cout << cem << endl;
	cout << cph << endl;
	cout << cpr << endl;*/
	return;
}

void file_login(const int cintid, const mystring &cpassword) {
	if (cintid < 2018 || cintid >= 2018 + user_number) {
		cout << 0 << '\n';
		return;
	}
	fout.seekp((cintid - 2018) * user_block + sizeof(char) * 60);
	char charpassword[21];
	fout.read(reinterpret_cast<char *>(&charpassword), sizeof(char) * 20);
	charpassword[20] = '\0';
	mystring password(charpassword, 20);
	fout.seekp(0, ios::beg);
	if (password == cpassword) cout << 1 << '\n';
	else cout << 0 << '\n';
	return;
}

void file_query_profile(const int cintid) {
	if (cintid < 2018 || cintid >= 2018 + user_number) {
		cout << 0 << '\n';
		return;
	}
	fout.seekp((cintid - 2018) * user_block + sizeof(char) * 20);
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
	mystring name(charname, 40), password(charpassword, 20), email(charemail, 20), phone(charphone, 20);
	cout << name << password << email << phone << privilege << '\n';
	fout.seekp(0, ios::beg);
	return;
}

void file_modify_profile(const int cintid, const mystring &cname, const mystring &cpassword, const mystring &cemail, const mystring &cphone) {
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
	fout.seekp((id1 - 2018) * user_block + sizeof(char) * 120);
	int charprivilege1, charprivilege2;
	fout.read(reinterpret_cast<char *>(&charprivilege1), sizeof(int));
	fout.seekp(0, ios::beg);
	fout.seekp((id2 - 2018) * user_block + sizeof(char) * 120);
	fout.read(reinterpret_cast<char *>(&charprivilege2), sizeof(int));
	if (charprivilege1 == 2 && charprivilege2 != 2) {
		fout.seekg(-4, ios::cur);
		fout.write(reinterpret_cast<const char *>(&privilege), sizeof(int));
		cout << 1 << '\n';
	}
	else cout << 0 << '\n';
	fout.seekp(0, ios::beg);
}