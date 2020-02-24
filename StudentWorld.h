#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GraphObject.h"
#include <string>
#include <list>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Socrates;
class Actor;
class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    
    void addActors(Actor *actor){m_Actor.push_back(actor);}
    void addNewActors();
    void getPosInDish(int &x,int&y);
    void getPosAroundDish(int &x,int&y);
    void socratesTakeDamage(int amount);
    void socratesAddFlame(int amount);
    void socratesRestoreHealth();
    void createDirt();
    void createFood();
    void createPit();
    void updateGameStatus();
    void deleteDeadActors();
    void createItems(char ch,int x,int y,Direction dir);
    void createVirus(char ch,int x,int y);
    bool checkOverlap(int x,int y);
    void determineDamageable(Actor *actor,int damage);
    bool determineEdible(Actor *actor);
    bool virusCanMove(Actor *actor);
    bool overlapWithSocrates(Actor *actor);
    double getSqrtOfDistance(int x,int y,double newX,double newY){return sqrt((x-newX)*(x-newX)+(y-newY)*(y-newY));}
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    virtual ~StudentWorld(){
        cleanUp();
    }

private:
    Socrates* m_socrates;
    list<Actor*> m_Actor;
};

#endif // STUDENTWORLD_H_
