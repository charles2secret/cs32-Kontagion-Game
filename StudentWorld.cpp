#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    
}
void StudentWorld::getPosInDish(int &x,int &y){
    x=randInt((VIEW_WIDTH/2)-120, (VIEW_WIDTH/2)+120);
    y=randInt((VIEW_HEIGHT/2)-120, (VIEW_HEIGHT/2)+120);
    while(sqrt((x-128)*(x-128)+(y-128)*(y-128))>120){
        x=randInt((VIEW_WIDTH/2)-120, (VIEW_WIDTH/2)+120);
        y=randInt((VIEW_HEIGHT/2)-120, (VIEW_HEIGHT/2)+120);
    }
}
void StudentWorld::getPosAroundDish(int &x, int &y){
    int positionAngle=randInt(0, 360);
    double PI = 4 * atan(1);
    x=128+128*cos(positionAngle*PI/180);
    y=128+128*sin(positionAngle*PI/180);
}

bool StudentWorld::checkOverlap(int x, int y){
    if(x==0&&y==0){
        return false;
    }
    list<Actor*>::iterator it;
    for(it=m_Actor.begin();it!=m_Actor.end();it++){
        if(getSqrtOfDistance(x, y, (*it)->getX(), (*it)->getY())<=2*SPRITE_RADIUS){
            return false;
        }
    }
    return true;
}
void StudentWorld::determineDamageable(Actor *actor,int damage){
    list<Actor*>::iterator it;
    for(it=m_Actor.begin();it!=m_Actor.end();it++){
        if(*it==actor||!(*it)->isdamageable()){
            continue;
        }
        if(getSqrtOfDistance(actor->getX(), actor->getY(), (*it)->getX(), (*it)->getY())<=2*SPRITE_RADIUS){
            (*it)->takeDamage(damage);
            actor->setDead();
            break;
        }
    }
    
}
bool StudentWorld::determineEdible(Actor *actor){
    list<Actor*>::iterator it;
        for(it=m_Actor.begin();it!=m_Actor.end();it++){
            if(*it==actor||!(*it)->isEdible()){
                continue;
            }
            if(getSqrtOfDistance(actor->getX(), actor->getY(), (*it)->getX(), (*it)->getY())<=2*SPRITE_RADIUS){
                actor->setDead();
                return true;
            }
        }
    return false;
}

