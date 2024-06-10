#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<time.h>
using namespace std;

class Human
{
	static Human* instance;
	string last_name;
	string first_name;
	tm birth_date;
	Human()
	{
		cout << "Contructor:\t" << this << endl;
	}
public:
	const std::string& get_last_name()const
	{
		//Константный метод НЕ может изменить this
		//Константный метод обязательно должен возвращать константное значение
		return last_name;
	}
	const std::string& get_first_name()const
	{
		return first_name;
	}
	const tm& get_birth_date()const
	{
		return birth_date;
	}
	void set_last_name(const std::string& last_name)
	{
		this->last_name = last_name;
	}
	void set_first_name(const std::string& first_name)
	{
		this->first_name = first_name;
	}
	void set_birth_date(int year, int month, int day)
	{
		//this->birth_date = birth_date;
		this->birth_date.tm_year = year - 1900;
		this->birth_date.tm_mon = month - 1;
		this->birth_date.tm_mday = day;
	}
	~Human()
	{
		cout << "Destructor:\t" << this << endl;
	}

	static Human& get_instance()
	{
		if (instance == nullptr)instance = new Human();
		return *instance;
	}

	void print()const
	{
		time_t timer;
		time(&timer);
		tm* current_time = localtime(&timer);
		unsigned int age = current_time->tm_year - birth_date.tm_year;
		if (current_time->tm_yday < birth_date.tm_yday)age--;
		cout << last_name << " " << first_name << " " << age << endl;
	}
};

Human* Human::instance = nullptr;

void main()
{
	setlocale(LC_ALL, "");
	Human main = Human::get_instance();
	main.set_last_name("Vercetty");
	main.set_first_name("Tommy");
	main.set_birth_date(1991, 9, 6);
	main.print();
}