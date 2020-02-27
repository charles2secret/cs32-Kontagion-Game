#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor: public GraphObject {
public:
    Actor(int imageID, double startX, double startY, Direction dir , int depth , double size ):GraphObject(imageID,startX,startY,dir,depth,size){}
    virtual void doSomething()=0;
    virtual bool isDead() const=0;
    virtual void setDead()=0;
    virtual bool isdamageable() const {return false;}
    virtual bool isEdible() const {return false;}
    virtual bool isBlockable() const {return false;}
    virtual bool isVirus() const{return false;}
    virtual bool isEcoli() const{return false;}
    virtual void takeDamage(int amount){if(amount==0)return;};
    virtual ~Actor(){}
private:
    
};

class Socrates: public Actor{
public:
    Socrates(StudentWorld *s):Actor(IID_PLAYER,0,128,0,0,1.0){m_StudentWorld=s;positionAngle=180;sprayNum=20;flameNum=5;hitPoints=100;}
    int getHealth()const{return hitPoints;}
    int getSprayNum()const{return sprayNum;}
    int getFlameNum()const{return flameNum;}
    void addFlame(int amount){flameNum+=amount;}
    void setDead(){hitPoints=0;}
    void restoreHealth(){hitPoints=100;}
    void takeDamage(int amount) {hitPoints-=amount;}
    virtual void doSomething();
    virtual bool isDead() const {return hitPoints<=0;}
private:
    StudentWorld *m_StudentWorld;
    int positionAngle;
    double PI = 4 * atan(1);
    int sprayNum;
    int flameNum;
    int hitPoints;
};

class Dirt:public Actor{
public:
    Dirt(double X,double Y):Actor(IID_DIRT,X,Y,0,1,1.0),hitPoints(1){}
    virtual void doSomething(){}
    virtual bool isDead()const {return hitPoints<=0;}
    void setDead(){hitPoints=0;}
    void takeDamage(int amount){hitPoints-=amount;}
    bool isdamageable() const {return true;}
    bool isBlockable() const{return true;}
    
private:
    int hitPoints;
};

class food:public Actor{
public:
    food(double X, double Y):Actor(IID_FOOD,X,Y,90,1,1.0 ),status(false){}
    virtual void doSomething(){}
    virtual bool isDead()const {return status;}
    void setDead(){status=true;}
    bool isEdible()const{return true;}
private:
    bool status;
};

class Pit:public Actor{
public:
    Pit(StudentWorld *s,double X,double Y):Actor(IID_PIT,X,Y,0,1,1.0),regularSalNum(5),aggSalNum(3),EcoliNum(2),status(false),m_studentWorld(s){}
    virtual void doSomething();
    bool emitAll()const;
    bool emit(int num);
    virtual bool isDead()const{return status;}
    void setDead(){status=true;}
    
private:
    int regularSalNum;
    int aggSalNum;
    int EcoliNum;
    bool status;
    StudentWorld *m_studentWorld;
};

class Weapon:public Actor{
public:
    Weapon(StudentWorld *s,int ID,double X,double Y,Direction dir,int distance,int damagePoint):Actor(ID,X,Y,dir,1,1.0),m_distanceTravel(distance){m_StudentWorld=s;m_damagePoints=damagePoint;
    }
    virtual void doSomething();
    int getDistance()const{return m_distanceTravel;}
    virtual void decDistance(int amount){m_distanceTravel-=amount;}
    virtual bool isDead()const{return m_distanceTravel<=0;}
    virtual int getDamage()const{return m_damagePoints;};
    void setDead(){m_distanceTravel=0;}
private:
    int m_distanceTravel;
    StudentWorld *m_StudentWorld;
    int m_damagePoints;
};

class Flame:public Weapon{
public:
    Flame(StudentWorld *s,double X,double Y,Direction dir):Weapon(s,IID_FLAME,X,Y,dir,40,5){}
};

class Spray:public Weapon{
public:
    Spray(StudentWorld *s,double X,double Y,Direction dir):Weapon(s,IID_SPRAY,X,Y,dir,120,2){}
};

class Goodies:public Actor{
public:
    Goodies(StudentWorld *world,int ID,double X,double Y,int score):Actor(ID,X,Y,0,1,1.0),hitPoints(1),m_score(score),m_studentWorld(world){getLifeTime();};
    bool isdamageable()const {return true;}
    void takeDamage(int amount) {hitPoints-=amount;}
    virtual bool isDead()const{return hitPoints<=0;}
    void setDead(){hitPoints=0;}
    void  getLifeTime() ;
    void decreseLifeTime(){m_lifeTime--;}
    int getScore() const{return m_score;}
    void doSomething();
    StudentWorld *getworld()const{return m_studentWorld;}
    virtual void performOwnStuff()=0;
    
private:
    int hitPoints;
    int m_lifeTime;
    int m_score;
    StudentWorld *m_studentWorld;
};

class ResHealthGoodie:public Goodies{
public:
    ResHealthGoodie(StudentWorld *s,double X,double Y):Goodies(s,IID_RESTORE_HEALTH_GOODIE,X,Y,250){}
    virtual void performOwnStuff();
};

class FlameThrowerGoodie:public Goodies{
public:
    FlameThrowerGoodie(StudentWorld *s,double X,double Y):Goodies(s,IID_FLAME_THROWER_GOODIE,X,Y,300){}
    virtual void performOwnStuff();
};
class ExtraLifeGoodie:public Goodies{
public:
    ExtraLifeGoodie(StudentWorld *s,double X,double Y):Goodies(s,IID_EXTRA_LIFE_GOODIE,X,Y,500){}
    virtual void performOwnStuff();
};
class Fungus:public Goodies{
public:
    Fungus(StudentWorld *s,double X,double Y):Goodies(s,IID_FUNGUS,X,Y,-50){}
    virtual void performOwnStuff();
};

class Virus:public Actor{
public:
    Virus(StudentWorld *s,int ID, double X,double Y,int hitPoints,int damagePoint):Actor(ID,X,Y,90,0,1.0),m_studentWorld(s),m_hitPoints(hitPoints),m_damagePoint(damagePoint),m_foodEaten(0),m_movementPlan(0){}
    bool isDead()const {return m_hitPoints<=0;}
    bool isVirus() const{return true;}
    void setDead(){m_hitPoints=0;}
    void doSomething();
    StudentWorld *getWorld()const{return m_studentWorld;}
    virtual char getName()=0;
    bool isdamageable() const {return true;}
    bool eatEnoughFood()const{return m_foodEaten>=3;}
    void takeDamage(int amount){m_hitPoints-=amount;}
    void allVirusDo();
    virtual bool isEcoli()const{return false;}
    
private:
    StudentWorld *m_studentWorld;
    int m_hitPoints;
    int m_damagePoint;
    int m_foodEaten;
    int m_movementPlan;
};
class RegularSal:public Virus{
public:
    RegularSal(StudentWorld*s,double X,double Y):Virus(s,IID_SALMONELLA,X,Y,4,1){}
    char getName(){return 'R';}
};
class AggressiveSal:public Virus{
public:
    AggressiveSal(StudentWorld*s,double X,double Y):Virus(s,IID_SALMONELLA,X,Y,10,2){}
    char getName(){return 'A';}
};
class Ecoli:public Virus{
public:
    Ecoli(StudentWorld*s,double X,double Y):Virus(s, IID_ECOLI,X,Y,5,4){}
    char getName(){return 'E';}
    virtual bool isEcoli()const{return true;}
};
#endif // ACTOR_H_
