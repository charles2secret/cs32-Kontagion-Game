#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor: public GraphObject {
public:
    //constructor
    Actor(int imageID, double startX, double startY, Direction dir , int depth , double size ):GraphObject(imageID,startX,startY,dir,depth,size){}
    //Accessors
    virtual bool isdamageable() const {return false;}//This function serves the purpose to check if the specific object can be damaged by Socrates’ weapons.

    virtual bool isEdible() const {return false;}//This function serves the purpose to check if the specific object can be eaten by virus(aka: bacterias).

    virtual bool isBlockable() const {return false;}//This function serves the purpose to check if the specific object can block the movement of viruses.

    virtual bool canPlaySound() const{return false;}//This function serves the purpose to check if the specific object can play a sound when they are damaged. Also, this also serves to determine if we can level up.

    
    virtual bool isDead() const=0;//This function has the purpose to check if the object is dead or not, and it was mainly used by the StudentWorld’s functions, for example, move( ). If that object returns true from this function, this means that it should be deleted at the end of this tick. I defined it to be pure virtual because Actor will never be called on this function, it will only be called on specific derived classes.

    //Action
    virtual void doSomething()=0;
    //helpers
    virtual void setDead()=0;//This function has a purpose to set the specific object to isDead( ) status, and it was mainly used by the StudentWorld’s functions. I defined it to be pure virtual because Actor will never be called on this function, it will only be called on specific derived classes.

    virtual void takeDamage(int amount){if(amount==0)return;};//This function has a purpose to deduce a specific amount of damage to that object. Most of the time, it won’t be called. It will only be called when Socrates overlaps with a fungus or Viruses; weapons overlap with viruses or dirt. That’s why I defined it to be virtual because only a few of its derived object needs to override this function.

    virtual void playHurtSound(){return;}
    virtual void playDeadSound(){return;}//These two functions have a similar purpose to play hurt sound or dead sound when appropriate.I defined it virtual because only a few of its derived object needs to override this function.

    //Destructor
    virtual ~Actor(){}
private:
    
};

class Socrates: public Actor{
public:
    //constructor
    Socrates(StudentWorld *s):Actor(IID_PLAYER,0,128,0,0,1.0){m_StudentWorld=s;positionAngle=180;sprayNum=20;flameNum=5;hitPoints=100;}
    //accessors
    int getHealth()const{return hitPoints;}
    int getSprayNum()const{return sprayNum;}
    int getFlameNum()const{return flameNum;}//All the above functions return the private member variables of this class, these will be called only in student world to change the status at the top of the game.

    //mutators
    void addFlame(int amount){flameNum+=amount;}
    virtual void setDead(){hitPoints=0;}
    void restoreHealth(){hitPoints=100;}
    virtual void takeDamage(int amount) {hitPoints-=amount;}
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
    //constructor
    Dirt(double X,double Y):Actor(IID_DIRT,X,Y,0,1,1.0),hitPoints(1){}
    //Accessors
    virtual bool isDead()const {return hitPoints<=0;}
    virtual bool isdamageable() const {return true;}
    virtual bool isBlockable() const{return true;}
    //mutators
    virtual void setDead(){hitPoints=0;}
    virtual void doSomething(){}
    virtual void takeDamage(int amount){hitPoints-=amount;}
    
    
private:
    int hitPoints;
};

class food:public Actor{
public:
    //constructor
    food(double X, double Y):Actor(IID_FOOD,X,Y,90,1,1.0 ),status(false){}
    //Accessor
    bool isEdible()const{return true;}
    virtual bool isDead()const {return status;}
    //mutator
    void setDead(){status=true;}
    virtual void doSomething(){}
    
private:
    bool status;
};

class Pit:public Actor{
public:
    //constructor
    Pit(StudentWorld *s,double X,double Y):Actor(IID_PIT,X,Y,0,1,1.0),regularSalNum(5),aggSalNum(3),EcoliNum(2),status(false),m_studentWorld(s){}
    //Accessors
    virtual bool isDead()const{return status;}
    //mutators
    virtual void doSomething();
    virtual void setDead(){status=true;}
    
private:
    int regularSalNum;
    int aggSalNum;
    int EcoliNum;
    bool status;
    StudentWorld *m_studentWorld;
    bool emitAll()const;
    bool emit(int num);
};

class Weapon:public Actor{
public:
    //constructors
    Weapon(StudentWorld *s,int ID,double X,double Y,Direction dir,int distance,int damagePoint):Actor(ID,X,Y,dir,1,1.0),m_distanceTravel(distance){m_StudentWorld=s;m_damagePoints=damagePoint;
    }
    //accessors
    
    virtual bool isDead()const{return m_distanceTravel<=0;}
    //mutators
    virtual void setDead(){m_distanceTravel=0;}
    virtual void doSomething();
private:
    int m_distanceTravel;
    StudentWorld *m_StudentWorld;
    int m_damagePoints;
    int getDamage()const{return m_damagePoints;};
    int getDistance()const{return m_distanceTravel;}
    void decDistance(int amount){m_distanceTravel-=amount;}
};

