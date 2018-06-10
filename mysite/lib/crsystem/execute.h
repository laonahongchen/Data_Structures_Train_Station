#pragma once
#include <fstream>
#include "predefined.h"
#include "user.h"
#include "train.h"
#include "ticket.h"
#include "bptree.h"
#include "string.h"

#define user_block (sizeof(char) * 120 + sizeof(int))
using namespace std;

int user_number = 0;
fstream fout;

string file_register(User &user) {
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
	return user.id.value();
}

string file_login(const int cintid, const mystring<20> &cpassword) {
	if (cintid < 2018 || cintid >= 2018 + user_number) {
		return "0";
	}
	fout.seekg((cintid - 2018) * user_block + sizeof(char) * 60);
	char charpassword[21];
	fout.read(reinterpret_cast<char *>(&charpassword), sizeof(char) * 20);
	charpassword[20] = '\0';
	mystring<20> password(charpassword);
	fout.seekg(0, ios::beg);
	for (int i = 0; i < cpassword.len; ++i) {
		if (password[i] != cpassword[i]) {
			return "0";
		}
	}
	if (password[cpassword.len] == '\0') return "1";
	else return "0";
}

string file_query_profile(const int cintid) {
	if (cintid < 2018 || cintid >= 2018 + user_number) {
		return "0";
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
	mystring<40> password(charpassword);
	mystring<20> email(charemail);
	mystring<20> phone(charphone);
	string blank = " ";
	char priv = privilege + '0';
	fout.seekg(0, ios::beg);
	return name.value() + blank + password.value() + blank + email.value() + blank + phone.value() + blank + priv;
}

string file_modify_profile(const int cintid, const mystring<40> &cname, const mystring<20> &cpassword, const mystring<20> &cemail, const mystring<20> &cphone) {
	if (cintid < 2018 || cintid >= 2018 + user_number) {
		return "0";
	}
	fout.seekp((cintid - 2018) * user_block + sizeof(char) * 20);
	fout.write(reinterpret_cast<const char *>(cname.str), sizeof(char) * 40);
	fout.write(reinterpret_cast<const char *>(cpassword.str), sizeof(char) * 20);
	fout.write(reinterpret_cast<const char *>(cemail.str), sizeof(char) * 20);
	fout.write(reinterpret_cast<const char *>(cphone.str), sizeof(char) * 20);
	fout.seekp(0, ios::beg);
	return "1";
}

string file_modify_priviledge(const int &id1, const int &id2, const int &privilege) {
	if (id1 < 2018 || id1 >= 2018 + user_number || id2 < 2018 || id2 >= 2018 + user_number) {
		return "0";
	}
	fout.seekg((id1 - 2018) * user_block + sizeof(char) * 120);
	int privilege1, privilege2;
	fout.read(reinterpret_cast<char *>(&privilege1), sizeof(int));
	fout.seekg(0, ios::beg);
	fout.seekg((id2 - 2018) * user_block + sizeof(char) * 120);
	fout.read(reinterpret_cast<char *>(&privilege2), sizeof(int));
	if (privilege1 == 2 && privilege2 != 2) {
		fout.seekp((id2 - 2018) * user_block + sizeof(char) * 120);
		fout.write(reinterpret_cast<const char *>(&privilege), sizeof(int));
		fout.seekg(0, ios::beg);
		fout.seekp(0, ios::beg);
		return "1";
	}
	else if (privilege1 == 2 && privilege2 == 2 && privilege == 2) {
		fout.seekg(0, ios::beg);
		fout.seekp(0, ios::beg);
		return "1";
	}
	else {
		fout.seekg(0, ios::beg);
		fout.seekp(0, ios::beg);
		return "0";
	}
}


string file_buy_ticket(const mystring<20> &id, const int &intid, const int &num, const mystring<20> &train_id,
	const mystring<20> &loc1, const mystring<20> &loc2, const mystring<10> &date,
	const int &intdate, const mystring<20> &ticket_kind, bplus_tree<train> &thetrain,
	bplus_tree<ticket> &theticket, bplus_tree<char> &find_ticket, bplus_tree<restTicket> &theRestTicket)
{
	if (intid < 2018 || intid >= 2018 + user_number) {
		return "0";
	}
	train pos;
	ticket findticket;
	if (thetrain.search(train_id, &pos) != 0) {
		return "0";
	}
	if (pos.buy(num, loc1, loc2, date, ticket_kind, theRestTicket)) {
		mystring<90> key = id + date + train_id + loc1 + loc2;
		if (theticket.search(key, &findticket) != 0) {
			int loc1_pos, loc2_pos;
			for (loc1_pos = 0; loc1_pos < pos.num_station - 1; ++loc1_pos) {
				if (pos.sta[loc1_pos].name == loc1)
					break;
			}
			for (loc2_pos = loc1_pos + 1; loc2_pos < pos.num_station; ++loc2_pos) {
				if (pos.sta[loc2_pos].name == loc2)
					break;
			}
			int date_from = intdate + pos.sta[loc1_pos].day_out_offset;
			int date_to = intdate + pos.sta[loc2_pos].day_in_offset;
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
			ticket newticket(train_id, loc1, loc2, date, pos.catalog, ret_date_loc1, pos.sta[loc1_pos].start, ret_date_loc2, pos.sta[loc2_pos].arrive, pos.num_price);
			for (int i = 0; i < newticket.num_kind; ++i) {
				newticket.ticket_kind[i] = pos.name_price[i];
				for (int j = loc1_pos; j <= loc2_pos; ++j) {
					newticket.price_ticket_kind[i] += pos.sta[j].price[i];
				}
			}
			for (int i = 0; i < newticket.num_kind; ++i) {
				if (newticket.ticket_kind[i] == ticket_kind) {
					newticket.num_ticket_kind[i] += num;
					break;
				}
			}
			theticket.insert(key, newticket);
			find_ticket.insert(key, newticket.catalog);
			return "1";
		}
		else {
			for (int i = 0; i < findticket.num_kind; ++i) {
				if (findticket.ticket_kind[i] == ticket_kind) {
					findticket.num_ticket_kind[i] += num;
					theticket.update(key, findticket);
					return "1";
				}
			}
		}
	}
	else {
		return "0";
	}
}

string file_query_order(const mystring<20> &id, const int &intid, const mystring<10> &date, const mystring<10> &catalog,
	bplus_tree<ticket> &theticket, bplus_tree<char> &find_ticket) {
	if (intid < 2018 || intid >= 2018 + user_number) {
		return "0";
	}
	mystring<60> left, right;
	for (int i = 0; i < 60; ++i) {
		left += ' ';
		right += 'z';
	}
	mystring<90> leftkey = id + date + left;
	mystring<90> rightkey = id + date + right;
	sjtu::vector<bplus_tree<char>::record_t> key_sequence;
	sjtu::vector<bplus_tree<char>::record_t> true_sequence;

	find_ticket.search_range(leftkey, rightkey, key_sequence);
	for (int i = 0; i < key_sequence.size(); ++i) {
		for (int j = 0; j < catalog.len; ++j) {
			if (key_sequence[i].value == catalog.str[j]) {
				true_sequence.push_back(key_sequence[i]);
				break;
			}
		}
	}
	if (true_sequence.empty()) {
		return "0";
	}
	string ans;
	for (int i = 0; i < true_sequence.size(); ++i) {
		mystring<90> findkey;
		for (int j = 0; j < 90; ++j) {
			findkey += true_sequence[i].key.k[j];
		}
		ticket pos;
		theticket.search(findkey, &pos);
		ans += pos.display();
		if (i != true_sequence.size() - 1) ans += " | ";
	}
	return ans;
}

string file_refund_ticket(const int &num, const mystring<20> &id, const int &intid, const mystring<20> &train_id,
	const mystring<20> &loc1, const mystring<20> &loc2, const mystring<10> &date, const int &intdate,
	const mystring<20> &ticket_kind, bplus_tree<ticket> &theticket, bplus_tree<train> &thetrain, bplus_tree<restTicket> &theRestTicket) {
	if (intid < 2018 || intid >= 2018 + user_number) {
		return "0";
	}
	train postrain;
	ticket pos;
	if (thetrain.search(train_id, &postrain) != 0) {
		return "0";
	}
	mystring<90> key = id + date + train_id + loc1 + loc2;
	if (theticket.search(key, &pos) != 0) {
		return "0";
	}
	if (postrain.refund(num, loc1, loc2, date, ticket_kind, theRestTicket)) {
		for (int i = 0; i < pos.num_kind; ++i) {
			if (pos.ticket_kind[i] == ticket_kind) {
				if (pos.num_ticket_kind[i] < num) {
					return "0";
				}
				else {
					pos.num_ticket_kind[i] -= num;
					theticket.update(key, pos);
					return "1";
				}
			}
		}
	}
	else return "0";
}
