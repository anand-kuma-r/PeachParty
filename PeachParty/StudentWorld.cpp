#include "StudentWorld.h"
#include "GameConstants.h"
#include "Board.h"// i added
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath)
{
    s_peach = nullptr;
    s_yoshi = nullptr;
    s_bankCoins = 0;
}
StudentWorld::~StudentWorld()
{
    for (vector<Actor*>::iterator it = s_actors.begin(); it != s_actors.end(); ++it) {
        delete (*it);
    }
    delete s_peach;// just copied cleanUp since i got overflow error when i directly call cleanUp().
    delete s_yoshi;
}

int StudentWorld::init()
{

    string board_file = assetPath() + "board0" +to_string(getBoardNumber()) + ".txt";
    Board::LoadResult result = bd.loadBoard(board_file);
    if (result == Board::load_fail_file_not_found)
        cerr << "Could not find board01.txt data file\n";
    else if (result == Board::load_fail_bad_format)
        cerr << "Your board was improperly formatted\n";
    else if (result == Board::load_success) {
        cerr << "Successfully loaded board\n";
        for (int x = 0; x < 16; x++)
        {
            for (int y = 0; y < 16; y++)
            {
                Board::GridEntry ge = bd.getContentsOf(x, y);
                int xPix = SPRITE_WIDTH * x;
                int yPix = SPRITE_HEIGHT * y;

                switch (ge) {
                case Board::blue_coin_square:

                    s_actors.push_back(new BlueCoinSquare(xPix, yPix, this));

                    break;
                case Board::red_coin_square:
                    s_actors.push_back(new RedCoinSquare(xPix, yPix, this));
                    break;
                case Board::star_square:
                    s_actors.push_back(new StarSquare(xPix, yPix, this));
                    break;
                case Board::up_dir_square:
                    
                    s_actors.push_back(new DirectionalSquare(xPix, yPix, 90, this));
                    break;
                case Board::down_dir_square:
                    s_actors.push_back(new DirectionalSquare(xPix, yPix, 270, this));
                    break;
                case Board::left_dir_square:
                    s_actors.push_back(new DirectionalSquare(xPix, yPix, 180, this));
                    break;
                case Board::right_dir_square:
                    s_actors.push_back(new DirectionalSquare(xPix, yPix, 0, this));
                    break;
                case Board::bank_square:
                    s_actors.push_back(new BankSquare(xPix, yPix, this));
                    break;
                case Board::event_square:
                    s_actors.push_back(new EventSquare(xPix, yPix, this));
                    break;
                case Board::player:

                    s_actors.push_back(new BlueCoinSquare(xPix, yPix, this));
                    s_peach = new Peach(xPix, yPix, this);
                    s_yoshi = new Yoshi(xPix, yPix, this);
                    break;
                case Board::bowser:
                    s_actors.push_back(new BlueCoinSquare(xPix, yPix, this));
                    s_actors.push_back(new Bowser( xPix, yPix, this));
                    break;
                case Board::boo:
                    s_actors.push_back(new BlueCoinSquare(xPix, yPix, this));
                    s_actors.push_back(new Boo(xPix, yPix, this));
                    break;
                }

            }
        }
    }
    //loadBoard(assetPath);
    startCountdownTimer(99);  // this placeholder causes timeout after 5 seconds
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    string vortex;
    if (s_peach->hasVortex())
        vortex = " VOR ";

    string timeLeft = "P1 Roll: " + to_string(s_peach->getRoll()) + " Stars: " + to_string(s_peach->getStars()) + " $$: " + to_string(s_peach->getCoins()) + vortex
        + " | Time: " + to_string(timeRemaining()) + " | Bank: " + to_string(get_bank_coins()) +
        +" | P1 Roll: " + to_string(s_peach->getRoll()) + " Stars: " + to_string(s_peach->getStars()) + " $$: " + to_string(s_peach->getCoins()) + vortex;
    setGameStatText(timeLeft);

    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.
    if (s_peach->isActive())
        s_peach->doSomething();
    if (s_yoshi->isActive())
        s_yoshi->doSomething();
    for (vector<Actor*>::iterator it = s_actors.begin(); it != s_actors.end(); ++it) {
        if ((*it)->isActive())
            (*it)->doSomething();
    }

    

    //P1 Roll: 3 Stars: 2 $$: 15 VOR | Time: 75 | Bank: 9 | P2 Roll: 0 Stars: 1 $$: 22 VOR

    if (timeRemaining() == 0)
    {
        if (s_peach->getCoins() + s_peach->getStars() * 20 >= s_yoshi->getCoins() + s_yoshi->getStars() * 20)
        {
            setFinalScore(s_peach->getStars(), s_peach->getCoins());
            return GWSTATUS_PEACH_WON;
        }
        if (s_peach->getCoins() + s_peach->getStars() * 20 < s_yoshi->getCoins() + s_yoshi->getStars() * 20)
        {
            setFinalScore(s_yoshi->getStars(), s_yoshi->getCoins());
            return GWSTATUS_YOSHI_WON;
        }
    }

    vector<Actor*>::iterator i = s_actors.begin();
    while (i != s_actors.end()) {
        if (!(*i)->isActive()) {
            delete* i;
            i = s_actors.erase(i);
        }
        else {
            i++;
        }
    }

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (vector<Actor*>::iterator it = s_actors.begin(); it != s_actors.end(); ++it) {
        delete (*it);
    }
    while (!s_actors.empty()) {
        s_actors.erase(s_actors.begin());
    }
    delete s_peach;
    delete s_yoshi;
}
Actor* StudentWorld::ActorAt(int x, int y)
{
    for (vector<Actor*>::iterator it = s_actors.begin(); it != s_actors.end(); it++)
    {
        int firstX = (*it)->getX();

        int secondX = x;
        int firstY = (*it)->getY();
        int secondY = y;
        if (abs(firstX - secondX) < SPRITE_WIDTH && abs(firstY - secondY) < SPRITE_HEIGHT && firstX <= secondX && firstY <= secondY) {
            return *it;
        }
    }

    return nullptr;
}
Actor* StudentWorld::specificObjectAt(int x, int y)
{
    for (vector<Actor*>::iterator it = s_actors.begin(); it != s_actors.end(); it++)
    {
        int firstX = (*it)->getX();

        int secondX = x;
        int firstY = (*it)->getY();
        int secondY = y;
        if (abs(firstX - secondX) == 0 && abs(firstY - secondY) == 0) {
            return *it;
        }
    }

    return nullptr;
}
bool StudentWorld::isEmptySpace(int x, int y) {
    Actor* object = ActorAt(x, y);
    return object == nullptr;
}
bool StudentWorld::isBlueCoinSquare(int x, int y)//checks is coords are that ofthe center of a BlueCoinSquare
{
    for (vector<Actor*>::iterator it = s_actors.begin(); it != s_actors.end(); it++)
    {
        if ((*it)->getX() == x && (*it)->getY() == y && (*it)->canGiveCoins())
        {
            return true;
        }
    }
    return false;
}
bool StudentWorld::isRedCoinSquare(int x, int y)//checks is coords are that of the center of a RedCoinSquare
{
    for (vector<Actor*>::iterator it = s_actors.begin(); it != s_actors.end(); it++)
    {
        if ((*it)->getX() == x && (*it)->getY() == y && (*it)->canTakeCoins())
        {
            return true;
        }
    }
    return false;
}
bool StudentWorld::isStarSquare(int x, int y)//checks is coords are that of the center of a StarSquare
{
    for (vector<Actor*>::iterator it = s_actors.begin(); it != s_actors.end(); it++)
    {
        if ((*it)->getX() == x && (*it)->getY() == y && (*it)->canGiveStars())
        {
            return true;
        }
    }
    return false;
}
bool StudentWorld::isBankSquare(int x, int y)//checks is coords are that of the center of a StarSquare
{
    for (vector<Actor*>::iterator it = s_actors.begin(); it != s_actors.end(); it++)
    {
        if ((*it)->getX() == x && (*it)->getY() == y && (*it)->canStealCoins())
        {
            return true;
        }
    }
    return false;
}
bool StudentWorld::isDirSquare(int x, int y)//checks is coords are that of the center of a StarSquare
{
    for (vector<Actor*>::iterator it = s_actors.begin(); it != s_actors.end(); it++)
    {
        if ((*it)->getX() == x && (*it)->getY() == y && (*it)->canChangeDir())
        {
            return true;
        }
    }
    return false;
}
bool StudentWorld::isEventSquare(int x, int y)//checks is coords are that of the center of a StarSquare
{
    for (vector<Actor*>::iterator it = s_actors.begin(); it != s_actors.end(); it++)
    {
        if ((*it)->getX() == x && (*it)->getY() == y && (*it)->canDoEvents())
        {
            return true;
        }
    }
    return false;
}
bool StudentWorld::isDroppingSquare(int x, int y)//checks is coords are that of the center of a Dropping Square
{
    for (vector<Actor*>::iterator it = s_actors.begin(); it != s_actors.end(); it++)
    {
        if ((*it)->getX() == x && (*it)->getY() == y && (*it)->canDoDropping())
        {
            return true;
           // cout << "dropping square detected!!" << endl;
        }
    }
    return false;
}
int StudentWorld::whatDirSquare(int x, int y)
{
    for (vector<Actor*>::iterator it = s_actors.begin(); it != s_actors.end(); it++)
    {
        if ((*it)->getX() == x && (*it)->getY() == y && (*it)->canChangeDir())
        {
            return (*it)->getDirection();
        }
        
    }
    return -1;
}
bool StudentWorld::isPeachAt(int x, int y)
{
    int peachX = s_peach->getX();
    int peachY = s_peach->getY();

    return peachX == x && peachY == y;
}
bool StudentWorld::isYoshiAt(int x, int y)
{
    int yoshiX = s_yoshi->getX();
    int yoshiY = s_yoshi->getY();

    return yoshiX == x && yoshiY == y;
}
void StudentWorld::DirSquareLandPeach(int dir)
{
    s_peach->changeMovDir(dir);
    if(dir==180)
        s_peach->setDirection(dir);
}
void StudentWorld::DirSquareLandYoshi(int dir)
{
    s_yoshi->changeMovDir(dir);
    if (dir == 180)
        s_yoshi->setDirection(dir);
}
void StudentWorld::teleportToRandSquare(Player* player)
{
    int numSquares = 0;
    for (vector<Actor*>::iterator it = s_actors.begin(); it != s_actors.end(); it++) {
        if ((*it)->is_a_square()) {
            numSquares++;
        }
    }
    int randomSquareIndex = randInt(0, numSquares - 1);
    int counter = 0;

    for (vector<Actor*>::iterator it = s_actors.begin(); it != s_actors.end(); it++)
    {
        if ((*it)->is_a_square())
        {
            if (counter == randomSquareIndex)
            {
                player->moveTo((*it)->getX(), (*it)->getY());
                //cout << "player teleported!!" << endl;
                player->setJustTeleported(true);
                return;
            }
            counter++;
        }
    }
}
void StudentWorld::teleportToRandSquareB(Actor* baddie)
{
    int numSquares = 0;
    for (vector<Actor*>::iterator it = s_actors.begin(); it != s_actors.end(); it++) {
        if ((*it)->is_a_square()) {
            numSquares++;
        }
    }
    int randomSquareIndex = randInt(0, numSquares - 1);
    int counter = 0;

    for (vector<Actor*>::iterator it = s_actors.begin(); it != s_actors.end(); it++)
    {
        if ((*it)->is_a_square())
        {
            if (counter == randomSquareIndex)
            {
                baddie->moveTo((*it)->getX(), (*it)->getY());
                //cout << "baddie teleported!!" << endl;
                //baddie->setJustTeleported(true);
                return;
            }
            counter++;
        }
    }
}
void StudentWorld::swapPlayers()
{
   int yx = s_yoshi->getX(); //swap coordinates
   int yy = s_yoshi->getY();
   s_yoshi->moveTo(s_peach->getX(), s_peach->getY());
   s_peach->moveTo(yx, yy);

   int yt = s_yoshi->getTicksToWalk(); // swap Ticks to walk
   s_yoshi->setTickstoWalk(s_peach->getTicksToWalk());
   s_yoshi->setTickstoWalk(yt);
   
   int ymd = s_yoshi->isMovDir(); //swap movDir
   s_yoshi->changeMovDir(s_peach->isMovDir());
   s_peach->changeMovDir(ymd);

   int ysd = s_yoshi->getDirection(); //swap movDir
   s_yoshi->setDirection(s_peach->getDirection());
   s_peach->setDirection(ysd);

   bool temp = s_yoshi->isWalking();
   s_yoshi->setWalking(s_peach->isWalking());
   s_peach->setWalking(temp);

  // cout << "Player swap completed!!" << endl;
   s_peach->setEventCompleted(true);
   s_yoshi->setEventCompleted(true);
   playSound(SOUND_PLAYER_TELEPORT);
}

 void StudentWorld::bowserStompP()
{
    s_peach->changeCoins(-s_peach->getCoins());
    s_peach->changeStars(-s_peach->getStars());
}
void StudentWorld::bowserStompY()
{
    s_yoshi->changeCoins(-s_peach->getCoins());
    s_yoshi->changeStars(-s_yoshi->getStars());
}
void StudentWorld::newActor(Actor* actor) {
    s_actors.push_back(actor);
}
void StudentWorld::swapCoins()
{
    int randj = randInt(1, 2);
    if (randj == 2)
    {
        int temp = s_yoshi->getCoins();
        s_yoshi->setCoins(s_peach->getCoins());
        s_peach->setCoins(temp);
        playSound(SOUND_BOO_ACTIVATE);
    }
    
}
void StudentWorld::swapStars()
{

}