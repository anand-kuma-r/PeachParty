#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject //  make this purely virtual
{
public:
	Actor(int imageID, int startX, int startY, int startDirection, int depth, double size, StudentWorld* world);
	void doSomething();
	bool isActive();
	void changeActive(bool change) { a_active = change; }
	virtual bool is_a_square() { return false; }
	StudentWorld* getWorld();
	virtual bool canGiveCoins() { return false; } // these are bool flags that are used for recognizing the Actors based on whether they can complete their tasks.
	virtual bool canTakeCoins() { return false; }
	virtual bool canGiveStars() { return false; }
	virtual bool canStealCoins() { return false; }
	virtual bool canChangeDir() { return false; }
	virtual bool canDoEvents() { return false; }
	virtual bool canDoDropping() { return false; }
	virtual bool isBad() { return false; }
	
private:
	StudentWorld* a_world;
	bool a_active;
	virtual void doDiffStuff() = 0;
};
///////////////////////////////////////////////////////////////////////////////////
// From here is squares
///////////////////////////////////////////////////////////////////////////////////
class CoinSquare : public Actor
{
public:
	CoinSquare(int imageID, int x, int y, StudentWorld* world);
	virtual bool is_a_square();
	bool can_be_hit_by_vortex();
private:

};

class BlueCoinSquare : public CoinSquare
{
public:
	BlueCoinSquare(int x, int y, StudentWorld* world);
	virtual void doDiffStuff();
	bool canGiveCoins();
private:
};
class RedCoinSquare : public CoinSquare
{
public:
	RedCoinSquare(int x, int y, StudentWorld* world);
	virtual void doDiffStuff();
	bool canTakeCoins();
private:
};
class StarSquare : public Actor
{
public:
	StarSquare(int x, int y, StudentWorld* world);
	virtual void doDiffStuff();
	virtual bool canGiveStars() { return isActive(); }
	virtual bool is_a_square();
	bool can_be_hit_by_vortex() { return false; }
private:

};
class DirectionalSquare : public Actor
{
public:
	DirectionalSquare(int x, int y, int dir, StudentWorld* world);
	virtual void doDiffStuff();
	virtual bool canChangeDir() { return isActive(); }
	virtual bool is_a_square() { return isActive(); }
	bool can_be_hit_by_vortex() { return false; }
private:

};
class BankSquare : public Actor
{
public:
	BankSquare(int x, int y, StudentWorld* world);
	virtual void doDiffStuff();
	virtual bool canStealCoins() { return isActive(); }
	virtual bool is_a_square() { return isActive(); }
	bool can_be_hit_by_vortex() { return false; }
private:

};
class EventSquare : public Actor
{
public:
	EventSquare(int x, int y, StudentWorld* world);
	virtual void doDiffStuff();
	virtual bool canDoEvents() { return isActive(); }
	virtual bool is_a_square() { return isActive(); }
	bool can_be_hit_by_vortex() { return false; }
private:

};
class DroppingSquare : public Actor
{
public:
	DroppingSquare(int x, int y, StudentWorld* world);
	virtual void doDiffStuff();
	virtual bool canDoDropping() { return isActive(); }
	virtual bool is_a_square() { return isActive(); }
	bool can_be_hit_by_vortex() { return false; }
};
///////////////////////////////////////////////////////////////////////////////////
// From here on is Moving Actors
///////////////////////////////////////////////////////////////////////////////////
class Player : public Actor
{
public:
	Player(int imageID, int startX, int startY, StudentWorld* world);
	virtual void doDiffStuff(int playerNumber);
	bool can_be_hit_by_vortex() { return false; }
	void setWalking(bool isWalking);
	int diceRoll();
	bool isWalking();
	int getTicksToWalk() { return p_ticksToWalk; };
	void decTickstoWalk() { p_ticksToWalk--; };
	void setTickstoWalk(int n) { p_ticksToWalk = n; };
	int isMovDir();
	void changeMovDir(int change);
	void setCoins(int n) { p_coins = n; }
	void setStars(int n) { p_stars = n; }
	void addCoins(int add);
	void subCoins(int sub);
	int getCoins() { return p_coins; };
	bool coinsAdded(); //return whether or not coins have been added
	void setCoinsAdded(bool change) { p_coinsAdded = change; };
	bool coinsSubbed() { return p_coinsSubbed; }
	void setCoinsSubbed(bool change) { p_coinsSubbed = change; };
	void checkDirection();
	int returnWhenToCheck() { return p_whenToCheck; }
	void setWhenToCheck(int n) { p_whenToCheck = n; }
	int getStars() { return p_stars; }
	void tradeCoinsForStars();
	bool starAdded() { return p_starsAdded; }
	void setStarsAdded(bool change) { p_starsAdded = change; }
	bool coinsStoleFromBank() { return p_coinsStoleFromBank; }
	void stealAllBankCoins();
	void setCoinsStoleFromBank(bool change) { p_coinsStoleFromBank = change; }
	bool coinsDeposited() { return p_coinsDeposited; }
	void depositCoins();
	void setCoinsDeposited(bool change) { p_coinsDeposited = change; }
	void checkFork();
	bool isFork() { return p_isFork; }
	void setIsOnDirSq(bool change) { p_isOnDirSq = change; }
	void teleportToRandSq();
	void swapPlayers();
	bool eventCompleted() { return p_eventCompleted; }
	void setEventCompleted(bool change) { p_eventCompleted = change; }
	bool hasVortex(){ return p_hasVortex; }
	void setVortex(bool change);
	bool getJustTeleported() { return p_justTeleported; }
	void setJustTeleported(bool change) { p_justTeleported = change; }
	bool itemsDropped() { return p_itemsDropped; }
	void dropItems();
	void changeCoins(int add) { p_coins += add; }
	void changeStars(int add) { p_stars += add; }
	int getRoll() { return p_roll; }

private:
	bool p_walking;
	int p_movDir;
	int p_coins;
	bool p_coinsAdded;
	bool p_coinsSubbed;
	int p_ticksToWalk;
	int p_whenToCheck;
	int p_stars;
	bool p_starsAdded;
	bool p_coinsStoleFromBank;
	bool p_coinsDeposited;
	bool p_isFork;
	bool p_isOnDirSq;
	bool p_eventCompleted;
	bool p_hasVortex;
	bool p_justTeleported;
	bool p_itemsDropped;
	bool p_roll;
};
class Peach : public Player
{
public:
	Peach(int x, int y, StudentWorld* world);
	virtual void doDiffStuff();
	int PlayerNum() { return p_playerNumber; }
	//void moveit(int destX, int destY);

private:
	int p_playerNumber = 1;
};

