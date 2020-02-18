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
    void getPosInDish(int &x,int&y);
    void createDirt();
    void createFood();
    void createPit();
    void createItems(char ch,int x,int y,Direction dir);
    bool checkOverlap(int x,int y);
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
