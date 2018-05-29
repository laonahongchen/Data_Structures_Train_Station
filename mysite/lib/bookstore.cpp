#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <map>

using namespace std;

#define rc reinterpret_cast

const int N = 1e3;

enum CmdType {INVALIDCMDTYPE, EXIT, NULLCMDTYPE, SU, LOGOUT, USERADD, REGISTER, DELETE, PASSWD, SELECT, MODIFY, IMPORT, SHOW, SHOWFINANCE, BUY};
enum MdfType {NULLMDFTYPE, ISBN, NAME, AUTHOR, KEYWORD, PRICE,FINANCE};

bool readFinance(char *&it) {
	char *_it = it;
	string s;
	for (; *it != '\0' && *it == ' '; ++it); 
	for (; *it != '\0' && *it != ' '; s += *it, ++it);
	if (s == "finance") 
		return 1;
	it = _it;
	return 0;
}

CmdType readCmdType(char *&it) {
	string s;
	for (; *it != '\0' && *it == ' '; ++it); 
	for (; *it != '\0' && *it != ' '; s += *it, ++it);
	if (s == "su") return SU;
	if (s == "logout") return LOGOUT;
	if (s == "useradd") return USERADD;
	if (s == "register") return REGISTER;
	if (s == "delete") return DELETE;
	if (s == "passwd") return PASSWD;
	if (s == "select") return SELECT;
	if (s == "modify") return MODIFY;
	if (s == "import") return IMPORT;
	if (s == "show" && readFinance(it)) return SHOWFINANCE;
	if (s == "show") return SHOW;
	if (s == "buy") return BUY;
	if (s == "") return NULLCMDTYPE;
	if (s == "exit") return EXIT;
	return INVALIDCMDTYPE;
}

string readStr(char *&it) {
	string s;
	for (; *it != '\0' && *it == ' '; ++it); 
	for (; *it != '\0' && *it != ' '; s += *it, ++it);
	return s;
}

MdfType readMdfType(char *&it) {
	string s;
	for (; *it != '\0' && *it == ' '; ++it);
	for (; *it != '\0' && *it != '='; s += *it, ++it);
	++it;
	if (s == "-ISBN") return ISBN;
	if (s == "-name") return NAME;
	if (s == "-author") return AUTHOR;
	if (s == "-keyword") return KEYWORD;
	if (s == "-price") return PRICE;
	return NULLMDFTYPE;
}

int readInt(char *&it) {
	for (; *it != '\0' && *it == ' '; ++it); 
	bool flag = 0;
	int x = 0;
	for (; *it != '\0' && '0' <= *it && *it <= '9'; flag = 1, x = x * 10 + *it - '0', ++it);
	if (!flag) return -1;
	return x;
}

string readIsbn(char *&it) {
	for (; *it != '\0' && *it == ' '; ++it); 
	string s;
	for (; *it != '\0' && '0' <= *it && *it <= '9'; s += *it, ++it);
	return s;
}

string readBookStr(char *&it) {
	for (; *it != '\0' && *it != '"'; ++it); 
	++it;
	string s;
	for (; *it != '\0' && *it != '"'; s += *it, ++it);
	++it;
	return s;
}

void readKeyword(char *&it, vector<string> &t) {
	for (; *it != '\0' && *it != '"'; ++it); 
	t.clear();
	while (1) {
		string s;
		++it;
		for (; *it != '\0' && *it != '"' && *it != '|'; s += *it, ++it);
		t.push_back(s);
		if (*it == '"') 
			break;
	}
	++it;
}

double readDouble(char *&it) {
	for (; *it != '\0' && *it == ' '; ++it); 
	double x = 0;
	for (; *it != '\0' && '0' <= *it && *it <= '9'; ++it)
		x = x * 10 + *it - '0';
	if (*it == '.') {
		++it;
		for (double p = 0.1; *it != '\0' && '0' <= *it && *it <= '9'; ++it, p *= 0.1)
			x += p * (*it - '0');
	}
	return x;
}

void Clear(string file) {
	ofstream fout;
	fout.open(file.c_str());
	fout.close();
}

struct User {
	string id, pwd;
	int ac = -1;
	string name;

