#pragma once

#include "bptree.h"
#include "string.h"

using namespace std;

struct train_station {
	mystring<20> name;
	int day_in_offset;
	int day_out_offset;
	mystring<5> arrive;
	mystring<5> start;
	mystring<5> stopover;
	int num_price;
	double price[5];

	train_station() : day_in_offset(0), day_out_offset(0) {}

	train_station(const mystring<20> na, const mystring<5> arr, const mystring<5> sta, const mystring<5> sto, size_t num_pri,
		const double *pri) : name(na), arrive(arr), start(sta), stopover(sto), num_price(num_pri), day_in_offset(0), day_out_offset(0)
	{
		for (int i = 0; i < num_price; ++i)
			price[i] = pri[i];
	}
};

struct train {
	mystring<20> train_id;
	mystring<40> name;
	char catalog;
	mystring<20> name_price[5];
	bool besaled;
	int num_station;
	int num_price;
	int ticket[30][59][5] = { 0 };  //ticket[i][j][k]: date: i, kind: k, sta[j] -> sta[j+1] has how many tickets
	train_station sta[60];

	train() : besaled(false), num_station(0), num_price(0) {}


	train(const mystring<20> &id, const mystring<40> &na, const char &cat, int num_sta, int num_pri, const mystring<20>* name_pri,
		const train_station *s) :
		num_station(num_sta), num_price(num_pri), train_id(id), name(na), catalog(cat)
	{
		for (int i = 0; i < 30; ++i) {
			for (int j = 0; j < num_station - 1; ++j) {
				for (int k = 0; k < num_price; ++k)
					ticket[i][j][k] = 2000;
			}
		}
		for (int i = 0; i < num_price; ++i)
			name_price[i] = name_pri[i];
		besaled = false;

		sta[0].arrive = s[0].arrive;
		sta[0].start = s[0].start;
		sta[0].stopover = s[0].stopover;
		sta[0].name = s[0].name;
		sta[0].num_price = s[0].num_price;
		for (int j = 0; j < num_price; ++j)
			sta[0].price[j] = s[0].price[j];
		sta[0].day_in_offset = sta[0].day_out_offset = 0;

		for (int i = 1; i < num_station; ++i) {
			sta[i].arrive = s[i].arrive;
			sta[i].start = s[i].start;
			sta[i].stopover = s[i].stopover;
			sta[i].name = s[i].name;
			sta[i].num_price = s[i].num_price;
			for (int j = 0; j < num_price; ++j)
				sta[i].price[j] = s[i].price[j];
			if (sta[i].arrive < sta[i - 1].start)
				sta[i].day_in_offset = sta[i - 1].day_out_offset + 1;
			else
				sta[i].day_in_offset = sta[i - 1].day_out_offset;
			if (sta[i].start < sta[i].arrive)
				sta[i].day_out_offset = sta[i].day_in_offset + 1;
			else
				sta[i].day_out_offset = sta[i].day_in_offset;
		}
	}


	void sale_train(bplus_tree<char> &find_train, bplus_tree<char> &location) {
		if (besaled) {
			cout << 0 << '\n';
			return;
		}
		besaled = true;
		cout << 1 << '\n';

		for (int i = 0; i < num_station - 1; ++i) {
			for (int j = i + 1; j < num_station; ++j) {
				mystring<60> Comkey = sta[i].name + sta[j].name + train_id;
				find_train.insert(Comkey, catalog);
			}
		}

		for (int i = 0; i < num_station; ++i) {
			location.insert(sta[i].name, catalog);
		}

	}

	void query() {
		if (!besaled) {
			cout << 0 << '\n';
			return;
		}
		cout << train_id << ' ' << name << ' ' << catalog << ' ' << num_station << ' ' << num_price;
		for (int i = 0; i < num_price; ++i)
			cout << ' ' << name_price[i];
		cout << '\n';

		for (int i = 0; i < num_station; ++i) {
			cout << sta[i].name << ' ' << sta[i].arrive << ' ' << sta[i].start << ' ' << sta[i].stopover;
			for (int j = 0; j < num_price; ++j)
				cout << ' ' << ("гд") << sta[i].price[j];
			cout << '\n';
		}
	}

