#include "ticket.h"

#define maxlen 1000

using namespace std;

int main() {

	bplus_tree<train> thetrain("train.db");
	bplus_tree<char> find_train("find_train.db");
	bplus_tree<char> location("location.db");

	while(true) {

		char command[maxlen], *it = command;
		cin.getline(command, maxlen);
		TRAIN_TYPE commandtype = read_TYPE(it);

		if (commandtype == ADD) {
			mystring<20> id = read_id(it);
			mystring<40> train_name = read_name(it);
			mystring<10> catalog = read_catalog(it);
			char truecatalog = catalog[0];
			int num_station = read_num(it);
			int num_price = read_num(it);
			mystring<40> name_price[5];
			for (int i = 0; i < num_price; ++i)
				name_price[i] = read_name(it);

			train_station train_sta[60];
			for (int i = 0; i < num_station; ++i) {
				char station_inf[maxlen], *st = station_inf;
				cin.getline(station_inf, maxlen);
				train_sta[i].name = read_name(st);
				train_sta[i].num_price = num_price;
				train_sta[i].arrive = read_time(st);
				train_sta[i].start = read_time(st);
				train_sta[i].stopover = read_time(st);
				for (int j = 0; j < num_price; ++j) 
					train_sta[i].price[j] = read_price(st);
			}

			train new_train(id, train_name, truecatalog, num_station, num_price, name_price, train_sta);
			add_train(id, new_train, thetrain);
			continue;
		}


		if (commandtype == SALE) {
			mystring<20> id = read_id(it);
			sale_train(id, thetrain, find_train, location);
			continue;
		}


		if (commandtype == QUERY) {
			mystring<20> id = read_id(it);
			query_train(id, thetrain);
			continue;
		}


		if (commandtype == DELETE) {
			mystring<20> id = read_id(it);
			delete_train(id, thetrain);
			continue;
		}


		if (commandtype == MODIFY) {
			mystring<20> id = read_id(it);
			mystring<40> train_name = read_name(it);
			mystring<10> catalog = read_catalog(it);
			char truecatalog = catalog[0];
			int num_station = read_num(it);
			int num_price = read_num(it);
			mystring<40> name_price[5];
			for (int i = 0; i < num_price; ++i)
				name_price[i] = read_name(it);

			train_station train_sta[60];
			for (int i = 0; i < num_station; ++i) {
				char station_inf[maxlen], *st = station_inf;
				cin.getline(station_inf, maxlen);
				train_sta[i].name = read_name(st);
				train_sta[i].num_price = num_price;
				train_sta[i].arrive = read_time(st);
				train_sta[i].start = read_time(st);
				train_sta[i].stopover = read_time(st);
				for (int j = 0; j < num_price; ++j)
					train_sta[i].price[j] = read_price(st);
			}

			modify_train(id, train_name, truecatalog, num_station, num_price, name_price, train_sta, thetrain);
			continue;
		}

		if (commandtype == EXIT) {
			cout << "BYE" << '\n';
			exit(0);
			continue;
		}

		if (commandtype == BUY) {
			mystring<20> user_id = read_id(it);
			int num = read_num(it);
			mystring<20> train_id = read_id(it);
			mystring<40> loc1 = read_name(it);
			mystring<40> loc2 = read_name(it);
			int date = read_date(it);
			mystring<40> ticket_kind = read_name(it);
			buy_ticket(num, train_id, loc1, loc2, date, ticket_kind, thetrain);
			continue;
		}

		if (commandtype == REFUND) {
			mystring<20> user_id = read_id(it);
			int num = read_num(it);
			mystring<20> train_id = read_id(it);
			mystring<40> loc1 = read_name(it);
			mystring<40> loc2 = read_name(it);
			int date = read_date(it);
			mystring<40> ticket_kind = read_name(it);
			refund_ticket(num, train_id, loc1, loc2, date, ticket_kind, thetrain);
			continue;
		}
		
		if (commandtype == QUERYTICKET) {
			mystring<40> loc1 = read_name(it);
			mystring<40> loc2 = read_name(it);
			int date = read_date(it);
			mystring<10> catalog = read_catalog(it);
			query_ticket(loc1, loc2, date, catalog, find_train, thetrain);
		}

		if (commandtype == QUERYTRAINSFER) {
			mystring<40> loc1 = read_name(it);
			mystring<40> loc2 = read_name(it);
			int date = read_date(it);
			mystring<10> catalog = read_catalog(it);
			query_transfer(loc1, loc2, date, catalog, find_train, thetrain, location);
		}

	}

}
