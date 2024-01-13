#include "Actor.h"
#include "StudentWorld.h"
#include <cstdlib>
//NOTE the speed of Peach is caused by my error in reading the spec. I didnt see the specified 
// number of pixels peach is supposed to move per tick. I assumed incorrectly that it should be evuivalent
// to the size of the sprite dimensions. By the time I realized the movement speed was wrong because of this
//I already completed every other required functions of part 1. Due to lack of time I submitted it like this 
//before the deadline. If i fixed it within a few hours of resubmission you will have never seen this comment.

using namespace std;
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageID, int startX, int startY, int startDirection, int depth, double size, StudentWorld* world)
    : GraphObject(imageID, startX, startY, startDirection, depth, size)
{
    a_world = world;
    a_active = true;
}

bool Actor::isActive() {
    return a_active;
}
void Actor::doSomething()
{
    if (!isActive())
    {
        return;
    }
    doDiffStuff();// this will handle the unique functionaity that each actor has
}
StudentWorld* Actor::getWorld()
{
    return a_world;
}

CoinSquare::CoinSquare(int imageID, int x, int y, StudentWorld* world)
    :Actor(imageID, x, y, 90, 1, 1.0, world)
{}
bool CoinSquare::is_a_square()
{
    return isActive();
}
bool CoinSquare::can_be_hit_by_vortex()
{
    return false;
}
BlueCoinSquare::BlueCoinSquare(int x, int y, StudentWorld* world)
    :CoinSquare(IID_BLUE_COIN_SQUARE, x, y, world)
{}
void BlueCoinSquare::doDiffStuff()
{
    return;
}
bool BlueCoinSquare::canGiveCoins()
{
    return isActive();

}
RedCoinSquare::RedCoinSquare(int x, int y, StudentWorld* world)
    :CoinSquare(IID_RED_COIN_SQUARE, x, y, world)
{}
void RedCoinSquare::doDiffStuff()
{
    return;
}
bool RedCoinSquare::canTakeCoins()
{
    return isActive();
}

StarSquare::StarSquare(int x, int y, StudentWorld* world)
    :Actor(IID_STAR_SQUARE, x, y, 0, 1, 1.0, world)
{}
bool StarSquare::is_a_square()
{
    return isActive();
}
void StarSquare::doDiffStuff()
{
    return;
}
DirectionalSquare::DirectionalSquare(int x, int y, int dir, StudentWorld* world)
    :Actor(IID_DIR_SQUARE, x, y, dir, 1, 1.0, world)
{}
void DirectionalSquare::doDiffStuff()
{
    if (getWorld()->isPeachAt(getX(), getY()))
    {
        getWorld()->DirSquareLandPeach(getDirection());
        //cout << "Peach has turneth! " << getDirection() << endl;
    }
    if (getWorld()->isYoshiAt(getX(), getY()))
    {
        getWorld()->DirSquareLandYoshi(getDirection());
        //cout << "Yoshi has turneth! " << getDirection() << endl;
    }
    
}
BankSquare::BankSquare(int x, int y, StudentWorld* world)
    :Actor(IID_BANK_SQUARE, x, y, 0, 1, 1.0, world)
{}
void BankSquare::doDiffStuff()
{
    return;
}
EventSquare::EventSquare(int x, int y, StudentWorld* world)
    :Actor(IID_EVENT_SQUARE, x, y, 0, 1, 1.0, world)
{}
void EventSquare::doDiffStuff()
{
    return;
}
DroppingSquare::DroppingSquare(int x, int y, StudentWorld* world)
    :Actor(IID_DROPPING_SQUARE, x, y, 0, 1, 1.0, world)
{}
void DroppingSquare::doDiffStuff()
{
    return;
}
Player::Player(int imageID, int x, int y, StudentWorld* world)
    :Actor(imageID, x, y, 0, 0, 1.0, world)
{
    p_walking = false;
    p_ticksToWalk = 0;
    p_movDir = getDirection();
    p_coins = 0;
    p_coinsAdded = false;
    p_coinsSubbed = false;
    p_whenToCheck = 0;
    p_starsAdded = false;
    p_stars = 0;
    p_coinsStoleFromBank = false;
    p_coinsDeposited = false;
    p_isFork = false;
    p_isOnDirSq = false;
    p_eventCompleted = false;
    p_hasVortex = false;
    p_justTeleported = false;
    p_itemsDropped = false;
    p_roll = 0;
}
bool Player::coinsAdded()
{
    return p_coinsAdded;
}
void Player::setWalking(bool isWalking)
{
    p_walking = isWalking;
}
bool Player::isWalking()
{
    return p_walking;
}
int Player::diceRoll()
{
    return randInt(1, 10); // Return a random number between 1 and 10
}
int Player::isMovDir() {
    return p_movDir;
}
void Player::changeMovDir(int change) {
    p_movDir = change;
}
void Player::addCoins(int add)
{
    p_coins += add;
    getWorld()->playSound(SOUND_GIVE_COIN);
}

