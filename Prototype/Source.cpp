#include <iostream>
#include <string>
#include <map>
#include <memory>
#define delimeter "\n------------------------------------------------\n\n"
using std::cin;
using std::cout;
using std::endl;

class Player
{
public:
	virtual std::unique_ptr<Player> clone() const = 0;
	/*Player(const std::string& name, int id) : name(name), id(id)
	{

	}*/
	virtual ~Player()
	{

	}
	virtual void print() const = 0;
};
class CarPlayer : public Player
{
	std::string name;
	int id;
public:
	CarPlayer(const std::string& name, int id) : name(name), id(id)
	{
		cout << "Car ctor: " << this << endl;
	}
	~CarPlayer()
	{
		cout << "car dtor: " << this << endl;
	}
	void print() const override
	{
		cout << this << ":\t" << name << " " << id << endl;
	}
	std::unique_ptr<Player>clone() const override
	{
		return std::make_unique<CarPlayer>(*this);
	}

};
class BikePlayer : public Player
{
	std::string name;
	int id;
public:
	BikePlayer(const std::string& name, int id) : name(name), id(id)
	{
		cout << "Bike ctor: " << this << endl;
	}
	~BikePlayer()
	{
		cout << "Bike dtor: " << this << endl;
	}
	void print() const override
	{
		cout << this << ":\t" << name << " " << id << endl;
	}
	std::unique_ptr<Player> clone() const override
	{
		return std::make_unique<BikePlayer>(*this);
	}
};
enum PlayerType
{
	CAR, BIKE
};
class PlayerFactory
{
	static std::map<PlayerType, std::unique_ptr<Player>> players;
	static void init()
	{
		static bool isInitialized = false;
		if (!isInitialized)
		{
			/*cout << "�������������� ���:\n";*/
			players[CAR] = std::make_unique<CarPlayer>("BMW", 735);
			players[BIKE] = std::make_unique<BikePlayer>("Harley Davidson", 200);
			isInitialized = true;
			/*cout << "������������� ���������\n"; */
		}
	}
public:
	static std::unique_ptr<Player> createPlayer(PlayerType type)
	{
		init();
		return players[type]->clone();
	}
	/*~PlayerFactory()
	{
		delete players[CAR];
		delete players[BIKE];
	}*/
};
std::map<PlayerType, std::unique_ptr<Player>> PlayerFactory::players;
//#define PROBLEM
//#define SOLUTION_1
#define SOLUTION_2
void main()
{
	setlocale(LC_ALL, "");
#ifdef PROBLEM
	const Player car_player_template("Car player", 100);
	const Player bike_player_template("Bike player", 200);
	car_player_template.print();
	bike_player_template.print();
	Player car_player = car_player_template;
	car_player.print();
	Player bike_player = bike_player_template;
	bike_player.print();
#endif // PROBLEM
#ifdef SOLUTION_1 //Ctrl K + S
	PlayerFactory factory;
	cout << delimeter;
	std::unique_ptr<Player> car_player = factory.createPlayer(CAR);
	car_player->print();

	cout << delimeter;
	std::unique_ptr<Player> bike_player = factory.createPlayer(BIKE);
	bike_player->print();
	cout << delimeter;
#endif // SOLUTION_1
#ifdef SOLUTION_2
	cout << delimeter;
	std::unique_ptr<Player> car_player = PlayerFactory::createPlayer(CAR);
	car_player->print();
	cout << delimeter;
	std::unique_ptr<Player> bike_player = PlayerFactory::createPlayer(BIKE);
	bike_player->print();
	cout << delimeter;
#endif // SOLUTION_2

}