	User() {}
	User(string id, string pwd, int ac, string name = "") : id(id), pwd(pwd), ac(ac), name(name) { }
} user;

struct Book {
	string isbn, name, author;
	vector<string> keyword;
	double price = 0;
	int qn = 0;

	Book() {}
} book;

struct UserShelf {
	int tot;
	vector<User> data;
	map<string, int> hash;
	User cur;

	void Output(const string &file, const int &ac) {
		fstream fio;
		fio.open(file.c_str(), fstream::app);
		fio << ac << endl;
		fio.close();
	}

	void Output(const string &file, const string &s) {
		fstream fio;
		fio.open(file.c_str(), fstream::app);
		fio << s.length() << ' ' << s << endl;
		fio.close();
	}

	void Output(const string &file, const User &user) {
		Output(file, user.id);
		Output(file, user.pwd);
		Output(file, user.ac);
		Output(file, user.name);
	}

	void Cur(const string &file, const User &user) {
		ofstream fout;
		fout.open(file.c_str());
		fout.close();
		Output(file, 1);
		Output(file, user.id);
		Output(file, user.pwd);
		Output(file, user.ac);
		Output(file, user.name);
	}

	void init() {
		Clear("userShelf");
		Clear("userShelfCur");

		ofstream fout;
		fout.open("userShelf");
		fout << 1 << endl;
		fout.close();
		User root = User("root", "sjtu", 7);
		Output("userShelf", root);
		Cur("userShelfCur", root);
	}

	void read() {
		fstream fio;
		fio.open("userShelf");
		fio >> tot;
		for (int i = 1; i <= tot; ++i) {
			int len;
			User user;
			fio >> len;
			if (len) fio >> user.id;
			fio >> len;
			if (len) fio >> user.pwd;
			fio >> user.ac;
			fio >> len;
			if (len) fio >> user.name;
			data.push_back(user);
			hash[user.id] = i;
		}
		fio.close();

		fio.open("userShelfCur");
		int curTot = 0;
		fio >> curTot;
		if (!fio.eof() && curTot) {
			int len;
			fio >> len;
			if (len) fio >> cur.id;
			fio >> len;
			if (len) fio >> cur.pwd;
			fio >> cur.ac;
			fio >> len;
			if (len) fio >> cur.name;
		}
		fio.close();
	}

	void add(const User &user) {
		data.push_back(user);
	}

	void del(const string &id) {
		User user;
		int p = hash[id];
		if (!p) { cout << "Invalid" << endl; return; }
		data[p - 1] = user;
	}

	User find(const string &id) {
		User user;
		int p = hash[id];
		if (p)
			user = data[p - 1];
		return user;
	}

	void apply(const User &user) {
		int p = hash[user.id];
		data[p - 1] = user;
	}

	void write() {
		Clear("userShelf");
		Clear("userShelfCur");

		ofstream fout;
		fout.open("userShelf");
		tot = 0;
		for (auto i : data)
			if (~i.ac) ++tot;
		fout << tot << endl;
		fout.close();
		for (auto i : data)
			if (~i.ac) 
				Output("userShelf", i);
		Cur("userShelfCur", cur);

		tot = 0;
		data.clear();
		hash.clear();
		cur = User();
	}
} userShelf;

struct BookShelf {
	int tot;
	vector<Book> data;
	map<string, int> hash;
	Book cur;

	void Output(const string &file, const double &price) {
		fstream fio;
		fio.open(file.c_str(), fstream::app);
		fio << setiosflags(ios::fixed) << setprecision(6) << price << endl;
		fio.close();
	}

	void Output(const string &file, const int &qn) {
		fstream fio;
		fio.open(file.c_str(), fstream::app);
		fio << qn << endl;
		fio.close();
	}

	void Output(const string &file, const vector<string> &keyword) {
		fstream fio;
		fio.open(file.c_str(), fstream::app);
		int tot = keyword.size();
		fio << tot << endl;
		for (auto i : keyword)
			fio << i << endl;
		fio.close();
	}

	void Output(const string &file, const string &s) {
		fstream fio;
		fio.open(file.c_str(), fstream::app);
		fio << s.length() << endl;
		fio << s << endl;
		fio.close();
	}