void Player::subCoins(int sub)
{
    if (p_coins >= 3)
    {
        p_coins -= sub;
        getWorld()->playSound(SOUND_TAKE_COIN);
    }
}
void Player::tradeCoinsForStars()
{
    if (getCoins() < 20)
        return;
    else
    {
        p_coins -= 20;
        p_stars++;
        getWorld()->playSound(SOUND_GIVE_STAR);
    }
}
void Player::stealAllBankCoins()
{
    int bank = getWorld()->get_bank_coins();
   // cout << "Bank has: " << bank << endl;
    p_coins += bank;
    getWorld()->reset_bank_coins();
    getWorld()->playSound(SOUND_WITHDRAW_BANK);
   // cout << "Bank has: " << getWorld()->get_bank_coins() << endl;
}
void Player::depositCoins()
{

    if (p_coins >= 5)
    {
        p_coins -= 5;
        getWorld()->deposit_bank_coins(5);
        //cout<< "deposited 5 coins in bank " << getWorld()->get_bank_coins() << endl;
        getWorld()->playSound(SOUND_DEPOSIT_BANK);
    }
    else if (p_coins < 5)
    {
        getWorld()->deposit_bank_coins(getCoins());
        p_coins -= getCoins();
       // cout << "deposited less than 5 coins in bank " << getWorld()->get_bank_coins() << endl;
        getWorld()->playSound(SOUND_DEPOSIT_BANK);
    }
    else return;
}
void Player::teleportToRandSq()
{
    getWorld()->teleportToRandSquare(this);
}
void Player::swapPlayers()
{
    getWorld()->swapPlayers();
}
void Player::setVortex(bool change)
{
     p_hasVortex = change;
     getWorld()->playSound(SOUND_GIVE_VORTEX);
     //cout << "Player has obtained Vortex!!" << endl;
}
void Player::dropItems()
{
   int rand = randInt(1, 2);

    if (rand == 1 && p_stars >= 1)
    {
        p_stars--;
        getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
        //cout << "number of stars: " << p_stars << endl;
    }
    else if (rand == 2)
    {
        p_coins -= 10;
        getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
    }
    
}
Peach::Peach(int x, int y, StudentWorld* world)
    :Player(IID_PEACH, x, y, world)
{}

