#include "train_id.h"

#define maxlen 1000

using namespace std;

int main() {

	bplus_tree<train> thetrain("train.db");

	while(true) {

		char command[maxlen], *it = command;
		cin.getline(command, maxlen);
		TRAIN_TYPE commandtype = read_TYPE(it);

		if (commandtype == ADD) {
			mystring<20> id = read_id(it);
			mystring<40> train_name = read_name(it);
			mystring<10> catalog = read_catalog(it);
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

			train new_train(id, train_name, catalog, num_station, num_price, name_price, train_sta);
			addtrain(id, new_train, thetrain);
			continue;
		}


		if (commandtype == SALE) {
			mystring<20> id = read_id(it);
			saletrain(id, thetrain);
			continue;
		}


		if (commandtype == QUERY) {
			mystring<20> id = read_id(it);
			querytrain(id, thetrain);
			continue;
		}


		if (commandtype == DELETE) {
			mystring<20> id = read_id(it);
			deletetrain(id, thetrain);
			continue;
		}


		if (commandtype == MODIFY) {
			mystring<20> id = read_id(it);
			mystring<40> train_name = read_name(it);
			mystring<10> catalog = read_catalog(it);
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

			modifytrain(id, train_name, catalog, num_station, num_price, name_price, train_sta, thetrain);
			continue;
		}

		if (commandtype == EXIT) {
			cout << "BYE" << '\n';
			exit(0);
			continue;
		}
		
		

	}

}