	bool modify(const mystring<40> &na, const char &cat, int num_sta, int num_pri, const mystring<20>* name_pri,
		const train_station *s)
	{
		if (besaled) {
			cout << 0 << '\n';
			return false;
		}
		num_station = num_sta;
		num_price = num_pri;
		name = na;
		catalog = cat;
		for (int i = 0; i < num_price; ++i)
			name_price[i], name_pri[i];
		besaled = false;

		sta[0].arrive = s[0].arrive;
		sta[0].start = s[0].start;
		sta[0].stopover = s[0].stopover;
		sta[0].name = s[0].name;
		sta[0].num_price = s[0].num_price;
		for (int j = 0; j < num_price; ++j)
			sta[0].price[j] = s[0].price[j];
		sta[0].day_in_offset = sta[0].day_out_offset = 0;

		for (int i = 1; i < num_station; ++i) {
			sta[i].arrive = s[i].arrive;
			sta[i].start = s[i].start;
			sta[i].stopover = s[i].stopover;
			sta[i].name = s[i].name;
			sta[i].num_price = num_price;
			for (int j = 0; j < num_price; ++j)
				sta[i].price[j] = s[i].price[j];
			if ((mystring<5>)sta[i].arrive < (mystring<5>)sta[i - 1].start)
				sta[i].day_in_offset = sta[i - 1].day_out_offset + 1;
			else
				sta[i].day_in_offset = sta[i - 1].day_out_offset;
			if ((mystring<5>)sta[i].arrive >(mystring<5>)sta[i].start)
				sta[i].day_out_offset = sta[i].day_in_offset + 1;
			else
				sta[i].day_out_offset = sta[i].day_in_offset;
		}

		cout << 1 << '\n';
		return true;
	}

	bool buy(int num, const mystring<20> &loc1, const mystring<20> &loc2, int date, const mystring<20> &kind) {
		int ticket_kind, loc1_pos, loc2_pos;
		if (!besaled)
			return false;
		for (ticket_kind = 0; ticket_kind < num_price; ++ticket_kind) {
			if ((mystring<20>)kind == name_price[ticket_kind])
				break;
		}
		if (ticket_kind == num_price)
			return false;
		for (loc1_pos = 0; loc1_pos < num_station - 1; ++loc1_pos) {
			if ((mystring<20>)loc1 == sta[loc1_pos].name)
				break;
		}
		if (loc1_pos == num_station - 1)
			return false;
		for (loc2_pos = loc1_pos + 1; loc2_pos < num_station; ++loc2_pos) {
			if ((mystring<20>)loc2 == sta[loc2_pos].name)
				break;
		}
		if (loc2_pos == num_station)
			return false;
		for (int i = loc1_pos; i < loc2_pos; ++i) {
			if (ticket[date - 1][i][ticket_kind] < num)
				return false;
		}
		for (int i = loc1_pos; i < loc2_pos; ++i)
			ticket[date - 1][i][ticket_kind] -= num;

		return true;

	}

	bool refund(int num, const mystring<20> &loc1, const mystring<20> &loc2, int date, const mystring<20> &kind) {
		int ticket_kind, loc1_pos, loc2_pos;

		if (!besaled)
			return false;

		for (ticket_kind = 0; ticket_kind < num_price; ++ticket_kind) {
			if ((mystring<20>)kind == name_price[ticket_kind])
				break;
		}
		if (ticket_kind == num_price)
			return false;

		for (loc1_pos = 0; loc1_pos < num_station - 1; ++loc1_pos) {
			if ((mystring<20>)loc1 == sta[loc1_pos].name)
				break;
		}
		if (loc1_pos == num_station - 1)
			return false;

		for (loc2_pos = loc1_pos + 1; loc2_pos < num_station; ++loc2_pos) {
			if ((mystring<20>)loc2 == sta[loc2_pos].name)
				break;
		}
		if (loc2_pos == num_station)
			return false;

		for (int i = loc1_pos; i < loc2_pos; ++i) {
			if (ticket[date - 1][i][ticket_kind] + num > 2000)
				return false;
		}

		for (int i = loc1_pos; i < loc2_pos; ++i)
			ticket[date - 1][i][ticket_kind] += num;

		return true;
	}