void Player::checkDirection()
{
    // Find available space perpendicular to current direction of motion
    if (isMovDir() == 0) // Moving right
    {
        if (!getWorld()->isEmptySpace(getX(), getY() + SPRITE_HEIGHT))
        {
            changeMovDir(90);//change move direction to up
            setDirection(0);
        }
        else if (!getWorld()->isEmptySpace(getX(), getY() - SPRITE_HEIGHT))
        {
            changeMovDir(270);
            setDirection(0);
        }
        // Can't move in any direction, stop walking
    }
    else if (isMovDir() == 180) // Moving left
    {
        if (!getWorld()->isEmptySpace(getX(), getY() + SPRITE_HEIGHT))
        {
            changeMovDir(90);
            setDirection(0);
        }
        else if (!getWorld()->isEmptySpace(getX(), getY() - SPRITE_HEIGHT))
        {
            changeMovDir(270);
            setDirection(0);
        }
        // Can't move in any direction, stop walking
    }
    else if (isMovDir() == 90) // Moving up
    {
        if (!getWorld()->isEmptySpace(getX() + SPRITE_WIDTH, getY()))
        {
            changeMovDir(0);
            setDirection(0);
        }
        else if (!getWorld()->isEmptySpace(getX() - SPRITE_WIDTH, getY()))
        {
            changeMovDir(180);
            setDirection(180);
        }
    }
    else if (isMovDir() == 270) // Moving down
    {
        if (!getWorld()->isEmptySpace(getX() + SPRITE_WIDTH, getY()))
        {
            changeMovDir(0);
            setDirection(0);
        }
        else if (!getWorld()->isEmptySpace(getX() - SPRITE_WIDTH, getY()))
        {
            changeMovDir(180);
            setDirection(180);
        }
    }


}
void Player::checkFork()
{
    //cout << "checks for fork" << endl;
    int paths = 0;

    if (isMovDir() == 0) // Moving right
    {
        if (!getWorld()->isEmptySpace(getX(), getY() + SPRITE_HEIGHT)) //checks up
        {
            paths++;
        }
        if (!getWorld()->isEmptySpace(getX(), getY() - SPRITE_HEIGHT)) //checks down
        {
            paths++;
        }
        if (!getWorld()->isEmptySpace(getX() + SPRITE_WIDTH, getY())) // checks right
        {
            paths++;
        }
    }
    else if (isMovDir() == 180) // Moving left
    {
        if (!getWorld()->isEmptySpace(getX(), getY() + SPRITE_HEIGHT)) //checks up
        {
            paths++;
        }
        if (!getWorld()->isEmptySpace(getX(), getY() - SPRITE_HEIGHT)) //checks down
        {
            paths++;
        }
        if (!getWorld()->isEmptySpace(getX() - SPRITE_WIDTH, getY())) // checks left
        {
            paths++;
        }
    }
    else if (isMovDir() == 90) // Moving up
    {
        if (!getWorld()->isEmptySpace(getX() + SPRITE_WIDTH, getY())) //checks right
        {
            paths++;
        }
        if (!getWorld()->isEmptySpace(getX() - SPRITE_WIDTH, getY()))//checks left
        {
            paths++;
        }
        if (!getWorld()->isEmptySpace(getX(), getY() + SPRITE_HEIGHT))// checks up
        {
            paths++;
        }
    }
    else if (isMovDir() == 270) // Moving down
    {
        if (!getWorld()->isEmptySpace(getX() + SPRITE_WIDTH, getY()))
        {
            paths++;
        }
        if (!getWorld()->isEmptySpace(getX() - SPRITE_WIDTH, getY()))
        {
            paths++;
        }
        if (!getWorld()->isEmptySpace(getX(), getY() - SPRITE_HEIGHT))// checks up
        {
            paths++;
        }
    }
    if (paths  > 1)
    {
        p_isFork = true;
        //cout << "Fork is found" << endl;
    }
}
void Peach::doDiffStuff() {
    Player::doDiffStuff(PlayerNum());
}