	void Output(const string &file, const Book &book) {
		Output(file, book.isbn);
		Output(file, book.name);
		Output(file, book.author);
		Output(file, book.keyword);
		Output(file, book.price);
		Output(file, book.qn);
	}

	void Cur(const string &file, const Book &book) {
		ofstream fout;
		fout.open(file.c_str());
		fout.close();
		Output(file, 1);
		Output(file, book.isbn);
		Output(file, book.name);
		Output(file, book.author);
		Output(file, book.keyword);
		Output(file, book.price);
		Output(file, book.qn);
	}

	void init() {
		Clear("bookShelf");
		Clear("bookShelfCur");

		ofstream fout;
		fout.open("bookShelf");
		fout << 0 << endl;
		fout.close();
		Book book;
		Cur("bookShelfCur", book);
	}

	void read() {
		fstream fio;
		fio.open("bookShelf");
		fio >> tot;
		for (int i = 1; i <= tot; ++i) {
			int len;
			Book book;
			fio >> len;
			if (len) fio >> book.isbn;
			fio >> len;
			if (len) {
				static char ch[N];
				fio.getline(ch, N);
				fio.getline(ch, N);
				for (int i = 0; i < len; ++i)
					book.name += ch[i];
			}
			fio >> len;
			if (len) {
				static char ch[N];
				fio.getline(ch, N);
				fio.getline(ch, N);
				for (int i = 0; i < len; ++i)
					book.author += ch[i];
			}
			fio >> len;
			while (len--) {
				string s;
				fio >> s;
				book.keyword.push_back(s);
			}
			fio >> book.price;
			fio >> book.qn; 
			data.push_back(book);
			hash[book.isbn] = i;
		}
		fio.close();

		fio.open("bookShelfCur");
		int curTot = 0;
		fio >> curTot;
		if (!fio.eof() && curTot) {
			int len;
			fio >> len;
			if (len) fio >> cur.isbn;
			fio >> len;
			if (len) fio >> cur.name;
			fio >> len;
			if (len) fio >> cur.author;
			fio >> len;
			while (len--) {
				string s;
				fio >> s;
				cur.keyword.push_back(s);
			}
			fio >> cur.price;
			fio >> cur.qn; 
		}
		fio.close();
	}

	void add(const Book &book) {
		data.push_back(book);
		hash[book.isbn] = data.size();
	}

	void del(const string &isbn) {
		Book book;
		int p = hash[isbn];
		if (!p) { cout << "Invalid" << endl; return; }
		data[p - 1] = book;
	}

	Book find(const string &isbn) {
		Book book;
		int p = hash[isbn];
		if (p)
			book = data[p - 1];
		return book;
	}

	void apply(const Book &book) {
		int p = hash[book.isbn];
		if (p)
			data[p - 1] = book;
	}

	void write() {
		Clear("bookShelf");
		Clear("bookShelfCur");

		ofstream fout;
		fout.open("bookShelf");
		tot = 0;
		for (auto i : data)
			if (i.isbn != "")
				++tot;
		fout << tot << endl;
		fout.close();
		for (auto i : data)
			if (i.isbn != "")
				Output("bookShelf", i);
		Cur("bookShelfCur", cur);

		tot = 0;
		data.clear();
		hash.clear();
		cur = Book();
	}

	void Display(const Book &book) {
		cout << book.isbn << '\t';
		cout << book.name << '\t';
		cout << book.author << '\t';
		int col = book.keyword.size();
		for (auto i : book.keyword) {
			cout << i;
			if (--col)
				cout << '|';
		}
		cout << '\t';
		cout << setiosflags(ios::fixed) << setprecision(2) << book.price << '\t';
		cout << book.qn << "本" << endl;
	}

	void display() {
		for (auto i : hash) {
			int p = i.second;
			Display(data[p - 1]);
		}
	}

	void displayIsbn(string isbn) {
		for (auto i : hash) {
			int p = i.second;
			if (i.first == isbn)
				Display(data[p - 1]);
		}
	}

	void displayName(string name) {
		for (auto i : hash) {
			int p = i.second;
			if (data[p - 1].name == name)
				Display(data[p - 1]);
		}
	}

	void displayAuthor(string author) {
		for (auto i : hash) {
			int p = i.second;
			if (data[p - 1].author == author)
				Display(data[p - 1]);
		}
	}

