#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"

#include <string>
#include <list>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Socrates;
class Actor;
class StudentWorld : public GameWorld
{
public:
    //constructor
    StudentWorld(std::string assetPath);
    //helpers
    void addActors(Actor *actor){m_Actor.push_back(actor);}//Add actors to the list data structrure I created

    void addNewActors();//Add different goodies at the end of each tich if it satisfies the random int requirement

    void getPosInDish(double &x,double &y);//Get a random position inside the petri dish for dirt, food and pit

    void getPosAroundDish(double &x,double &y);//Get a random position on the edge of the petri dish for different goodies

    void socratesTakeDamage(int amount);//Reduce Socrates’ hit points by a specific amount

    void socratesAddFlame(int amount);//Add Socrates’ number of flame by a specific amount

    void socratesRestoreHealth();//Restore Socrates’ health to 100

    void createDirt();
    void createFood();
    void createPit();//Create different objects when init() function is called

    void updateGameStatus();//Update the game status at the top of the game by calling functions in gameworld

    void deleteDeadActors();//Delete any dead actors at the end of each tick, by using iterator to check if the object in the list is dead.

    void creatSpray(double x,double y,int dir);
    void creatFlame(double x,double y,int dir);
    void creatReg(double x,double y);
    void creatAgg(double x,double y);
    void creatEco(double x,double y);
    //Create different objects by calling addActors function

    bool checkOverlap(double x,double y);//Check if two objects are overlap by computing their distance and check if it is less than or equal to a specific amount

    void determineDamageable(Actor *actor,int damage);
    bool determineEdible(Actor *actor);
    //Check if the object passed in can damage or eat other stuff by iterating through the list

    bool virusCanMove(Actor *actor,double x,double y);
    //Check if the virus passed in can move in this direction and doesn’t block by dirt

    void setStatus(){levelUp++;}
    bool isVirusKilled();//If all viruses are dead, return true

    bool canLevelUp(){return levelUp==getLevel()&&isVirusKilled();}
    //Check to see if the game can level up

    int virusCanFindFood(Actor *actor);
    bool virusCloseToSocrates(Actor *actor,int distance);
    bool overlapWithSocrates(Actor *actor);
    int getDirectionTowardSocrates(Actor *actor);
//Viruses use these functions to determine their movement, which I specified in Virus class.

    double getSqrtOfDistance(double x,double y,double newX,double newY)const{return sqrt((x-newX)*(x-newX)+(y-newY)*(y-newY));}//Return the ditance between two position

    //main functions
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    virtual ~StudentWorld(){
        cleanUp();
    }

private:
    Socrates* m_socrates;
    list<Actor*> m_Actor;
    double PI = 4 * atan(1);
    int levelUp;
};

#endif // STUDENTWORLD_H_
