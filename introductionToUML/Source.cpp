#include<iostream>
#include <conio.h>
#include<Windows.h>
#include<thread>
using namespace std;

#define MIN_TANK_VOLUME 20	//SMART for two
#define MAX_TANK_VOLUME	150	//Dodge RAM
#define Escape 27
#define Enter 13
 

class Tank
{
	const int VOLUME;	//Объем бака является его характеристикой
	double fuel_level;	//Уровень топлива отображает состояние бака
public:
	int get_VOLUME()const
	{
		return VOLUME;
	}
	double get_fuel_level()const
	{
		return fuel_level;
	}
	void fill(double amount)
	{
		if (amount < 0)return;
		fuel_level += amount;
		if (fuel_level > VOLUME)fuel_level = VOLUME;
	}
	double give_fuel(double amount)
	{
		fuel_level -= amount;
		if (fuel_level < 0)fuel_level = 0;
		return fuel_level;
	}
	Tank(int volume) :
		VOLUME
		(
			volume < MIN_TANK_VOLUME ? MIN_TANK_VOLUME :
			volume > MAX_TANK_VOLUME ? MAX_TANK_VOLUME :
			volume
		)
	{
		//l-value = r-value;
		//VOLUME = volume;
		fuel_level = 0;
		cout << "Tank is ready " << this << endl;
	}
	~Tank()
	{
		cout << "Tank is over " << this << endl;
	}
	void info()const
	{
		cout << "Volume:   " << VOLUME << " liters;\n";
		cout << "Fuel leve:" << get_fuel_level() << " liters;\n";
	}
};
#define min_engine_consumption 3
#define max_engine_consumption 30
class Engine
{
	const double consumption;
	double consumption_per_second;
	bool is_started;
public:
	double get_consumption()const
	{
		return consumption;
	}
	double get_consumption_per_second()
	{
		return consumption_per_second;
	}
	void start()
	{
		is_started = true;
	}
	void stop()
	{
		is_started = false;
	}
	bool started()
	{
		return is_started;
	}

	void set_consumptoin_per_second()
	{
		consumption_per_second = consumption * 3e-5;
	}
	
	Engine(int consumption):consumption
	(
		consumption<min_engine_consumption?min_engine_consumption:
		consumption>max_engine_consumption?max_engine_consumption:
		consumption
	)
	{
		//this->consumption = consumption;
		set_consumptoin_per_second();
		is_started = false;
		cout << "engine is ready" << endl;
	}
	~Engine()
	{
		cout << "Engine is over: " << this << endl;
	}
	void info()const
	{
		cout << "consumption:\n";
		cout << consumption << " litres/100km\n";
		cout << consumption_per_second << " litres/seconds\n";
	}


};
#define max_speed_low_limit 120
#define max_speed_high_limit 400
class Car
{
	Engine engine;
	Tank tank;
	int speed;
	const int max_speed;
	bool driver_inside;
	struct 
	{
		std::thread panel_thread;
	}control_threads;
public:
	Car(int max_speed, double consumption,int volume):engine(consumption), tank(volume),max_speed
		(max_speed<max_speed_low_limit? max_speed_low_limit:
			max_speed>max_speed_high_limit? max_speed_high_limit:
			max_speed
		)
	{
		speed = 0;
		driver_inside = false;
		cout << "Your car is  ready" << this << endl;
		cout<<"press Enter to get in : " << endl; 
	}
	~Car()
	{
		cout << "car is over" << endl;
	}
	void control()
	{
		char key;
		do
		{
			key = _getch();
			switch (key)
			{
			case Enter: driver_inside ? get_out() : get_in(); break;
			case Escape:
				get_out();
			}
		} while (key!=Escape);
	}
	void get_in()
	{
		driver_inside = true;
		//panel();
		control_threads.panel_thread = std::thread(&Car::panel, this);
	}
	void get_out()
	{
		driver_inside = false;
		if (control_threads.panel_thread.joinable())control_threads.panel_thread.join();
		system("cls");
		cout << "you got out of the car" << endl;
	}
	void panel()
	{	
		while (driver_inside)
		{
			system("cls");
			cout << "Speed: " << speed << "km/h" << endl;
			cout << "Engine is" << (engine.started() ? "starterd" : "stoped") << endl;
			cout << "Fuel level" << tank.get_fuel_level() << "litres" << endl;
			cout << "consumption" << engine.get_consumption_per_second() << "litres/sec" << endl;
			Sleep(600);
		}
	
	}
	void info()const
	{
		engine.info();
		tank.info();
		cout << "Max speed: " << max_speed <<"km/h"<< endl;
	}
	/*void idle_speed(Engine engine,Tank tank )
	{
		do
		{
			tank.get_fuel_level()engine.set_consumptoin_per_second()
		} while (engine.started());
	}*/

};
//#define TANK_CHECK// #define - определить 
// Директива define создает макроопределение (макрос)
//Директива указание комплилятору на выполнение некоторых действий

void main()
{
	setlocale(LC_ALL, "");
#ifdef TANK_CHECK
	Tank tank(3000);
	int fuel;
	do
	{
		tank.info();
		cout << "Введите объем топлива: "; cin >> fuel;
		tank.fill(fuel);
	} while (fuel);
#endif // DEBUG
	//Engine engine(10);
	//engine.info();
	//engine.start();
	Car bmw(250,10,80);
	//bmw.info();
	bmw.control();
	



};
