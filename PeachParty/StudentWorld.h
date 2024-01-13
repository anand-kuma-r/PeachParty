#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	~StudentWorld();
	virtual int init();
	virtual int move();
	virtual void cleanUp();


	Actor* ActorAt(int x, int y);
	bool isPeachAt(int x, int y);
	bool isYoshiAt(int x, int y);
	bool isEmptySpace(int x, int y);
	bool isBlueCoinSquare(int x, int y);
	bool isRedCoinSquare(int x, int y);
	bool isStarSquare(int x, int y);
	bool isBankSquare(int x, int y);
	void DirSquareLandPeach(int dir);
	void DirSquareLandYoshi(int dir);
	int get_bank_coins() const { return s_bankCoins; }
	void deposit_bank_coins(int coins) { s_bankCoins += coins; }
	void reset_bank_coins() { s_bankCoins = 0; }
	bool isDirSquare(int x, int y);
	bool isEventSquare(int x, int y);
	bool isDroppingSquare(int x, int y);
	int whatDirSquare(int x,int y);
	void teleportToRandSquare(Player* player);
	void swapPlayers();
	void swapCoins();
	void swapStars();
	void bowserStompP();
	void bowserStompY();
	void newActor(Actor* actor);
	Actor* specificObjectAt(int x, int y);
	void teleportToRandSquareB(Actor* baddie);


private:
	int s_size = 0; // size of list 
	std::vector<Actor*> s_actors;//list of pointers to actors
	Peach* s_peach;
	Yoshi* s_yoshi;
	int s_board = getBoardNumber();
	Board bd;
	int s_bankCoins;
};

#endif // STUDENTWORLD_H_