	void queryTicket(const mystring<20> &loc1, const mystring<20> &loc2, int date) {
		int loc1_pos, loc2_pos;
		for (loc1_pos = 0; loc1_pos < num_station - 1; ++loc1_pos) {
			if (sta[loc1_pos].name == loc1)
				break;
		}
		for (loc2_pos = loc1_pos + 1; loc2_pos < num_station; ++loc2_pos) {
			if (sta[loc2_pos].name == loc2)
				break;
		}
		cout << train_id << ' ';
		int date_from = date + sta[loc1_pos].day_out_offset;
		int date_to = date + sta[loc2_pos].day_in_offset;
		int month_from = (date_from > 30) ? 7 : 6;
		int month_to = (date_to > 30) ? 7 : 6;
		if (date_from > 30)
			date_from -= 30;
		if (date_to > 30)
			date_to -= 30;

		mystring<10> ret_date_loc1("2018-0"), ret_date_loc2("2018-0");

		ret_date_loc1 += ('0' + month_from);
		ret_date_loc1 += '-';

		ret_date_loc2 += ('0' + month_to);
		ret_date_loc2 += '-';

		ret_date_loc1 += ('0' + date_from / 10);
		ret_date_loc1 += ('0' + date_from % 10);


		ret_date_loc2 += ('0' + date_to / 10);
		ret_date_loc2 += ('0' + date_to % 10);

		cout << loc1 << ' ' << ret_date_loc1 << ' ' << sta[loc1_pos].start << ' ' << loc2 << ' ' << ret_date_loc2 << ' ' << sta[loc2_pos].arrive;

		for (int i = 0; i < num_price; ++i) {
			int ticket_num = 2000;
			cout << ' ' << name_price[i];
			for (int j = loc1_pos; j < loc2_pos; ++j) {
				if (ticket[date - 1][j][i] < ticket_num)
					ticket_num = ticket[date - 1][j][i];
			}
			double price = 0;
			for (int j = loc1_pos; j <= loc2_pos; ++j)
				price += sta[j].price[i];

			cout << ' ' << ticket_num << ' ' << price;
		}
		cout << '\n';
	}


};

void add_train(const mystring<20> &id, const train& newtrain, bplus_tree<train> &thetrain)
{
	train pos;
	if (thetrain.search(id, &pos) == 0) {
		cout << 0 << '\n';
		return;
	}

	thetrain.insert(id, newtrain);
	cout << 1 << '\n';
}

void sale_train(const mystring<20> &id, bplus_tree<train> &thetrain, bplus_tree<char> &findtrain, bplus_tree<char> &loc)
{
	train pos;
	if (thetrain.search(id, &pos) != 0) {
		cout << 0 << '\n';
		return;
	}

	pos.sale_train(findtrain, loc);

	thetrain.update(id, pos);
}

void query_train(const mystring<20> &id, bplus_tree<train> &thetrain)
{
	train pos;
	if (thetrain.search(id, &pos) != 0) {
		cout << 0 << '\n';
		return;
	}

	pos.query();
}

void delete_train(const mystring<20> &id, bplus_tree<train> &thetrain)
{
	train pos;
	if (thetrain.search(id, &pos) != 0) {
		cout << 0 << '\n';
		return;
	}

	//pos.delete_train();
	thetrain.remove(id);
	cout << 1 << '\n';
}

void modify_train(const mystring<20> &id, const mystring<40> &na, const char &cat, int num_sta, int num_pri,
	const mystring<20>* name_pri, const train_station *s, bplus_tree<train> &thetrain)
{
	train pos;
	if (thetrain.search(id, &pos) != 0) {
		cout << 0 << '\n';
		return;
	}

	if (pos.modify(na, cat, num_sta, num_pri, name_pri, s))
		thetrain.update(id, pos);

}

bool refund_ticket(int num, const mystring<20> &trainid, const mystring<20> &loc1, const mystring<20> &loc2,
	int date, const mystring<20> &kind, bplus_tree<train> &thetrain)
{
	train pos;
	if (thetrain.search(trainid, &pos) != 0) {
		cout << 0 << '\n';
		return false;
	}
	if (pos.refund(num, loc1, loc2, date, kind)) {
		cout << 1 << '\n';
		thetrain.update(trainid, pos);
		return true;
	}
	cout << 0 << '\n';
	return false;
}