	void displayKeyword(string keyword) {
		for (auto i : hash) {
			int p = i.second;
			bool flag = 0;
			for (auto j : data[p - 1].keyword) 
				if (j == keyword) {
					flag = 1; 
					break;
				}
			if (flag)
				Display(data[p - 1]);
		}
	}
} bookShelf;

struct Finance {
	vector<double> data;

	void init() {
		ofstream fout;
		fout.open("finance");
		fout << 0 << endl;
		fout.close();
	}

	void read() {
		ifstream fin;
		fin.open("finance");
		int tot;
		fin >> tot;
		while (tot--) {
			double x;
			fin >> x;
			data.push_back(x);
		}
		fin.close();
	}

	void write() {
		ofstream fout;
		fout.open("finance");
		fout << data.size() << endl;
		for (auto i : data) 
			fout << setiosflags(ios::fixed) << setprecision(6) << i << endl;
		fout.close();
		data.clear();
	}

	void add(double x) {
		data.push_back(x);
	}

	void display() {
		double pos = 0, neg = 0;
		for (auto i : data) {
			if (i > 0) 
				pos += i;
			else
				neg -= i;
		}
		cout << "+ " << setiosflags(ios::fixed) << setprecision(2) << pos;
		cout << " - " << setiosflags(ios::fixed) << setprecision(2) << neg << endl;
	}

	void display(int tm) {
		if (tm > data.size()) { cout << "Invalid" << endl; return; }
		auto it = data.rbegin();
		double pos = 0, neg = 0;
		for (; tm; --tm, ++it) {
			if (*it > 0)
				pos += *it;
			else
				neg -= *it;
		}
		cout << "+ " << setiosflags(ios::fixed) << setprecision(2) << pos;
		cout << " - " << setiosflags(ios::fixed) << setprecision(2) << neg << endl;
	}
} finance;

void init() {
	ifstream fin;
	fin.open("init");
	if (!fin) {
		fin.close();
		ofstream fout;
		fout.open("init");
		fout.close();
		userShelf.init();
		bookShelf.init();
		finance.init();
	}
}

void Read() {
	userShelf.read();
	bookShelf.read();
	finance.read();
}

void Write() {
	userShelf.write(); 
	bookShelf.write();
	finance.write();
}

void Throw() {
	cout << "Invalid" << endl;
	Write();
}

