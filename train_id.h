#include "model_b_plus_tree.h"
#include "newstring.h"

using namespace std;

struct train_station {
	mystring<40> name;
	mystring<5> arrive;
	mystring<5> start;
	mystring<5> stopover;
	int num_price;
	double price[5];

	train_station(){}

	train_station(const mystring<40> na, const mystring<5> arr, const mystring<5> sta, const mystring<5> sto, size_t num_pri,
		const double *pri): name(na), arrive(arr), start(sta), stopover(sto), num_price(num_pri)
	{
		for (int i = 0; i < num_price; ++i)
			price[i] = pri[i];
	}
};

class train{
	mystring<20> train_id;
	mystring<40> name;
	mystring<10> catalog;
	mystring<40> name_price[5];
	bool besaled;
	int num_station;
	int num_price;
	train_station sta[60];

public:
	train(): besaled(false), num_station(0), num_price(0){}


	train(const mystring<20> &id, const mystring<40> na, const mystring<10> cat, size_t num_sta, size_t num_pri, const mystring<40>* name_pri,
		const train_station *s) :
		num_station(num_sta), num_price(num_pri), train_id(id), name(na), catalog(cat)
	{
		for (int i = 0; i < num_price; ++i)
			name_price[i] = name_pri[i];
		besaled = false;

		for (int i = 0; i < num_station; ++i) {
			sta[i].arrive = s[i].arrive;
			sta[i].start = s[i].start;
			sta[i].stopover = s[i].stopover;
			sta[i].name = s[i].name;
			sta[i].num_price = s[i].num_price;
			for (int j = 0; j < num_price; ++j)
				sta[i].price[j] = s[i].price[j];
		}
	}


	void sale_train() {
		if (besaled) {
			cout << 0 << '\n';
			return;
		}
		besaled = true;
		cout << 1 << '\n';
		return;
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
				cout << ' ' << ("￥") << sta[i].price[j];
			cout << '\n';
		}
	}

	/*void delete_train() {
		if (besaled)
			cout << 0 << '\n';
		else {
			cout << 1 << '\n';
		}
	}*/

	bool modify(const mystring<40> na, const mystring<10> cat, size_t num_sta, size_t num_pri, const mystring<40>* name_pri,
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

		for (int i = 0; i < num_station; ++i) {
			sta[i].arrive = s[i].arrive;
			sta[i].start = s[i].start;
			sta[i].stopover = s[i].stopover;
			sta[i].name = s[i].name;
			sta[i].num_price = num_price;
			for (int j = 0; j < num_price; ++j)
				sta[i].price[j] = s[i].price[j];
		}

		cout << 1 << '\n';
		return true;
	}
};

void addtrain(const mystring<20> &id, const train& newtrain, bplus_tree<train> thetrain)
{
	train pos;
	if (thetrain.search(id, &pos) == 0) {
		cout << 0 << '\n';
		return;
	}

	thetrain.insert(id, newtrain);
	cout << 1 << '\n';
}

void saletrain(const mystring<20> &id, bplus_tree<train> thetrain)
{
	train pos;
	if (thetrain.search(id, &pos) != 0) {
		cout << 0 << '\n';
		return;
	}

	pos.sale_train();
	thetrain.update(id, pos);
}

void querytrain(const mystring<20> &id, bplus_tree<train> thetrain)
{
	train pos;
	if (thetrain.search(id, &pos) != 0) {
		cout << 0 << '\n';
		return;
	}

	pos.query();
}

void deletetrain(const mystring<20> &id, bplus_tree<train> thetrain)
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

void modifytrain(const mystring<20> &id, const mystring<40> &na, const mystring<10> &cat, size_t num_sta, size_t num_pri, 
	const mystring<40>* name_pri, const train_station *s, bplus_tree<train> thetrain)
{
	train pos;
	if (thetrain.search(id, &pos) != 0) {
		cout << 0 << '\n';
		return;
	}

	if (pos.modify(na, cat, num_sta, num_pri, name_pri, s))
		thetrain.update(id, pos);

}

enum TRAIN_TYPE {ADD, SALE, QUERY, DELETE, MODIFY, INVALID_USERTYPE, EXIT};

TRAIN_TYPE read_TYPE(char* &it) {

	mystring<30> token;
	for (; *it != '\0' && *it == ' '; ++it);
	for (; *it != '\0' && *it != ' '; token += *it, ++it);

	if (token == "add_train") return ADD;

	if (token == "sale_train") return SALE;

	if (token == "query_train") return QUERY;

	if (token == "delete_train") return DELETE;

	if (token == "modify_train") return MODIFY;

	if (token == "exit") return EXIT;

	return INVALID_USERTYPE;
}

mystring<20> read_id(char* &it) {

	mystring<20> token;

	for (; *it != '\0' && *it == ' '; ++it);
	for (; *it != '\0' && *it != ' '; token += *it, ++it);

	return token;
}

mystring<40> read_name(char* &it) {

	mystring<40> token;

	for (; *it != '\0' && *it == ' '; ++it);
	for (; *it != '\0' && *it != ' '; token += *it, ++it);

	return token;
}

mystring<10> read_catalog(char* &it) {

	mystring<10> token;

	for (; *it != '\0' && *it == ' '; ++it);
	for (; *it != '\0' && *it != ' '; token += *it, ++it);

	return token;
}

mystring<5> read_time(char *&it) {
	mystring<5> token;

	for (; *it != '\0' && *it == ' '; ++it);
	for (; *it != '\0' && *it != ' '; token += *it, ++it);

	return token;
}

int read_num(char*&it){
	int sum = 0;
	for (; *it != '\0' && *it == ' '; ++it);
	for (; *it != '\0' && *it != ' '; ++it) sum = sum * 10 + *it - '0';
	return sum;
}

double read_price(char *&it) {
	double sum = 0;
	for (; *it != '\0' && *it == ' '; ++it);
	for (; !(*it >= '0' && *it <= '9'); ++it);
	for (; *it != '\0' && *it != ' ' && *it != '.'; ++it) sum = sum * 10 + *it - '0';
	if (*it == '.') {
		double ad;
		++it;
		int cnt = 1;
		for (; *it != '\0' && *it != ' '; ++it, ++cnt) {
			ad = (*it - '0') / pow(10, cnt);
			sum += ad;
		}
	}
	return sum;
}