void Player::doDiffStuff(int playerNumber)
{

    int key = getWorld()->getAction(playerNumber);
    int roll = 0;
    int moveX = 0;
    int moveY = 0;
    if (!isWalking())
    {
        switch (key)
        {
        case ACTION_NONE:
            break;
        case ACTION_ROLL:

            roll = diceRoll();
            p_roll = roll;
            //cout << "rolled: " << roll << endl;
            setTickstoWalk(roll * 8);
            setWalking(true);
            //decTickstoWalk();
            break;
        case ACTION_FIRE:
            if (hasVortex())
            {
                if(isMovDir()==0)
                    getWorld()->newActor(new Vortex(getX()+SPRITE_WIDTH, getY(),isMovDir(), getWorld()));
                else if(isMovDir() == 90)
                    getWorld()->newActor(new Vortex(getX(), getY()+SPRITE_HEIGHT, isMovDir(), getWorld()));
                else if (isMovDir() == 180)
                    getWorld()->newActor(new Vortex(getX()-SPRITE_WIDTH, getY(), isMovDir(), getWorld()));
                else if (isMovDir() == 270)
                    getWorld()->newActor(new Vortex(getX(), getY()-SPRITE_HEIGHT, isMovDir(), getWorld()));
                setVortex(false);
                getWorld()->playSound(SOUND_PLAYER_FIRE);
            }
            
        }
    }
    if (returnWhenToCheck() == 8)
    {
        if (!getWorld()->isDirSquare(getX(), getY()) && isWalking())
        {
            checkFork();
        }
        checkDirection();
        setWhenToCheck(0);
    }
    if (isWalking())
    {
        if (!p_isFork || ( !p_isFork && !p_justTeleported)) // if current square isnt at a fork or on a directionalsquare.
        {
            setWhenToCheck(returnWhenToCheck() + 1);

            //int destX = getX();
            //int destY = getY();
            //cout << endl;
            //cout << "current X: " << destX << " current Y: " << destY << endl;
            switch (isMovDir())
            {
            case 0:
                moveX = 2;
                break;
            case 90:
                moveY = 2;
                break;
            case 180:
                moveX = -2;
                break;
            case 270:
                moveY = -2;
                break;
            }
            //cout << "direction peach is moving " << isMovDir() << endl;
            //cout << "moving in X " << moveX << endl;
            //cout << "moving in y " << moveY << endl;

            //cout << "How many ticks in between checks it has been " << returnWhenToCheck() << endl;
            moveTo(getX() + moveX, getY() + moveY);
            decTickstoWalk();
            setCoinsAdded(false);
            setCoinsSubbed(false);
            setStarsAdded(false);
            setCoinsStoleFromBank(false);
            setCoinsDeposited(false);
            setIsOnDirSq(false);
            p_eventCompleted = false;
            p_itemsDropped = false;
            //cout <<"moving to position " <<getX()+ moveX << " " << getY() + moveY << endl;
            //cout << "ticks left to walk " << getTicksToWalk() << endl;
            //cout << "is peach on center of blue coin square: " << getWorld()->isBlueCoinSquare(getX() + moveX, getY() + moveY) << endl;
            //cout << "Is peach walking 0 is false and 1 is true: " << isWalking() << endl;
        }
        if (p_isFork || p_justTeleported)
        {
            int newKey = getWorld()->getAction(playerNumber);

            switch (key)
            {
            case ACTION_NONE:
                break;
            case ACTION_LEFT:
                /*if (isMovDir() == 0)
                {
                    break;
                }
                else*/ if (getWorld()->isEmptySpace(getX() - SPRITE_WIDTH, getY()))
                {
                    break;
                }
                else
                {
                    changeMovDir(180);
                    if (p_isFork)
                        p_isFork = false;
                    if (p_justTeleported)
                        p_justTeleported = false;

                }
                break;
            case ACTION_RIGHT:
                /*if (isMovDir() == 180)
                {
                    break;
                }
                else*/ if (getWorld()->isEmptySpace(getX() + SPRITE_WIDTH, getY()))
                {
                    break;
                }
                else
                {
                    changeMovDir(0);
                    if (p_isFork)
                        p_isFork = false;
                    if (p_justTeleported)
                        p_justTeleported = false;

                }
                break;
            case ACTION_UP:
                /*if (isMovDir() == 270)
                {
                    break;
                }
                else */if (getWorld()->isEmptySpace(getX(), getY() + SPRITE_HEIGHT))
                {
                    break;
                }
                else
                {
                    changeMovDir(90);
                    if (p_isFork)
                        p_isFork = false;
                    if (p_justTeleported)
                        p_justTeleported = false;

                }
                break;
            case ACTION_DOWN:
                /*if (isMovDir() == 90)
                {
                    break;
                }
                else*/ if (getWorld()->isEmptySpace(getX(), getY() - SPRITE_HEIGHT))
                {
                    break;
                }
                else
                {
                    changeMovDir(270);
                    if (p_isFork)
                        p_isFork = false;
                    if (p_justTeleported)
                        p_justTeleported = false;

                }
                break;
            case ACTION_ROLL:
                if (getTicksToWalk() == 0)
                {
                    setWalking(false);// to activate any square on fork that may meed it.
                    int roll = diceRoll();
                    //cout << "rolled: " << roll << endl;
                    setTickstoWalk(roll * 8);
                    setWalking(true);

                    return;
                }

                break;
            }

        }
    }

    if (getTicksToWalk() == 0)
    {
        setWalking(false);
        //cout << "peach stopped walking" << endl;
    }
    if (isMovDir() == 180)
    {
        setDirection(180);
    }
    if (returnWhenToCheck() == 8)
    {
        if (!getWorld()->isDirSquare(getX(), getY()) && isWalking())
        {
            checkFork();
        }
        checkDirection();
        setWhenToCheck(0);
    }
    

    if (isWalking() == false && getWorld()->isBlueCoinSquare(getX(), getY()) && coinsAdded() == false)
    {
        addCoins(3);
        //cout << "current coins " << getCoins() << endl;
        //cout << "Is peach walking 0 is false and 1 is true: " << isWalking() << endl;
        setCoinsAdded(true);
    }
    if (isWalking() == false && getWorld()->isRedCoinSquare(getX(), getY()) && coinsSubbed() == false)
    {
        subCoins(3);
        //cout << "current coins " << getCoins() << endl;
        //cout << "Is peach walking 0 is false and 1 is true: " << isWalking() << endl;
        setCoinsSubbed(true);
    }
    if (getWorld()->isStarSquare(getX(), getY()) && starAdded() == false)
    {
        tradeCoinsForStars();
        //cout << "current stars " << getStars() << endl;
        //cout << "current coins " << getCoins() << endl;
        //cout << "Is peach walking 0 is false and 1 is true: " << isWalking() << endl;
        setStarsAdded(true);
    }
    if (isWalking() == false && getWorld()->isBankSquare(getX(), getY()) && coinsStoleFromBank() == false)
    {
        stealAllBankCoins();
        //cout << "current coins " << getCoins() << endl;
        //cout << "Is peach walking 0 is false and 1 is true: " << isWalking() << endl;
        setCoinsStoleFromBank(true);
        setCoinsDeposited(true);// so that the bank doesn't deduct coins right after it gets stolen
    }
    if (getWorld()->isBankSquare(getX(), getY()) && coinsDeposited() == false)
    {
        depositCoins();
        //cout << "current coins " << getCoins() << endl;
        //cout << "Is peach walking 0 is false and 1 is true: " << isWalking() << endl;
        setCoinsDeposited(true);
    }
    if (getWorld()->isDirSquare(getX(), getY()) && p_isOnDirSq == false)
    {

        //changeMovDir(getWorld()->whatDirSquare(getX(), getY()));
        //moveTo(getX() - moveX, getY() - moveY);
        setIsOnDirSq(true);
        
        //cout << "Is peach walking 0 is false and 1 is true: " << isWalking() << endl;
    }
    if (isWalking() == false && getWorld()->isEventSquare(getX(), getY()) && eventCompleted() == false)
    {
        int rand = randInt(1,3);
        if(rand == 1)
            teleportToRandSq();
        else if(rand == 2)
            swapPlayers();
        else if(rand == 3)
            setVortex(true);
        //cout << "current coins " << getCoins() << endl;
        p_eventCompleted = true;
        //cout << "Is peach walking 0 is false and 1 is true: " << isWalking() << endl;
        // so that the bank doesn't deduct coins right after it gets stolen
    }
    if (isWalking() == false && getWorld()->isDroppingSquare(getX(), getY()) && itemsDropped() == false)
    {
        dropItems();
       // cout << "current coins " << getCoins() << endl;
        p_itemsDropped = true;
        //cout << "Is peach walking 0 is false and 1 is true: " << isWalking() << endl;
        // so that the bank doesn't deduct coins right after it gets stolen
    }
}
Yoshi::Yoshi(int x, int y, StudentWorld* world)
    :Player(IID_YOSHI, x, y, world)
{}
void Yoshi::doDiffStuff() {
    //cout << "yoshi is doing different things" << endl;
    Player::doDiffStuff(PlayerNum());
}
Baddie::Baddie(int imageID, int startX, int startY, StudentWorld* world)
    :Actor(imageID, startX, startY, 0, 0, 1.0, world)
{
    b_walking = false;
    b_pauseCounter = 180;
    b_eventCompleted = false;
    b_squaresToMove = 0;
    b_tickToMove =0 ;
    b_movDir = getDirection() ;
    b_isFork = false;
    b_whenToCheck = 0;
    b_justTeleported = false;
}
void Baddie::doDiffStuff()
{
    if (!isWalking())
    {
        if (getWorld()->isPeachAt(getX(), getY()) && b_eventCompleted == false)
        {

            if (canSwap())
            {
                
                int randj = randInt(1, 2);
                if(randj == 2)
                {
                    getWorld()->swapCoins();
                    getWorld()->playSound(SOUND_BOO_ACTIVATE);
                }
                if (randj == 1)
                {
                    getWorld()->swapStars();
                    getWorld()->playSound(SOUND_BOO_ACTIVATE);
                }
                b_eventCompleted = true;
            }

            int randi = randInt(1, 2);
            if (randi == 1)
            {
                
                if (canStomp())
                {
                    getWorld()->bowserStompP();
                    getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
                    //cout << "bowserStomp Yoshi " << endl;
                    b_eventCompleted = true;
                }
            }
        }
        if (getWorld()->isYoshiAt(getX(), getY()) && b_eventCompleted == false)
        {
            if (canSwap())
            {
                
                int randj = randInt(1, 2);
                if(randj == 2)
                {
                    getWorld()->swapCoins();
                    getWorld()->playSound(SOUND_BOO_ACTIVATE);
                }
                if (randj == 1)
                {
                    getWorld()->swapStars();
                    getWorld()->playSound(SOUND_BOO_ACTIVATE);
                }
                b_eventCompleted = true;
            }
            int rand = randInt(1, 2);
            if (rand == 2)
            {
                if (canStomp())
                {
                    getWorld()->bowserStompY();
                    getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
                    //cout << "bowserStomp Yoshi " << endl;
                    b_eventCompleted = true;
                }
            }
        }

        b_pauseCounter--;

        if (b_pauseCounter == 0)
        {
            b_squaresToMove = randInt(1, 10);
            setWalking(true);
            //cout << "bowser rolled: " << b_squaresToMove << endl;

            b_tickToMove = b_squaresToMove * 8;
            int key = randInt(1, 4);
            switch (key)
            {
            case 1:
                if (getWorld()->isEmptySpace(getX() - SPRITE_WIDTH, getY()))
                {
                    break;
                }
                else
                {
                    changeMovDir(180);
                    setDirection(180);
                    if (b_isFork)
                        b_isFork = false;

                }
                break;
            case 2:
               if (getWorld()->isEmptySpace(getX() + SPRITE_WIDTH, getY()))
                {
                    break;
                }
                else
                {
                    changeMovDir(0);
                    if (b_isFork)
                        b_isFork = false;
                   
                }
                break;
            case 3:
              if (getWorld()->isEmptySpace(getX(), getY() + SPRITE_HEIGHT))
                {
                    break;
                }
                else
                {
                    changeMovDir(90);
                    if (b_isFork)
                        b_isFork = false;
          
                }
                break;
            case 4:
                if (getWorld()->isEmptySpace(getX(), getY() - SPRITE_HEIGHT))
                {
                    break;
                }
                else
                {
                    changeMovDir(270);
                    if (b_isFork)
                        b_isFork = false;
              
                }
                break;

            }
           
        }
    }
    if (b_whenToCheck == 8)
    {
        checkFork();
        checkDirection();
        b_whenToCheck = 0;
    }
    if (isWalking())
    {
        b_whenToCheck++;
        int moveX = 0;
        int moveY = 0;
        switch (isMovDir())
        {
        case 0:
            moveX = 2;
            break;
        case 90:
            moveY = 2;
            break;
        case 180:
            moveX = -2;
            break;
        case 270:
            moveY = -2;
            break;
        }
        //cout << "direction peach is moving " << isMovDir() << endl;
        //cout << "moving in X " << moveX << endl;
        //cout << "moving in y " << moveY << endl;

        //cout << "How many ticks in between checks it has been " << returnWhenToCheck() << endl;
        moveTo(getX() + moveX, getY() + moveY);
        b_eventCompleted = false;
        b_tickToMove--;
        if (b_tickToMove == 0)
        {
            //setWalking(false);
            
            b_pauseCounter = 180;
            if (canStomp())
            {
                int rand = randInt(1, 4);
                if(rand == 4)
                {Actor* actorToDelete = getWorld()->specificObjectAt(getX(), getY());
                if(actorToDelete->is_a_square())
                    actorToDelete->changeActive(false); //basically has same effect as deleting but it will all get cleaned up later.
                    
                    getWorld()->newActor(new DroppingSquare(getX(), getY(), getWorld()));
                 }
                
            }
            setWalking(false);//cout << "bowser stoppped walking" << endl;
        }
    }
}