void solve() {
	ifstream fin;
	fin.open("command.txt");

	while (1) {
		char s[N], *it = s;
		fin.getline(s, N);
		if (fin.eof()) 
			break;
		Read();
		int cmdType = readCmdType(it);
		if (cmdType == INVALIDCMDTYPE) { Throw(); continue; }
		else if (cmdType == EXIT)
			break;
		else if (cmdType == SU) {
			string id = readStr(it);
			User user = userShelf.find(id);
			if (user.ac == -1) { Throw(); continue; }
			if (userShelf.cur.ac > user.ac) 
				userShelf.cur = user;
			else {
				string pwd = readStr(it);
				if (pwd != user.pwd || pwd == "") { Throw(); continue; }
				userShelf.cur = user;
			}
		}
		else if (cmdType == LOGOUT) {
			if (userShelf.cur.ac == -1) { Throw(); continue; }
			User user;
			userShelf.cur = user;
		}
		else if (cmdType == USERADD) {
			if (userShelf.cur.ac <= 1) { Throw(); continue; }
			User user;
			user.id = readStr(it);
			if (~userShelf.find(user.id).ac) { Throw(); continue; }
			user.pwd = readStr(it);
			user.ac = readInt(it);
			user.name = readStr(it);
			if (!user.ac || user.ac >= userShelf.cur.ac || readStr(it) != "" || user.name == "") { Throw(); continue; }
			userShelf.add(user);
		}
		else if (cmdType == REGISTER) {
			User user;
			user.id = readStr(it);
			user.pwd = readStr(it);
			user.ac = 1;
			user.name = readStr(it);
			if (readStr(it) != "" || user.name == "") { Throw(); continue; }
			userShelf.add(user);
		}
		else if (cmdType == DELETE) {
			if (userShelf.cur.ac != 7) { Throw(); continue; }
			string id = readStr(it);
			if (id == "") { Throw(); continue; }
			userShelf.del(id);
		}
		else if (cmdType == PASSWD) {
			if (userShelf.cur.ac == -1) { Throw(); continue; }
			string id = readStr(it);
			User user = userShelf.find(id);
			if (user.ac == -1) { Throw(); continue; }
			if (userShelf.cur.id == "root") {
				user.pwd = readStr(it);
				if (user.pwd == "") { Throw(); continue; }
				userShelf.apply(user);
			}
			else {
				if (user.pwd == readStr(it)) {
					user.pwd = readStr(it);
					if (user.pwd == "") { Throw(); continue; }
					userShelf.apply(user);
				}
				else { Throw(); continue; }
			}
		}
		else if (cmdType == SELECT) {
			if (userShelf.cur.ac < 3) { Throw(); continue; }
			string isbn = readIsbn(it);
			bookShelf.cur = bookShelf.find(isbn);
			if (bookShelf.cur.isbn == "") {
				bookShelf.cur.isbn = isbn;
				bookShelf.add(bookShelf.cur);
			}
		}
		else if (cmdType == MODIFY) {
			if (userShelf.cur.ac < 3) { Throw(); continue; }
			if (bookShelf.cur.isbn == "") { Throw(); continue; }
			bool flag = 0;
			while (1) {
				int mdfType = readMdfType(it);
				if (mdfType == NULLMDFTYPE)
					break;
				else {
					if (mdfType == ISBN) {
						string isbn = readIsbn(it);
						if (isbn != bookShelf.cur.isbn) {
							if (bookShelf.find(isbn).isbn != "") { 
								bookShelf.del(bookShelf.cur.isbn);
								flag = 1; break;
							}
							bookShelf.del(bookShelf.cur.isbn);
							bookShelf.cur.isbn = isbn;
							bookShelf.add(bookShelf.cur);
						}
					}
					else if (mdfType == NAME) 
						bookShelf.cur.name = readBookStr(it);
					else if (mdfType == AUTHOR) 
						bookShelf.cur.author = readBookStr(it);
					else if (mdfType == KEYWORD) 
						readKeyword(it, bookShelf.cur.keyword);
					else if (mdfType == PRICE) 
						bookShelf.cur.price = readDouble(it);
				}
			}
			if (flag) { Throw(); continue; }
			bookShelf.apply(bookShelf.cur);
		}
		else if (cmdType == IMPORT) {
			if (userShelf.cur.ac < 3) { Throw(); continue; }
			if (bookShelf.cur.isbn == "") { Throw(); continue; }
			int qn = readInt(it);
			double cost = readDouble(it);
			bookShelf.cur.qn += qn;
			bookShelf.apply(bookShelf.cur);
			finance.add(-cost);
		}
		else if (cmdType == SHOW) {
			if (userShelf.cur.ac == -1) { Throw(); continue; }
			int showType = readMdfType(it);
			if (showType == NULLMDFTYPE)
				bookShelf.display();
			else if (showType == ISBN) {
				string isbn = readIsbn(it);
				bookShelf.displayIsbn(isbn);
			}
			else if (showType == NAME) {
				string name = readBookStr(it);
				bookShelf.displayName(name);
			}
			else if (showType == AUTHOR) {
				string author = readBookStr(it);
				bookShelf.displayAuthor(author);
			}
			else if (showType == KEYWORD) {
				string keyword = readBookStr(it);
				bookShelf.displayKeyword(keyword);
			}
		}
		else if (cmdType == SHOWFINANCE) {
			if (userShelf.cur.ac < 7) { Throw(); continue; }
			int tm = readInt(it);
			if (tm == -1)
				finance.display();
			else 
				finance.display(tm);
		}
		else if (cmdType == BUY) {
			if (userShelf.cur.ac == -1) { Throw(); continue; }
			string isbn = readIsbn(it);
			int qn = readInt(it);
			Book book = bookShelf.find(isbn);
			if (book.qn < qn) { Throw(); continue; }
			book.qn -= qn;
			finance.add(qn * book.price);
			bookShelf.apply(book);
		}
		Write();
	} 
} 

int main() {
	init();
	solve();
	return 0;
}
