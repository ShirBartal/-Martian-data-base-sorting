#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <csignal>
#include <regex>
#include <type_traits>
#include <set>
using namespace std;

enum class Criteria { first, last, ssn };
struct Martian {
	string fname_;
	string lname_;
	string SSN_;
	Martian(std::string fn, std::string ln, std::string ssn)
		: fname_(fn), lname_(ln), SSN_(ssn) { }
};

class MartianDatabase {
private:
	vector <Martian*> martianD;
	vector<Martian*>martianFirst;
	vector<Martian*>martianLast;
	vector<Martian*>martianSSN;
public:
	// creates a new empty Martian Database initialized with the
	// contents of filename.
	MartianDatabase(istream& infile);

	//// add item m to database, return true if successfully added.
	bool addMartian(const Martian &m);

	//// delete item with the ssn in database, return true if successfully 
	//// deleted.
	bool deleteMartian(string ssn);

	//// return true if the database does not contain any items, false
	//// otherwise.
	bool isEmpty();

	//// return true if the database is at its maximum storage, false
	//// otherwise.
	bool isFull();

	// prints the list of Martians, sorted by last name, sorted
	// by first name and by MSSN.
	void printMartians(Criteria type); //Criteria type

									   //// deletes all the dynamically created memory
	~MartianDatabase();
	//void print();
};

void printMenu();
bool sortStringFirst(Martian* a, Martian* b);
bool sortStringLast(Martian* a, Martian* b);
bool sortStringSsn(Martian* a, Martian* b);

int main() {
	string file1;
	cout << "Enter the name of the file. " << endl;
	cin >> file1;
	ifstream infile(file1);
	if (!infile)
		cout << "File couldn't open properly. " << endl;
	MartianDatabase dataBase(infile);
	if (dataBase.isEmpty()) {
		cout << "File is empty, quit." << endl;
		return 1;
	}
	int i = 0;
	while (i != 6) {
		printMenu();
		i = -1;
		bool firstTime = true;
		do {
			if (firstTime)
				firstTime = false;
			else
				cout << "Please enter an integer 1 to 6" << endl;
			cin >> i;
			if (!cin.good()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
		} while (!((i >= 1) && (i <= 6)));
		switch (i) {
		case 1: {
			string first, last, ssn;
			cout << "Please enter a first name:" << endl;
			cin >> first;
			cout << "Please enter a last name:" << endl;
			cin >> last;
			cout << "Please enter a SSN:" << endl;
			cin >> ssn;
			Martian newMartian(first, last, ssn);
			bool canIadd = dataBase.addMartian(newMartian);
			if (!canIadd)
				cout << "Can not add a new martian " << endl;
			break;
		}
		case 2: {
			string ssn;
			cout << "Please enter a ssn. " << endl;
			cin >> ssn;
			bool checkDelete = dataBase.deleteMartian(ssn);
			if (!checkDelete)
				cout << "SSN not found" << endl;
			break;
		}
		case 3: {
			dataBase.printMartians(Criteria::first);
			break;
		}
		case 4: {
			dataBase.printMartians(Criteria::last);
			break;
		}
		case 5: {
			dataBase.printMartians(Criteria::ssn);
			break;
		}
		case 6: {
			break;
		}
		default:
			break;
		}
	}
}

void printMenu() {
	cout << "	Matrian Database" << endl;
	cout << endl;
	cout << "	1. Add a martian" << endl;
	cout << "	2. Delete a martian" << endl;
	cout << "	3. Print by first name" << endl;
	cout << "	4. Print by last name" << endl;
	cout << "	5. Print by SSN" << endl;
	cout << "	6. Quit" << endl;
	cout << "	-----------------------" << endl;
}

MartianDatabase::~MartianDatabase() {
	for (auto it = martianD.begin(); it != martianD.end(); ++it) {
		// 'it' is of type vector<int>::iterator
		Martian*martian1 = *it;
		delete martian1;
	}
}

void MartianDatabase::printMartians(Criteria type) { //Criteria type
	if (type == Criteria::first) {
		sort(martianFirst.begin(), martianFirst.end(), &sortStringFirst);
		int i = 1;
		for (auto it = martianFirst.begin(); it != martianFirst.end(); ++it) {
			// 'it' is of type vector<Martian>::iterator
			Martian* element = *it;
			cout << i << " ";
			cout << element->fname_ << " ";
			cout << element->lname_ << " ";
			cout << element->SSN_ << endl;
			i++;
		}
	}
	if (type == Criteria::last) {
		sort(martianLast.begin(), martianLast.end(), sortStringLast);
		int i = 1;
		for (auto it = martianLast.begin(); it != martianLast.end(); ++it) {
			// 'it' is of type vector<Martian>::iterator
			Martian* element = *it;
			cout << i << " ";
			cout << element->fname_ << " ";
			cout << element->lname_ << " ";
			cout << element->SSN_ << endl;
			i++;
		}

	}
	if (type == Criteria::ssn) {
		sort(martianSSN.begin(), martianSSN.end(), sortStringSsn);
		int i = 1;
		for (auto it = martianSSN.begin(); it != martianSSN.end(); ++it) {
			// 'it' is of type vector<Martian>::iterator
			Martian* element = *it;
			cout << i << " ";
			cout << element->fname_ << ", ";
			cout << element->lname_ << ", ";
			cout << element->SSN_ << endl;
			i++;

		}

	}

}
bool sortStringFirst(Martian*a, Martian* b)
{
	return a->fname_ < b->fname_;
}
bool sortStringLast(Martian*a, Martian* b)
{
	return a->lname_ < b->lname_;
}
bool sortStringSsn(Martian*a, Martian* b)
{
	return a->SSN_ < b->SSN_;
}
bool MartianDatabase::addMartian(const Martian &m) {
	if (!isFull()) {
		Martian* a = new Martian(m.fname_, m.lname_, m.SSN_); //const?
		martianD.push_back(a);
		martianFirst.push_back(a);
		martianLast.push_back(a);
		martianSSN.push_back(a);
		return true;
	}
	return false;
}

bool MartianDatabase::isFull() {
	return martianD.size() == 100;
}

bool MartianDatabase::isEmpty() {
	return martianD.empty();
}


bool MartianDatabase::deleteMartian(string ssn) {
	bool check = false;
	for (auto it = martianFirst.begin(); it != martianFirst.end(); ++it) {
		Martian* element = *it;
		if (element->SSN_ == ssn) {
			martianFirst.erase(it);
			check = true;
			break;
		}
	}
	for (auto it = martianLast.begin(); it != martianLast.end(); ++it) {
		Martian* element = *it;
		if (element->SSN_ == ssn) {
			martianLast.erase(it);
			break;
		}
	}
	for (auto it = martianSSN.begin(); it != martianSSN.end(); ++it) {
		Martian* element = *it;
		if (element->SSN_ == ssn) {
			martianSSN.erase(it);
			break;
		}
	}
	for (auto it = martianD.begin(); it != martianD.end(); ++it) {
		Martian* element = *it;
		if (element->SSN_ == ssn) {
			martianD.erase(it);
			delete element;
			break;
		}
	}
	return check;
}


MartianDatabase::MartianDatabase(istream& infile) {
	if (!infile)
	{
		cout << "Error: Cannot open file!" << endl;
	}
	while (infile.peek() != EOF) {
		string firstName, lastName, ssn;
		infile >> firstName;
		infile >> lastName;
		infile >> ssn;
		Martian* martian = new Martian(firstName, lastName, ssn);
		martianD.push_back(martian);
		martianLast.push_back(martian);
		martianFirst.push_back(martian);
		martianSSN.push_back(martian);
	}
}



