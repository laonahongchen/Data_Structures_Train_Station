#include <iostream>
#include <fstream>
#include <cstring>
#include "predefined.h"
#include "bptree.h"
#include "string.h"
#include "read.h"
#include "user.h"
#include "train.h"
#include "ticket.h"
#include "execute.h"

#define maxlen 300

using namespace std;

namespace CR
{

bplus_tree<ticket> theticket("ticket.db");
bplus_tree<train> thetrain("train.db");
bplus_tree<restTicket> theRestTicket("restTicket.db");
bplus_tree<char> find_ticket("find_ticket.db");
bplus_tree<char> find_train("find_train.db");
bplus_tree<char> location("location.db");

void output(char *info, std::string s)
{
	for (auto i : s)
		*info++ = i;
}

void openFile()
{
	fout.open("UserShelf", ios::binary | ios::in | ios::out);
	if (!fout)
	{
		cout << "DEBUG: init user file" << endl;
		fout.open("UserShelf", ios::binary | ios::out);
		fout.close();
		fout.open("UserShelf", ios::binary | ios::in | ios::out);
	}
	else
		cout << "DEBUG: user file existed" << endl;
	fout.seekg(0, ios::end);
	user_number = fout.tellg() / user_block;
	fout.seekg(0, ios::beg);
}

void closeFile()
{
	fout.close();
	theticket.close_file();
	thetrain.close_file();
	theRestTicket.close_file();
	find_ticket.close_file();
	find_train.close_file();
	location.close_file();
}

void userRegister(char *it, char *info)
{
	openFile();
	mystring<40> name(readUSER_NAME(it));
	mystring<20> password(readUSER_PASSWORD(it));
	mystring<20> email(readUSER_EMAIL(it));
	mystring<20> phone(readUSER_PHONE(it));
	User user(name, password, email, phone);
	output(info, file_register(user));
	cout << "DEBUG: new user id: " << user.id.value() << endl;
	closeFile();
}

void userLogin(char *it, char *info)
{
	openFile();
	int intid = readUSER_INTID(it);
	mystring<20> password(readUSER_PASSWORD(it));
	output(info, file_login(intid, password));
	closeFile();
}

void userQueryProfile(char *it, char *info)
{
	openFile();
	int intid = readUSER_INTID(it);
	output(info, file_query_profile(intid));
}

void userModifyProfile(char *it, char *info)
{
	openFile();
	int intid = readUSER_INTID(it);
	mystring<40> name(readUSER_NAME(it));
	mystring<20> password(readUSER_PASSWORD(it));
	mystring<20> email(readUSER_EMAIL(it));
	mystring<20> phone(readUSER_PHONE(it));
	output(info, file_modify_profile(intid, name, password, email, phone));
}

void userModifyPrivilege(char *it, char *info)
{
	openFile();
	int id1 = readUSER_INTID(it);
	int id2 = readUSER_INTID(it);
	int privilege = readUSER_PRIVILEGE(it);
	output(info, file_modify_priviledge(id1, id2, privilege));
}

void queryTicket(char *it, char *info)
{
	openFile();
	mystring<20> loc1 = readLOC(it);
	mystring<20> loc2 = readLOC(it);
	mystring<10> date = readDATE(it);
	mystring<10> catalog = readCATALOG(it);
	output(info, query_ticket(loc1, loc2, date, catalog, find_train, thetrain, theRestTicket));
}

void queryTransfer(char *it, char *info)
{
	openFile();
	mystring<20> loc1 = readLOC(it);
	mystring<20> loc2 = readLOC(it);
	mystring<10> date = readDATE(it);
	mystring<10> catalog = readCATALOG(it);
	output(info, query_transfer(loc1, loc2, date, catalog, find_train, thetrain, location, theRestTicket));
}

void buyTicket(char *it, char *info)
{
	openFile();
	mystring<20> id = readUSER_ID(it);
	int intid = readuser_id(id);
	int num = readNUM(it);
	mystring<20> train_id = readTRAIN_ID(it);
	mystring<20> loc1 = readLOC(it);
	mystring<20> loc2 = readLOC(it);
	mystring<10> date = readDATE(it);
	mystring<20> ticket_kind = readTICKET_KIND(it);
	int intdate = readdate(date);
	output(info, file_buy_ticket(id, intid, num, train_id, loc1, loc2, date, intdate, ticket_kind, thetrain, theticket, find_ticket, theRestTicket));
}

void queryOrder(char *it, char *info)
{
	openFile();
	mystring<20> id = readUSER_ID(it);
	int intid = readuser_id(id);
	mystring<10> date = readDATE(it);
	mystring<10> catalog = readCATALOG(it);
	output(info, file_query_order(id, intid, date, catalog, theticket, find_ticket));
}

void refundTicket(char *it, char *info)
{
	openFile();
	mystring<20> id = readUSER_ID(it);
	int intid = readuser_id(id);
	int num = readNUM(it);
	mystring<20> train_id = readTRAIN_ID(it);
	mystring<20> loc1 = readLOC(it);
	mystring<20> loc2 = readLOC(it);
	mystring<10> date = readDATE(it);
	mystring<20> ticket_kind = readTICKET_KIND(it);
	int intdate = readdate(date);
	output(info, file_refund_ticket(num, id, intid, train_id, loc1, loc2, date, intdate, ticket_kind, theticket, thetrain, theRestTicket));
}

void addTrain(char *it, char *info)
{
	openFile();
	cout << "DEBUG: addTrain" << endl;
	mystring<20> id = readTRAIN_ID(it);
	mystring<40> train_name = readTRAIN_NAME(it);
	mystring<10> catalog = readCATALOG(it);
	char truecatalog = catalog[0];
	int num_station = readNUM(it);
	int num_price = readNUM(it);

	cout << "DEBUG: " << id << " " << train_name << " " << catalog << " " << num_station << " " << num_price << endl;

	mystring<20> name_price[5];
	for (int i = 0; i < num_price; ++i)
		name_price[i] = readLOC(it);
	train_station train_sta[60];
	for (int i = 0; i < num_station; ++i)
	{
		train_sta[i].name = readLOC(it);
		train_sta[i].num_price = num_price;
		train_sta[i].arrive = readTIME(it);
		train_sta[i].start = readTIME(it);
		train_sta[i].stopover = readTIME(it);
		for (int j = 0; j < num_price; ++j)
		{
			train_sta[i].price[j] = readPRICE(it);
		}
	}
	train new_train(id, train_name, truecatalog, num_station, num_price, name_price, train_sta);
	output(info, add_train(id, new_train, thetrain));
	closeFile();
}

void saleTrain(char *it, char *info)
{
	openFile();
	mystring<20> id = readTRAIN_ID(it);
	output(info, sale_train(id, thetrain, find_train, location));
}

void queryTrain(char *it, char *info)
{
	openFile();
	mystring<20> id = readTRAIN_ID(it);
	cout << "DEBUG: queryTrain " << id << endl;
	output(info, query_train(id, thetrain));
	closeFile();
}

void deleteTrain(char *it, char *info)
{
	openFile();
	mystring<20> id = readTRAIN_ID(it);
	output(info, delete_train(id, thetrain));
}

void modifyTrain(char *it, char *info)
{
	openFile();
	mystring<20> id = readTRAIN_ID(it);
	mystring<40> train_name = readTRAIN_NAME(it);
	mystring<10> catalog = readCATALOG(it);
	char truecatalog = catalog[0];
	int num_station = readNUM(it);
	int num_price = readNUM(it);
	mystring<20> name_price[5];
	for (int i = 0; i < num_price; ++i)
		name_price[i] = readLOC(it);
	train_station train_sta[60];
	for (int i = 0; i < num_station; ++i)
	{
		char station_inf[maxlen], *st = station_inf;
		cin.getline(station_inf, maxlen);
		train_sta[i].name = readLOC(st);
		train_sta[i].num_price = num_price;
		train_sta[i].arrive = readTIME(st);
		train_sta[i].start = readTIME(st);
		train_sta[i].stopover = readTIME(st);
		for (int j = 0; j < num_price; ++j)
			train_sta[i].price[j] = readPRICE(st);
	}
	output(info, modify_train(id, train_name, truecatalog, num_station, num_price, name_price, train_sta, thetrain));
}

void cleanCRSystem()
{
	fout.close();
	fout.open("UserShelf", ios::binary | ios::out);
	fout.close();
	fstream fout1;
	fout1.open("ticket.db", ios::out);
	fout1.close();
	fout1.open("train.db", ios::out);
	fout1.close();
	fout1.open("find_ticket.db", ios::out);
	fout1.close();
	fout1.open("find_train.db", ios::out);
	fout1.close();
	fout1.open("location.db", ios::out);
	fout1.close();
	fout1.open("restTicket.db", ios::out);
	fout1.close();
	user_number = 0;
}

void systemCheck(char *it, char *info)
{
	mystring<40> name(readUSER_NAME(it));
	cout << name.value() << ' ';
	name = readUSER_NAME(it);
	cout << name.value() << endl;
	output(info, string("Star Labs"));
}

} // namespace CR

extern "C"
{
	void initCRSystem()
	{
		CR::openFile();
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

	void queryTicket(char *it, char *info)
	{
		CR::queryTicket(it, info);
	}

	void queryTransfer(char *it, char *info)
	{
		CR::queryTransfer(it, info);
	}

	void buyTicket(char *it, char *info)
	{
		CR::buyTicket(it, info);
	}

	void queryOrder(char *it, char *info)
	{
		CR::queryOrder(it, info);
	}

	void refundTicket(char *it, char *info)
	{
		CR::refundTicket(it, info);
	}

	void addTrain(char *it, char *info)
	{
		CR::addTrain(it, info);
	}

	void saleTrain(char *it, char *info)
	{
		CR::saleTrain(it, info);
	}

	void queryTrain(char *it, char *info)
	{
		CR::queryTrain(it, info);
	}

	void deleteTrain(char *it, char *info)
	{
		CR::deleteTrain(it, info);
	}

	void modifyTrain(char *it, char *info)
	{
		CR::modifyTrain(it, info);
	}
}