void Baddie::checkFork() // checks forks for baddies will try to move to common class with player to cooperate with oop rules after get it working 
{
    //cout << "checks for fork" << endl;
    int paths = 0;

    if (isMovDir() == 0) // Moving right
    {
        if (!getWorld()->isEmptySpace(getX(), getY() + SPRITE_HEIGHT)) //checks up
        {
            paths++;
        }
        if (!getWorld()->isEmptySpace(getX(), getY() - SPRITE_HEIGHT)) //checks down
        {
            paths++;
        }
        if (!getWorld()->isEmptySpace(getX() + SPRITE_WIDTH, getY())) // checks right
        {
            paths++;
        }
    }
    else if (isMovDir() == 180) // Moving left
    {
        if (!getWorld()->isEmptySpace(getX(), getY() + SPRITE_HEIGHT)) //checks up
        {
            paths++;
        }
        if (!getWorld()->isEmptySpace(getX(), getY() - SPRITE_HEIGHT)) //checks down
        {
            paths++;
        }
        if (!getWorld()->isEmptySpace(getX() - SPRITE_WIDTH, getY())) // checks left
        {
            paths++;
        }
    }
    else if (isMovDir() == 90) // Moving up
    {
        if (!getWorld()->isEmptySpace(getX() + SPRITE_WIDTH, getY())) //checks right
        {
            paths++;
        }
        if (!getWorld()->isEmptySpace(getX() - SPRITE_WIDTH, getY()))//checks left
        {
            paths++;
        }
        if (!getWorld()->isEmptySpace(getX(), getY() + SPRITE_HEIGHT))// checks up
        {
            paths++;
        }
    }
    else if (isMovDir() == 270) // Moving down
    {
        if (!getWorld()->isEmptySpace(getX() + SPRITE_WIDTH, getY()))
        {
            paths++;
        }
        if (!getWorld()->isEmptySpace(getX() - SPRITE_WIDTH, getY()))
        {
            paths++;
        }
        if (!getWorld()->isEmptySpace(getX(), getY() - SPRITE_HEIGHT))// checks up
        {
            paths++;
        }
    }
    if (paths > 1)
    {
        b_isFork = true;
        //cout << "Fork is found" << endl;
    }
}
void Baddie::checkDirection()
{
    // Find available space perpendicular to current direction of motion
   
    if (isMovDir() == 0) // Moving right
    {
        if (!getWorld()->isEmptySpace(getX(), getY() + SPRITE_HEIGHT))
        {
            changeMovDir(90);//change move direction to up
            setDirection(0);
        }
        else if (!getWorld()->isEmptySpace(getX(), getY() - SPRITE_HEIGHT))
        {
            changeMovDir(270);
            setDirection(0);
        }
        // Can't move in any direction, stop walking
    }
    else if (isMovDir() == 180) // Moving left
    {
        if (!getWorld()->isEmptySpace(getX(), getY() + SPRITE_HEIGHT))
        {
            changeMovDir(90);
            setDirection(0);
        }
        else if (!getWorld()->isEmptySpace(getX(), getY() - SPRITE_HEIGHT))
        {
            changeMovDir(270);
            setDirection(0);
        }
        // Can't move in any direction, stop walking
    }
    else if (isMovDir() == 90) // Moving up
    {
        if (!getWorld()->isEmptySpace(getX() + SPRITE_WIDTH, getY()))
        {
            changeMovDir(0);
            setDirection(0);
        }
        else if (!getWorld()->isEmptySpace(getX() - SPRITE_WIDTH, getY()))
        {
            changeMovDir(180);
            setDirection(180);
        }
    }
    else if (isMovDir() == 270) // Moving down
    {
        if (!getWorld()->isEmptySpace(getX() + SPRITE_WIDTH, getY()))
        {
            changeMovDir(0);
            setDirection(0);
        }
        else if (!getWorld()->isEmptySpace(getX() - SPRITE_WIDTH, getY()))
        {
            changeMovDir(180);
            setDirection(180);
        }
    }
}
Bowser::Bowser(int startX, int startY, StudentWorld* world)
    :Baddie(IID_BOWSER, startX, startY, world)
{

}

Boo::Boo(int startX, int startY, StudentWorld* world)
    :Baddie(IID_BOO, startX, startY, world)
{

}
Vortex::Vortex(int startX, int startY, int startDirection , StudentWorld* world)
    :Actor(IID_VORTEX, startX, startY, 0, 0, 1.0, world)
{
    v_movDir = startDirection;
}
void Vortex::doDiffStuff()
{
    if (isMovDir() == 0)
        moveTo(getX() + 2, getY());
    else if (isMovDir() == 90)
        moveTo(getX() , getY()+2);
    else if (isMovDir() == 180)
        moveTo(getX() - 2, getY());
    else if (isMovDir() == 270)
        moveTo(getX(), getY()-2);

    if (getX() < 0 || getX() > VIEW_WIDTH || getY() < 0 || getY() > VIEW_HEIGHT)
    {
        changeActive(false);
    }

    Actor* a = getWorld()->ActorAt(getX(), getY());
    if (a->isBad())
    {
        getWorld()->teleportToRandSquareB(a);
        getWorld()->playSound(SOUND_HIT_BY_VORTEX);
    }
}
