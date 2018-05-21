#ifndef TICKET
#define TICKET

#include "model_b_plus_tree.h"
#include "train.h"
#include "newstring.h"

struct timer {
	int day_off;
	mystring<5> time;

	timer() {}

	timer(int x):day_off(x){}

	timer(int x, const mystring<5> &s) :day_off(x), time(s) {}

	timer(const timer &o) :day_off(o.day_off), time(o.time) {}

	bool operator < (const timer &o) {
		if (day_off < o.day_off) return true;
		if (day_off == o.day_off && time < o.time) 
			return true;
		
		return false;
	}
};

timer operator - (const timer & s, const timer &o) {
	timer ret;
	int day_off = s.day_off - o.day_off;
	int hour1 = 10 * (s.time[0] - '0') + s.time[1] - '0';
	int hour2 = 10 * (o.time[0] - '0') + o.time[1] - '0';
	int minute1 = 10 * (s.time[3] - '0') + s.time[4] - '0';
	int minute2 = 10 * (o.time[3] - '0') + o.time[4] - '0';
	int minute = minute1 - minute2;
	int hour = hour1 - hour2;
	if (minute < 0) {
		minute += 60;
		hour--;
	}
	if (hour < 0) {
		hour += 24;
		day_off--;
	}
	ret.day_off = day_off;
	ret.time += hour / 10 + '0';
	ret.time += hour % 10 + '0';
	ret.time += ':';
	ret.time += minute / 10 + '0';
	ret.time += minute % 10 + '0';
	return ret;
}


void query_ticket(const mystring<40> &loc1, const mystring<40> &loc2, int date, 
	const mystring<10> &catalog, bplus_tree<char> &findtrain, bplus_tree<train> &thetrains)
{
	mystring<20> left, right;
	for (int i = 0; i < 20; ++i) {
		left+= ' ';
		right+= 'z';
	}
	mystring<100> leftkey = loc1 + loc2 + left;
	mystring<100> rightkey = loc1 + loc2 + right;
	sjtu::vector<bplus_tree<char>::record_t> trainid_sequence;
	sjtu::vector<bplus_tree<char>::record_t> true_sequence;
	
	findtrain.search_range(leftkey, rightkey, trainid_sequence);
	for (int i = 0; i < trainid_sequence.size(); ++i) {
		for (int j = 0; j < catalog.len; ++j) {
			if (trainid_sequence[i].value == catalog.str[j]) {
				true_sequence.push_back(trainid_sequence[i]);
				break;
			}
		}
	}
	
	if (true_sequence.empty()) {
		cout << 0 << '\n';
		return;
	}
	
	cout << true_sequence.size() << '\n';

	for (int i = 0; i < true_sequence.size(); ++i) {
		mystring<20> train_id;
		for (int j = 0; j < 20; ++j) {
			if(true_sequence[i].key.k[80 + j] == ' ')
				break;
			train_id += true_sequence[i].key.k[80 + j];
		}
		train pos;
		if (thetrains.search(train_id, &pos) == 0)
			pos.queryTicket(loc1, loc2, date);
	}

}

void query_transfer(const mystring<40> &loc1, const mystring<40> &loc2, int date, const mystring<10> &catalog,
	bplus_tree<char> &findtrain, bplus_tree<train> &thetrains, bplus_tree<char> &location)
{
	off_t offset = location.meta.first_leaf_offset;
	
	mystring<100> leftkey, rightkey;

	mystring<20> left, right;
	for (int i = 0; i < 20; ++i) {
		left += ' ';
		right += 'z';
	}

	//def_finally_ret
	bool flag = false;
	mystring<40> last_loc_mid;
	train last_train_one, last_train_two;
	timer one_arrive, two_start;
	timer wait_timer(1000);

	for (int block = 0; block < location.meta.leaf_node_num; ++block) {

		bplus_tree<char>::leaf_node_t leaf;
		location.map(&leaf, offset);


		for (int pos = 0; pos < leaf.num; ++pos) {

			mystring<40> loc_mid;

			//def_loc_mid
			for (int i = 0; i < 40; ++i) {
				if (leaf.children[pos].key.k[i] == ' ')
					break;
				loc_mid += leaf.children[pos].key.k[i];
			}

			if (loc_mid == loc1 || loc_mid == loc2)
				continue;

			sjtu::vector<bplus_tree<char>::record_t> trainid_sequence;
			sjtu::vector<bplus_tree<char>::record_t> true_sequence_one, true_sequence_two;
			
			//get_train_sequence_one
			leftkey = loc1 + loc_mid + left;
			rightkey = loc1 + loc_mid + right;
			findtrain.search_range(leftkey, rightkey, trainid_sequence);
			for (int i = 0; i < trainid_sequence.size(); ++i) {
				for (int j = 0; j < catalog.len; ++j) {
					if (trainid_sequence[i].value == catalog.str[j]) {
						true_sequence_one.push_back(trainid_sequence[i]);
						break;
					}
				}
			}

			trainid_sequence.clear();

			//get_train_sequence_two
			leftkey = loc_mid + loc2 + left;
			rightkey = loc_mid + loc2 + right;
			findtrain.search_range(leftkey, rightkey, trainid_sequence);
			for (int i = 0; i < trainid_sequence.size(); ++i) {
				for (int j = 0; j < catalog.len; ++j) {
					if (trainid_sequence[i].value == catalog.str[j]) {
						true_sequence_two.push_back(trainid_sequence[i]);
						break;
					}
				}
			}

			//inable -> next_loc_mid
			if (true_sequence_one.empty() || true_sequence_two.empty())
				continue;
			
			for (int i = 0; i < true_sequence_one.size(); ++i) {

				mystring<20> train_id_one;
				for (int j = 0; j < 20; ++j) {
					if (true_sequence_one[i].key.k[80 + j] == ' ')
						break;
					train_id_one += true_sequence_one[i].key.k[80 + j];
				}
				train train_one;
				if (thetrains.search(train_id_one, &train_one) != 0)
					continue;

				for (int j = 0; j < train_one.num_station; ++j){
					if (train_one.sta[j].name == loc_mid) {
						one_arrive.time = train_one.sta[j].arrive;
						one_arrive.day_off = train_one.sta[j].day_in_offset;
						break;
					}
				}
				
				for (int j = 0; j < true_sequence_two.size(); ++j) {
					mystring<20> train_id_two;
					for (int k = 0; k < 20; ++k) {
						if (true_sequence_two[j].key.k[80 + k] == ' ')
							break;
						train_id_two += true_sequence_two[j].key.k[80 + k];
					}
					train train_two;
					if (thetrains.search(train_id_two, &train_two) != 0)
						continue;

					for (int k = 0; k < train_one.num_station; ++k) {
						if (train_one.sta[k].name == loc_mid) {
							two_start.time = train_two.sta[k].start;
							two_start.day_off = train_two.sta[k].day_out_offset;
							break;
						}
					}

					if (two_start < one_arrive)
						continue;
					timer tmp = two_start - one_arrive;
					if (wait_timer < tmp)
						continue;
					last_train_one = train_one;
					last_train_two = train_two;
					last_loc_mid = loc_mid;
					flag = true;
				}
			}

		}
		offset = leaf.next;
	}

	if (!flag) {
		cout << -1 << '\n';
		return;
	}

	last_train_one.queryTicket(loc1, last_loc_mid, date);
	last_train_two.queryTicket(last_loc_mid, loc2, date);

}


#endif // !TICKET
