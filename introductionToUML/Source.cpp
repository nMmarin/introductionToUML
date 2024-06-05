#include <iostream>
#include <conio.h>
#include <windows.h>
#include <thread>
using namespace std;

#define MIN_TANK_VOLUME 20
#define MAX_TANK_VOLUME 150
#define MIN_ENGINE_CONSUMPTION 3
#define MAX_ENGINE_CONSUMPTION 30
#define MAX_SPEED_LOW_LIMIT 120
#define MAX_SPEED_HIGH_LIMIT 400
#define Escape 27
#define Enter 13

#define Up 72 //speed up
#define Down 80 //slow down
#define Space 32 //engine on/off
class Tank
{
	const int VOLUME;
	double fuelLevel;
public:
	int getVOLUME() const
	{
		return VOLUME;
	}
	double getFuelLevel() const
	{
		return fuelLevel;
	}
	void fill(double amount)
	{
		if (amount < 0)
		{
			return;
		}
		fuelLevel += amount;
		if (fuelLevel > VOLUME)
		{
			fuelLevel = VOLUME;
		}
	}
	double giveFuel(double amount)
	{
		fuelLevel -= amount;
		if (fuelLevel < 0)
		{
			fuelLevel = 0;
		}
		return fuelLevel;
	}
	Tank(int volume) :
		VOLUME
		(
			volume < MIN_TANK_VOLUME ? MIN_TANK_VOLUME :
			volume > MAX_TANK_VOLUME ? MAX_TANK_VOLUME :
			volume
		)
	{
		fuelLevel = 0;
		cout << "Tank is ready " << this << endl;
	}
	~Tank()
	{
		cout << "Tank is over " << this << endl;
	}
	void info() const
	{
		cout << "VOLUME: " << VOLUME << " liters" << endl
			<< "Fuel level: " << getFuelLevel() << " liters" << endl;
	}
};
class Engine
{
	const double CONSUMPTION;
	double consumptionPerSecond;
	bool isStarted;

public:
	double getCONSUMPTION() const
	{
		return CONSUMPTION;
	}
	double getConsumptionPerSecond() const
	{
		return consumptionPerSecond;
	}
	void setConsumptionPerSecond(int speed)
	{
		if (started())
		{
			consumptionPerSecond = CONSUMPTION * 3e-5 + CONSUMPTION * 0.0000283 * speed;//Я не знаю, насколько реалистично потребление топлива, я просто тыкала в калькулятор, пока не получила усредненные значения из интернета
		}
		else
		{
			consumptionPerSecond = 0;
		}

	}
	void start()
	{
		isStarted = true;

	}
	void stop()
	{
		isStarted = false;
	}
	bool started() const
	{
		return isStarted;

	}
	void info() const
	{
		cout << "Consumption:\n:";
		cout << CONSUMPTION << " liters/100km" << endl;
		cout << consumptionPerSecond << " liters/second" << endl;
	}
	Engine(double consumption) : CONSUMPTION
	(
		consumption < MIN_ENGINE_CONSUMPTION ? MIN_ENGINE_CONSUMPTION :
		consumption > MAX_ENGINE_CONSUMPTION ? MAX_ENGINE_CONSUMPTION :
		consumption
	)
	{
		isStarted = false;
		setConsumptionPerSecond(0);
		cout << "Engine is ready: " << this << endl;
	}
	~Engine()
	{
		cout << "Engine is over: " << this << endl;
	}

};
class Car
{
	Engine engine;
	Tank tank;
	int speed;
	const unsigned int MAX_SPEED;
	bool driverInside;
	struct
	{
		std::thread panelThread;
		std::thread consumptionThread;
	} controlThreads;
public:
	int getSpeed() const
	{
		return speed;
	}
	Car(int maxSpeed, double consumption, int volume) : engine(consumption), tank(volume),
		MAX_SPEED
		(
			maxSpeed < MAX_SPEED_LOW_LIMIT ? MAX_SPEED_LOW_LIMIT :
			maxSpeed > MAX_SPEED_HIGH_LIMIT ? MAX_SPEED_HIGH_LIMIT :
			maxSpeed
		)
	{
		speed = 0;
		driverInside = false;
		cout << "Your car is ready: " << this << endl;
		cout << "Press Enter to get in: " << endl;
	}
	~Car()
	{
		cout << "Car is over" << this << endl;
	}
	void info() const
	{
		engine.info();
		tank.info();
		cout << "max speed: " << MAX_SPEED << " km/h" << endl;
	}
	void getIn()
	{
		driverInside = true;
		controlThreads.panelThread = std::thread(&Car::panel, this);
	}
	void getOut()
	{
		driverInside = false;
		if (controlThreads.panelThread.joinable())
		{
			controlThreads.panelThread.join();
		}
		system("cls");
		cout << "Вы вышли из машины" << endl;
	}
	void panel() const
	{
		while (driverInside)
		{
			system("cls");
			cout << "Speed: " << speed << " km/h\n";
			cout << "Engine is: " << (engine.started() ? "started " : "stopped ") << endl;
			cout << "Fuel level: " << tank.getFuelLevel() << " liters" << endl;
			cout << "Consumption: " << engine.getConsumptionPerSecond() << " liters/second" << endl;
			Sleep(1000);
		}

	}
	void spendFuel()
	{
		while (engine.started())
		{
			tank.giveFuel(engine.getConsumptionPerSecond());
			Sleep(1000);
		}
	}
	void control()
	{
		char key;
		do
		{
			key = _getch();
			switch (key)
			{
			case Enter:
				if (driverInside and !speed) getOut();
				else if (!driverInside and !speed) getIn();
				break;
			case Escape:
				stopEngine();//Чтобы заджойнить поток потребления топлива
				getOut();
				break;
				//HW
			case Up:
				if (driverInside and engine.started())
				{
					speedUp();
				}
				break;
			case Down:
				if (driverInside and engine.started())
				{
					slowDown();
				}
				break;
			case Space:
				if (engine.started() and !speed) stopEngine();
				else if (!engine.started() and !speed) startEngine();
				break;
			}


		} while (key != Escape);
	}
	void startEngine()//HW
	{
		engine.start();
		engine.setConsumptionPerSecond(0);
		cout << "Двигатель запущен\n";
		controlThreads.consumptionThread = std::thread(&Car::spendFuel, this);

	}
	void stopEngine()//HW
	{
		engine.stop();
		engine.setConsumptionPerSecond(0);
		if (controlThreads.consumptionThread.joinable())
		{
			controlThreads.consumptionThread.join();
		}
		cout << "Двигатель остановлен\n";
	}
	void fillTank(int amount)//HW
	{
		tank.fill(amount);
	}
	void speedUp()//HW
	{
		//Ускоряемся на 10км/ч при нажатии кнопки
		speed += 10;
		if (speed > MAX_SPEED)
		{
			speed = MAX_SPEED;
		}
		engine.setConsumptionPerSecond(speed);
	}
	void slowDown()//HW
	{
		//Замедляемся на 10км/ч при нажатии кнопки

		if (speed > 0)
		{
			speed -= 10;
		}
		engine.setConsumptionPerSecond(speed);
	}
};
//#define TANK_CHECK
//define - определить
//Директива дефайн создает макроопределение (макрос)
//Директива - указание компилятору на выполнение некоторых действий
#define ENGINE_CHECK
void main()
{
	setlocale(LC_ALL, "");

#ifdef TANK_CHECK //Если определено TANK_CHECK, то код до директивы endif будет виден компилятору

	Tank tank(3000);
	Engine engine;
	int fuel;
	do
	{
		tank.info();
		cout << "Введите объем топлива: ";
		cin >> fuel;
		tank.fill(fuel);
		engine.info();
		engine.start();//If engine is on, consumption per second = IDLE_CONSUMPTION = 0.0003 
		engine.info();
		engine.stop();//If engine is off, consumption per second = 0 

	} while (fuel);
	tank.info();
#endif
	/*Engine engine(10);
	engine.info();*/
	Car bmw(250, 10, 80);
	bmw.fillTank(80);//Filling up our rattletrap before the start
	bmw.control();

}