bool StudentWorld::overlapWithSocrates(Actor *actor){
    if(getSqrtOfDistance(actor->getX(), actor->getY(), m_socrates->getX(), m_socrates->getY())<=2*SPRITE_RADIUS)
    {
        return true;
    }
    return false;
}
void StudentWorld::createDirt(){
    int dirtNum=180-20*getLevel();
    if(dirtNum<=20){
        dirtNum=20;
    }
    for(int i=0;i<dirtNum;i++){
        int x;
        int y;
        getPosInDish(x, y);
        
        addActors(new Dirt(x, y));
    }
}
void StudentWorld::createFood(){
    int foodNum=5*getLevel();
    if(foodNum>=25){
        foodNum=25;
    }
    for(int i=0;i<foodNum;i++){
        int x=0;
        int y=0;
        while(!checkOverlap(x, y)){
            getPosInDish(x, y);
        }
        addActors(new food(x,y));
    }
}
void StudentWorld::createPit(){
    int pitNum=getLevel();
    for(int i=0;i<pitNum;i++){
        int x=0;
        int y=0;
        while(!checkOverlap(x, y)){
            getPosInDish(x, y);
        }
        addActors(new Pit(this,x,y));
    }
}
void StudentWorld::createItems(char ch, int x,int y,Direction dir){
    switch (ch) {
        case 'F':
            addActors(new Flame(this,x,y,dir));
            break;
        case 'S':
            addActors(new Spray(this,x,y,dir));
            break;
        default:
            break;
    }
}
void StudentWorld::createVirus(char ch, int x, int y){
    switch(ch){
        case 'R':
            addActors(new RegularSal(this,x,y));
            break;
        case 'A':
            addActors(new AggressiveSal(this,x,y));
            break;
        case 'E':
            addActors(new Ecoli(this,x,y));
            break;
        
    }
}
int StudentWorld::init()
{
    m_socrates=new Socrates(this);
    createDirt();
    createFood();
    createPit();
    
    return GWSTATUS_CONTINUE_GAME;
}
void StudentWorld::updateGameStatus(){
    string temp="";
    if(getScore()<0){
        int tempScore=getScore()*(-1);
        if(tempScore<10){
            temp="-00000";
        }
        else if(tempScore<100){
            temp="-0000";
        }
        else if(tempScore<1000){
            temp="-000";
        }
        else if(tempScore<10000){
            temp="-00";
        }
        else if(tempScore<100000){
            temp="-0";
        }
        setGameStatText("Score: "+temp+to_string(tempScore)+"  "+"Level: "+to_string(getLevel())+"  "+"Lives: "+to_string(getLives())+"  "+"health: "+to_string(m_socrates->getHealth())+"  "+"Sprays: "+to_string(m_socrates->getSprayNum())+"  "+"Flames: "+to_string(m_socrates->getFlameNum()));
        return;
    }
    if(getScore()<10){
        temp="00000";
    }
    else if(getScore()<100){
        temp="0000";
    }
    else if(getScore()<1000){
        temp="000";
    }
    else if(getScore()<10000){
        temp="00";
    }
    else if(getScore()<100000){
        temp="0";
    }
    setGameStatText("Score: "+temp+to_string(getScore())+"  "+"Level: "+to_string(getLevel())+"  "+"Lives: "+to_string(getLives())+"  "+"health: "+to_string(m_socrates->getHealth())+"  "+"Sprays: "+to_string(m_socrates->getSprayNum())+"  "+"Flames: "+to_string(m_socrates->getFlameNum()));
}
void StudentWorld::deleteDeadActors(){
    list<Actor*>::iterator it;
    for(it=m_Actor.begin();it!=m_Actor.end();it++){
        if((*it)->isDead())
        {
            delete *it;
            it=m_Actor.erase(it);
        }
    }
}
void StudentWorld::addNewActors(){
    int chanceFungus=max(510 - getLevel() * 10, 200);
    if(randInt(0, chanceFungus-1)==0){
        int x;
        int y;
        getPosAroundDish(x, y);
        addActors(new Fungus(this, x, y));
    }
    int chanceGoodies=max(510 - getLevel() * 10, 250);
    if(randInt(0, chanceGoodies-1)==0){
        int goodieType=randInt(1, 10);
        int x;
        int y;
        getPosAroundDish(x, y);
        if(goodieType>=1&&goodieType<=6){
            addActors(new ResHealthGoodie(this,x,y));
        }
        else if(goodieType>=7&&goodieType<=9){
            addActors(new FlameThrowerGoodie(this,x,y));
        }
        else{
            addActors(new ExtraLifeGoodie(this,x,y));
        }
    }
}
int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    m_socrates->doSomething();
    list<Actor*>::iterator it;
    for(it=m_Actor.begin();it!=m_Actor.end();it++){
        if(!(*it)->isDead())
        {
            (*it)->doSomething();
            if(m_socrates->isDead()){
                playSound(SOUND_PLAYER_DIE);
                return GWSTATUS_PLAYER_DIED;
            }
        }
    }
    deleteDeadActors();
    addNewActors();
    updateGameStatus();
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    list<Actor*>::iterator it;
    for(it=m_Actor.begin();it!=m_Actor.end();){
        delete *it;
        it=m_Actor.erase(it);
    }
    delete m_socrates;
    
}
void StudentWorld::socratesTakeDamage(int amount){
    m_socrates->takeDamage(amount);
}
void StudentWorld::socratesAddFlame(int amount){
    m_socrates->addFlame(amount);
}
void StudentWorld::socratesRestoreHealth(){
    m_socrates->restoreHealth();
    
}
