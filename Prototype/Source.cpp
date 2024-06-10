#include<iostream>
#include<string>
#include<map>
#include<memory>


using namespace std;
#define delimiter "\n---------------------------\n"
class Player
{

public:
	virtual unique_ptr<Player> clone() const = 0;
	//Player(const std:: string& name,int id):name(name),id(id){}
	virtual ~Player() {}
	virtual void print()const = 0;
	/*{
		cout <<this <<"\t" << name << "\t" << id << endl;
	}*/

};
class CarPlayer :public Player
{
	std::string name;
	int id;
public:
	CarPlayer(const std::string& name, int id) :name(name), id(id)
	{
		cout << "CarConstructor:\t" << this << endl;
	}
	~CarPlayer()
	{
		cout << "CarDestructor" << this << endl;

	}
	void print()const override
	{
		cout << this << ":\t" << name << " " << id << endl;
	}
	unique_ptr<Player> clone()const override
	{
		return make_unique<CarPlayer>(*this);
	}
};

class BikePlayer :public Player
{
	std::string name;
	int id;
public:
	BikePlayer(const std::string name, int id) :name(name), id(id)
	{
		cout << "BikeCostructor:" << this << endl;
	}
	~BikePlayer()
	{
		cout << "BikeDestructor:" << this << endl;
	}
	void print()const override
	{
		cout << this << ":\t" << name << "" << id << endl;
	}
	unique_ptr<Player>clone()const override
	{
		return make_unique<BikePlayer>(*this);
	}
};

enum PlayerType { CAR, BIKE };
class PlayerFactory
{
	static std::map<PlayerType,unique_ptr<Player>> players;
	static void Init()
	{
		if(players.find(CAR)==players.end())players[CAR] = make_unique<CarPlayer>(CarPlayer("BMW", 735));
		if(players.find(BIKE)==players.end())players[BIKE] = make_unique<BikePlayer>(BikePlayer("Harley Davidson", 200));
	}
	

public:
	/*~PlayerFactory()
	{
		delete players[CAR];
		delete players[BIKE];
	}*/
	static unique_ptr<Player> CreatePlayer(PlayerType type)
	{
		Init();
		return players[type]->clone();
	}
};
//#define PROBLEM 
//#define SOLUTION_1
#define SOLUTION_2
void main()
{
	setlocale(LC_ALL, "");
#ifdef PROBLEM
	const Player car_player_template("Car player", 100);
	const Player bike_player_template("bike player", 200);
	car_player_template.print();
	bike_player_template.print();
	Player car_player = car_player_template;
	car_player.print();
	Player bike_player = bike_player_template;
	bike_player.print();
#endif // PROBLEM

#ifdef SOLUTION_1
	PlayerFactory factory;

	cout << delimiter << endl;
	std::unique_ptr<Player> car_player = factory.CreatePlayer(CAR);
	car_player->print();
	cout << delimiter << endl;
	std::unique_ptr<Player> bike_player = factory.CreatePlayer(BIKE);
	bike_player->print();
	cout << delimiter << endl;
	/*delete bike_player;
	delete car_player;
	cout << delimiter << endl;*/
#endif // SOLUTION_1

	std::unique_ptr<Player> car_player = PlayerFactory::CreatePlayer(CAR);
	car_player->print();
	cout << delimiter << endl;
	std::unique_ptr<Player> bike_player = PlayerFactory::CreatePlayer(BIKE);
	bike_player->print();


}