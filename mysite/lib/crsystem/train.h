#pragma once

#include "bptree.h"
#include "string.h"
#include <cstring>
#include <iomanip>

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

struct restTicket {
	int ticket[59][5];

	restTicket() {
		for (int j = 0; j < 59; ++j) {
			for (int k = 0; k < 5; ++k)
				ticket[j][k] = 2000;
		}
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
	train_station sta[60];
	train() : besaled(false), num_station(0), num_price(0) {}


	train(const mystring<20> &id, const mystring<40> &na, const char &cat, int num_sta, int num_pri, const mystring<20>* name_pri,
		const train_station *s) :
		num_station(num_sta), num_price(num_pri), train_id(id), name(na), catalog(cat)
	{

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


	bool sale_train(bplus_tree<char> &find_train, bplus_tree<char> &location) {
		if (besaled) {
			return false;;
		}
		besaled = true;

		for (int i = 0; i < num_station - 1; ++i) {
			for (int j = i + 1; j < num_station; ++j) {
				mystring<60> Comkey = sta[i].name + sta[j].name + train_id;
				find_train.insert(Comkey, catalog);
			}
		}

		for (int i = 0; i < num_station; ++i) {
			location.insert(sta[i].name, catalog);
		}
		return true;
	}

	string query() {
		/*
		if (!besaled) {
			return "0";
		}
		*/
		string ans;
		string blank = " ";
		ans += train_id.value() + blank + name.value() + blank + catalog + blank + to_string(num_station) + blank + to_string(num_price) + blank;
		for (int i = 0; i < num_price; ++i)
			ans += name_price[i].value() + blank;

		for (int i = 0; i < num_station; ++i) {
			ans += sta[i].name.value() + blank + sta[i].arrive.value() + blank + sta[i].start.value() + blank + sta[i].stopover.value() + blank;
			for (int j = 0; j < num_price; ++j)
			{
				ans += to_string(sta[i].price[j]) + blank;
			}
		}
		return ans;
	}

	bool modify(const mystring<40> &na, const char &cat, int num_sta, int num_pri, const mystring<20>* name_pri,
		const train_station *s)
	{
		if (besaled) {
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
		return true;
	}

	bool buy(int num, const mystring<20> &loc1, const mystring<20> &loc2, const mystring<10> &date, const mystring<20> &kind, bplus_tree<restTicket> &theRestTicket) {
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

		mystring<30> key = date + train_id;
		restTicket here, there;
		if (theRestTicket.search(key, &here) != 0) {
			for (int i = loc1_pos; i < loc2_pos; ++i) {
				there.ticket[i][ticket_kind] -= num;
				if (there.ticket[i][ticket_kind] < 0)
					return false;
			}
			theRestTicket.insert(key, there);
			return true;
		}

		else {
			for (int i = loc1_pos; i < loc2_pos; ++i) {
				here.ticket[i][ticket_kind] -= num;
				if (here.ticket[i][ticket_kind] < 0)
					return false;
			}
			theRestTicket.update(key, here);
			return true;
		}

	}

	bool refund(int num, const mystring<20> &loc1, const mystring<20> &loc2, const mystring<10> &date, const mystring<20> &kind, bplus_tree<restTicket> &theRestTicket) {
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

		mystring<30> key = date + train_id;
		restTicket here, there;
		if (theRestTicket.search(key, &here) != 0) {
			for (int i = loc1_pos; i < loc2_pos; ++i) {
				there.ticket[i][ticket_kind] -= num;
				if (there.ticket[i][ticket_kind] < 0)
					return false;
			}
			theRestTicket.insert(key, there);
			return true;
		}
		else {
			for (int i = loc1_pos; i < loc2_pos; ++i) {
				here.ticket[i][ticket_kind] -= num;
				if (here.ticket[i][ticket_kind] < 0)
					return false;
			}
			theRestTicket.update(key, here);
			return true;
		}
	}

	string queryTicket(const mystring<20> &loc1, const mystring<20> &loc2, const mystring<10> &date, bplus_tree<restTicket> &theRestTicket) {
		string ans;
		string blank = " ";
		int loc1_pos, loc2_pos;
		for (loc1_pos = 0; loc1_pos < num_station - 1; ++loc1_pos) {
			if (sta[loc1_pos].name == loc1)
				break;
		}
		for (loc2_pos = loc1_pos + 1; loc2_pos < num_station; ++loc2_pos) {
			if (sta[loc2_pos].name == loc2)
				break;
		}
		ans += train_id.value() + blank + catalog + blank;
		int intdate = readdate(date);
		int date_from = intdate + sta[loc1_pos].day_out_offset;
		int date_to = intdate + sta[loc2_pos].day_in_offset;
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

		ans += loc1.value() + blank + ret_date_loc1.value() + blank + sta[loc1_pos].start.value() + blank + loc2.value() + blank + ret_date_loc2.value() + blank + sta[loc2_pos].arrive.value() + blank;
		mystring<30> key = date + train_id;
		restTicket here, there;
		if (theRestTicket.search(key, &here) != 0) {
			theRestTicket.insert(key, there);
			here = there;
		}
		for (int i = 0; i < num_price; ++i) {
			int ticket_num = 2000;
			ans += name_price[i].value() + blank;
			for (int j = loc1_pos; j < loc2_pos; ++j) {
				if (here.ticket[j][i] < ticket_num)
					ticket_num = here.ticket[j][i];
			}
			double price = 0;
			for (int j = loc1_pos + 1; j <= loc2_pos; ++j)
				price += sta[j].price[i];
			ans += to_string(ticket_num) + blank + to_string(price) + blank;
		}
		return ans;
	}

};

string add_train(const mystring<20> &id, const train& newtrain, bplus_tree<train> &thetrain)
{
	train pos;
	if (thetrain.search(id, &pos) == 0) {
		return "0";
	}

	thetrain.insert(id, newtrain);
	return "1";
}

string sale_train(const mystring<20> &id, bplus_tree<train> &thetrain, bplus_tree<char> &findtrain, bplus_tree<char> &loc)
{
	train pos;
	if (thetrain.search(id, &pos) != 0) {
		return "0";
	}

	if (pos.sale_train(findtrain, loc)) {
		thetrain.update(id, pos);
		return "1";
	}
	else return "0";
}

string query_train(const mystring<20> &id, bplus_tree<train> &thetrain)
{
	train pos;
	if (thetrain.search(id, &pos) != 0) {
		return "0";
	}
	return pos.query();
}

string delete_train(const mystring<20> &id, bplus_tree<train> &thetrain)
{
	train pos;
	if (thetrain.search(id, &pos) != 0) {
		return "0";
	}
	thetrain.remove(id);
	return "1";
}

string modify_train(const mystring<20> &id, const mystring<40> &na, const char &cat, int num_sta, int num_pri,
	const mystring<20>* name_pri, const train_station *s, bplus_tree<train> &thetrain)
{
	train pos;
	if (thetrain.search(id, &pos) != 0) {
		return "0";
	}

	if (pos.modify(na, cat, num_sta, num_pri, name_pri, s)) {
		thetrain.update(id, pos);
		return "1";
	}
	else return "0";

}

