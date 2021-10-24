#include <iostream>
#include <string>

class Account {
public:
	int getID() const {
		return ID;
	}
	double getBalance() const {
		return balance;
	}
	double getInterest() const {
		return interest;
	}

	void setID(int _ID) {
		if (_ID >= 10000 && _ID <= 99999) {
			ID = _ID;
		}
		else {
			/*throw_out_of_range_fmt("Account::setID: _ID (which is %d) must be between"
				" 10,000 and 99,999.", _ID);*/
			std::cout << "Account::setID: _ID (which is " << _ID << ") must be between"
				<< " 10,000 and 99,999. Try again: ";
			std::cin >> _ID;
			setID(_ID);
		}
	}
	void setBalance(double _balance) {
		balance = _balance;
	}
	void setInterest(double _interest) {
		if (_interest >= 0 && _interest <= 1) {
			interest = _interest;
		}
		else {
			std::cout << "Account::setInterest: _interest (which is " << _interest << ") must be between"
				<< "0 and 1. Try again: ";
			std::cin >> _interest;
			setInterest(_interest);
		}
	}
	double calculateInterest() {
		return balance * interest;
	}
private:
	int ID = 0;
	double balance = 0.;
	double interest = 0.;
};

class VetPatient {
public:
	VetPatient() = default;
	VetPatient(std::string _name, std::string _species, int _age) : name(_name), species(_species), age(_age) {}
	VetPatient(std::string _name, std::string _species) : name(_name), species(_species) {}
	VetPatient(std::string _species, int _age) : species(_species), age(_age) {}

	std::string getName() {
		return name;
	}
	std::string getSpecies() {
		return species;
	}
	int getAge() {
		return age;
	}

	void setName(std::string _name) {
		name = _name;
	}
	void setSpecies(std::string _species) {
		species = _species;
	}
	void setAge(int _age) {
		age = _age;
	}
private:
	std::string name, species;
	int age = 0;
};

class Event;

class Time {
private:
	int hours;
	int minutes;
	int seconds;

public:
	int getHours() { return hours; }
	int getMinutes() { return minutes; }
	int getSeconds() { return seconds; }

	void setHours(int h) { hours = h; }
	void setMinutes(int m) { minutes = m; }
	void setSeconds(int s) { seconds = s; }

	Time() { }

	Time(int h, int m, int s) {
		hours = h;
		minutes = m;
		seconds = s;
	}
	friend std::istream &operator>>(std::istream &os, Time &time);
};

class Event {
public:
	const Time &getStart() const {
		return start;
	}
	const Time &getEnd() const {
		return end;
	}

	void setStart(const Time _start) {
		start = _start;
	}
	void setEnd(const Time _end) {
		end = _end;
	}
private:
	Time start, end;
};

std::istream &operator>>(std::istream &is, Time &time) {
	std::cout << "Enter hours: "; std::cin >> time.hours;
	std::cout << "Enter minutes: "; std::cin >> time.minutes;
	std::cout << "Enter seconds: "; std::cin >> time.seconds;
	return is;
}

void ReverseArrayByPtr(int *src, int *dest, const int SIZE);

int main() {
	/*Time time;
	std::cin >> time;  /// just uncomment this to test it lol
	Event event;
	event.setStart(time);*/

	int a1[5] = { 1, 2, 3, 4, 5 }, a2[5];
	ReverseArrayByPtr(a1, a2, 5);
	for (auto i = 0; i < 5; ++i) {
		std::cout << a1[i] << ' ' << a2[i] << '\n';
	}
}

void ReverseArrayByPtr(int *src, int *dest, const int SIZE) {
	for (auto it = src; it != src + SIZE; ++it) {
		auto dest_pos = dest + SIZE - 1 - (it - src);
		*dest_pos = *it;
	}
}

//void ReverseArrayByPtr(int *src, int *dest, const int SIZE) {
//	for (int i = SIZE - 1; i >= 0; --i) {
//		dest[i] = src[SIZE - i - 1];
//	}
//}