class Flame:public Weapon{
public:
    Flame(StudentWorld *s,double X,double Y,Direction dir):Weapon(s,IID_FLAME,X,Y,dir,32,5){}
};

class Spray:public Weapon{
public:
    Spray(StudentWorld *s,double X,double Y,Direction dir):Weapon(s,IID_SPRAY,X,Y,dir,112,2){}
};

class Goodies:public Actor{
public:
    //constructor
    Goodies(StudentWorld *world,int ID,double X,double Y,int score):Actor(ID,X,Y,0,1,1.0),hitPoints(1),m_score(score),m_studentWorld(world){getLifeTime();};
    //accessors
    virtual bool isdamageable()const {return true;}
    int getScore() const{return m_score;}
    StudentWorld *getworld()const{return m_studentWorld;}
    //mutators
    virtual void performOwnStuff()=0;//This is a pure virtual function, because goodies will never be called on this function. Instead, different goodies have its own performOwnStuff function which I will specify later.

    void takeDamage(int amount) {hitPoints-=amount;}
    virtual void setDead(){hitPoints=0;}
    virtual void doSomething();
    virtual bool isDead()const{return hitPoints<=0;}
    
private:
    int hitPoints;
    int m_lifeTime;
    int m_score;
    StudentWorld *m_studentWorld;
    void  getLifeTime() ;
    void decreseLifeTime(){m_lifeTime--;}//These functions help this class to keep track of its life time to make sure it goes away on time.

};

class ResHealthGoodie:public Goodies{
public:
    ResHealthGoodie(StudentWorld *s,double X,double Y):Goodies(s,IID_RESTORE_HEALTH_GOODIE,X,Y,250){}
    virtual void performOwnStuff();
};

class FlameThrowerGoodie:public Goodies{
public:
    //constructor
    FlameThrowerGoodie(StudentWorld *s,double X,double Y):Goodies(s,IID_FLAME_THROWER_GOODIE,X,Y,300){}
    //mutator
    virtual void performOwnStuff();
};
class ExtraLifeGoodie:public Goodies{
public:
    //constructor
    ExtraLifeGoodie(StudentWorld *s,double X,double Y):Goodies(s,IID_EXTRA_LIFE_GOODIE,X,Y,500){}
    //mutator
    virtual void performOwnStuff();
};
class Fungus:public Goodies{
public:
    //constructor
    Fungus(StudentWorld *s,double X,double Y):Goodies(s,IID_FUNGUS,X,Y,-50){}
    //mutator
    virtual void performOwnStuff();
};

class Virus:public Actor{
public:
    //constructor
    Virus(StudentWorld *s,int ID, double X,double Y,int hitPoints,int damagePoint):Actor(ID,X,Y,90,0,1.0),m_studentWorld(s),m_hitPoints(hitPoints),m_damagePoint(damagePoint),m_foodEaten(0),m_movementPlan(0){}
    //accessors
    virtual bool isDead()const {return m_hitPoints<=0;}
    virtual bool canPlaySound() const{return true;}
    
    StudentWorld *getWorld()const{return m_studentWorld;}
    virtual bool isdamageable() const {return true;}
    //mutators
    virtual void takeDamage(int amount){m_hitPoints-=amount;}
    void allVirusDo();//All virus will check to see if it overlaps with Socrates, if so, gies specific damage to Socrates, then play SOUND_PLAYER_HURT;

    virtual void setDead(){m_hitPoints=0;}
    virtual void doSomething();
    virtual int performOwnStuff()=0;//This is a pure virtual function, because virus will never be called on this function. Instead, different viruses have its own performOwnStuff function which I will specify later.

    virtual void playHurtSound();
    virtual void playDeadSound();
    virtual void createOwn(double x,double y)=0;
    
private:
    StudentWorld *m_studentWorld;
    int m_hitPoints;
    int m_damagePoint;
    int m_foodEaten;
    int m_movementPlan;
    bool eatEnoughFood()const{return m_foodEaten>=3;}
    
};
class RegularSal:public Virus{
public:
    //constructor
    RegularSal(StudentWorld*s,double X,double Y):Virus(s,IID_SALMONELLA,X,Y,4,1){}
    //accessor
    virtual int performOwnStuff();
    virtual void createOwn(double x,double y);
};
class AggressiveSal:public Virus{
public:
    //constructor
    AggressiveSal(StudentWorld*s,double X,double Y):Virus(s,IID_SALMONELLA,X,Y,10,2){}
    //accessor
    virtual int performOwnStuff();
    virtual void createOwn(double x,double y);
};
class Ecoli:public Virus{
public:
    //constructor
    Ecoli(StudentWorld*s,double X,double Y):Virus(s, IID_ECOLI,X,Y,5,4){}
    //accessor
    
    virtual int performOwnStuff();
    virtual void playHurtSound();
    virtual void playDeadSound();
    virtual void createOwn(double x,double y);
};
#endif // ACTOR_H_
