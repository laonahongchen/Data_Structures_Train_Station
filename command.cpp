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
	fstream fout1;
	fout1.open("ticket.db", ios::binary | ios::in | ios::out);
	if (!fout1) {
		fout1.open("ticket.db", ios::binary | ios::out);
		fout1.close();
	}
	else fout1.close();
	fout1.open("train.db", ios::binary | ios::in | ios::out);
	if (!fout1) {
		fout1.open("train.db", ios::binary | ios::out);
		fout1.close();
	}
	else fout1.close();
	fout1.open("find_ticket.db", ios::binary | ios::in | ios::out);
	if (!fout1) {
		fout1.open("find_ticket.db", ios::binary | ios::out);
		fout1.close();
	}
	else fout1.close();
	fout1.open("find_train.db", ios::binary | ios::in | ios::out);
	if (!fout1) {
		fout1.open("find_train.db", ios::binary | ios::out);
		fout1.close();
	}
	else fout1.close();
	fout1.open("location.db", ios::binary | ios::in | ios::out);
	if (!fout1) {
		fout1.open("location.db", ios::binary | ios::out);
		fout1.close();
	}
	else fout1.close();
	return;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	init();
	bplus_tree<ticket> theticket("ticket.db");
	bplus_tree<train> thetrain("train.db");
	bplus_tree<char> find_ticket("find_ticket.db");
	bplus_tree<char> find_train("find_train.db");
	bplus_tree<char> location("location.db");
	while (true) {
		char command[maxlen], *it = command;
		cin.getline(command, maxlen);
		int commandtype = readCOMMAND_TYPE(it);
		if (commandtype == REGISTER) {
			mystring<40> name(readUSER_NAME(it));
			mystring<20> password(readUSER_PASSWORD(it));
			mystring<20> email(readUSER_EMAIL(it));
			mystring<20> phone(readUSER_PHONE(it));
			User user(name, password, email, phone);
			file_register(user);
		}
		else if (commandtype == LOGIN) {
			int intid = readUSER_INTID(it);
			mystring<20> password(readUSER_PASSWORD(it));
			file_login(intid, password);
		}
		else if (commandtype == QUERY_PROFILE) {
			int intid = readUSER_INTID(it);
			file_query_profile(intid);
		}
		else if (commandtype == MODIFY_PROFILE) {
			int intid = readUSER_INTID(it);
			mystring<40> name(readUSER_NAME(it));
			mystring<20> password(readUSER_PASSWORD(it));
			mystring<20> email(readUSER_EMAIL(it));
			mystring<20> phone(readUSER_PHONE(it));
			file_modify_profile(intid, name, password, email, phone);
		}
		else if (commandtype == MODIFY_PRIVILEGE) {
			int id1 = readUSER_INTID(it);
			int id2 = readUSER_INTID(it);
			int privilege = readUSER_PRIVILEGE(it);
			file_modify_priviledge(id1, id2, privilege);
		}
		else if (commandtype == QUERY_TICKET) {
			mystring<20> loc1 = readLOC(it);
			mystring<20> loc2 = readLOC(it);
			int intdate = readINTDATE(it);
			mystring<10> catalog = readCATALOG(it);
			query_ticket(loc1, loc2, intdate, catalog, find_train, thetrain);
		}
		else if (commandtype == QUERY_TRANSFER) {
			mystring<20> loc1 = readLOC(it);
			mystring<20> loc2 = readLOC(it);
			int date = readINTDATE(it);
			mystring<10> catalog = readCATALOG(it);
			query_transfer(loc1, loc2, date, catalog, find_train, thetrain, location);
		}
		else if (commandtype == BUY_TICKET) {
			mystring<20> id = readUSER_ID(it);
			int intid = readuser_id(id);
			int num = readNUM(it);
			mystring<20> train_id = readTRAIN_ID(it);
			mystring<20> loc1 = readLOC(it);
			mystring<20> loc2 = readLOC(it);
			mystring<10> date = readDATE(it);
			mystring<20> ticket_kind = readTICKET_KIND(it);
			int intdate = readdate(date);
			file_buy_ticket(id, intid, num, train_id, loc1, loc2, date, intdate, ticket_kind, thetrain, theticket, find_ticket);
		}
		else if (commandtype == QUERY_ORDER) {
			mystring<20> id = readUSER_ID(it);
			int intid = readuser_id(id);
			mystring<10> date = readDATE(it);
			mystring<10> catalog = readCATALOG(it);
			file_query_order(id, intid, date, catalog, theticket, find_ticket);
		}
		else if (commandtype == REFUND_TICKET) {
			mystring<20> id = readUSER_ID(it);
			int intid = readuser_id(id);
			int num = readNUM(it);
			mystring<20> train_id = readTRAIN_ID(it);
			mystring<20> loc1 = readLOC(it);
			mystring<20> loc2 = readLOC(it);
			mystring<10> date = readDATE(it);
			mystring<20> ticket_kind = readTICKET_KIND(it);
			int intdate = readdate(date);
			file_refund_ticket(num, id, intid, train_id, loc1, loc2, date, intdate, ticket_kind, theticket, thetrain);
		}
		else if (commandtype == ADD_TRAIN) {
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
			for (int i = 0; i < num_station; ++i) {
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
			train new_train(id, train_name, truecatalog, num_station, num_price, name_price, train_sta);
			add_train(id, new_train, thetrain);
		}
		else if (commandtype == SALE_TRAIN) {
			mystring<20> id = readTRAIN_ID(it);
			sale_train(id, thetrain, find_train, location);
		}
		else if (commandtype == QUERY_TRAIN) {
			mystring<20> id = readTRAIN_ID(it);
			query_train(id, thetrain);
		}
		else if (commandtype == DELETE_TRAIN) {
			mystring<20> id = readTRAIN_ID(it);
			delete_train(id, thetrain);
		}
		else if (commandtype == MODIFY_TRAIN) {
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
			for (int i = 0; i < num_station; ++i) {
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
			modify_train(id, train_name, truecatalog, num_station, num_price, name_price, train_sta, thetrain);
		}
		else if (commandtype == EXIT) {
			fout.close();
			cout << "BYE" << '\n';
			exit(0);
		}
		else if (commandtype == CLEAN) {
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
			cout << 1 << '\n';
		}
		else if (commandtype == INVALID_COMMANDTYPE) continue;
	}
	return 0;
}