class Yoshi : public Player
{
public:
	Yoshi(int x, int y, StudentWorld* world);
	virtual void doDiffStuff();
	int PlayerNum() { return p_playerNumber; }
	//void moveit(int destX, int destY);

private:
	int p_playerNumber = 2;

};
class Baddie : public Actor
{
public:
	Baddie(int imageID, int startX, int startY, StudentWorld* world);
	bool isWalking() { return b_walking; }
	void setWalking(bool change) { b_walking = change; }
	virtual void doDiffStuff();
	virtual bool canStomp() { return false; }
	virtual bool canSwap() { return false; }
	bool eventCompleted() { return b_eventCompleted; }
	int isMovDir() { return b_movDir; }
	void changeMovDir(int change) { b_movDir = change; }
	void checkFork();
	bool isFork() { return b_isFork; }
	void checkDirection();
	bool can_be_hit_by_vortex() { return true; }
	void setJustTeleported(bool change) { b_justTeleported = change; }
	bool isBad() { return isActive(); }
	
private:
	bool b_walking;
	int b_pauseCounter;
	bool b_eventCompleted;
	int b_squaresToMove;
	int b_tickToMove;
	int b_movDir;
	bool b_isFork;
	int b_whenToCheck;
	bool b_justTeleported;
};
class Bowser : public Baddie
{
public:
	Bowser(int startX, int startY, StudentWorld* world);
	virtual void doDiffStuff() { Baddie::doDiffStuff(); }
	virtual bool canStomp() { return isActive(); }
private:
};
class Boo : public Baddie
{
public:
	Boo(int startX, int startY, StudentWorld* world);
	virtual void doDiffStuff() { Baddie::doDiffStuff(); }
	bool canSwap() { return true; } 

private:
};
class Vortex : public Actor
{
public:
	Vortex(int startX, int startY, int startDirection, StudentWorld* world);
	void doDiffStuff();
	int isMovDir() {return v_movDir; }
private:
	int v_movDir;
};

#endif // ACTOR_H_