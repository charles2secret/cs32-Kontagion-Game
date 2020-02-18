#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor: public GraphObject {
public:
    Actor(int imageID, double startX, double startY, Direction dir , int depth , double size ):GraphObject(imageID,startX,startY,dir,depth,size){}
    virtual void doSomething()=0;
private:
    
};

class Socrates: public Actor{
public:
    Socrates(StudentWorld *s):Actor(IID_PLAYER,0,128,0,0,1.0){m_StudentWorld=s;positionAngle=180;sprayNum=20;flameNum=5;hitPoints=100;}
    
    virtual ~Socrates(){}
    virtual void doSomething();
private:
    StudentWorld *m_StudentWorld;
    int positionAngle;
    const double PI = 4 * atan(1);
    int sprayNum;
    int flameNum;
    int hitPoints;
    int m_x;
    int m_y;
};

class Dirt:public Actor{
public:
    Dirt(int X,int Y):Actor(IID_DIRT,X,Y,90,1,1.0){}
    virtual void doSomething(){}
    
    virtual ~Dirt(){}

};

class food:public Actor{
public:
    food(int X, int Y):Actor(IID_FOOD,X,Y,90,1,1.0 ){}
    virtual void doSomething(){}
};

class Pit:public Actor{
public:
    Pit(int X,int Y):Actor(IID_PIT,X,Y,0,1,1.0),regularSalNum(5),aggSalNum(3),EcoliNum(2){}
    virtual void doSomething(){}
private:
    int regularSalNum;
    int aggSalNum;
    int EcoliNum;
};

class Weapon:public Actor{
public:
    Weapon(int ID,int X,int Y,Direction dir):Actor(ID,X,Y,dir,1,1.0){}
    virtual void doSomething(){}
};

class Flame:public Weapon{
public:
    Flame(int X,int Y,Direction dir):Weapon(IID_FLAME,X,Y,dir),m_distanceTravel(32){}
    virtual void doSomething(){}
private:
    int m_distanceTravel;
    
};
class Spray:public Weapon{
public:
    Spray(int X,int Y,Direction dir):Weapon(IID_SPRAY,X,Y,dir),m_distanceTravel(112){}
    virtual void doSomething();
private:
    int m_distanceTravel;
};
#endif // ACTOR_H_
