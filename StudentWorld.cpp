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
    m_socrates=nullptr;
    levelUp=0;
}
void StudentWorld::getPosInDish(double &x,double &y){
    x=randInt((VIEW_WIDTH/2)-120, (VIEW_WIDTH/2)+120);
    y=randInt((VIEW_HEIGHT/2)-120, (VIEW_HEIGHT/2)+120);
    while(sqrt((x-128)*(x-128)+(y-128)*(y-128))>120){
        x=randInt((VIEW_WIDTH/2)-120, (VIEW_WIDTH/2)+120);
        y=randInt((VIEW_HEIGHT/2)-120, (VIEW_HEIGHT/2)+120);
    }
}
void StudentWorld::getPosAroundDish(double &x, double &y){
    int positionAngle=randInt(0, 360);
    
    x=128+128*cos(positionAngle*PI/180);
    y=128+128*sin(positionAngle*PI/180);
}

bool StudentWorld::checkOverlap(double x, double y){
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
bool StudentWorld::virusCanMove(Actor *actor, double x, double y){
    list<Actor*>::iterator it;
    for(it=m_Actor.begin();it!=m_Actor.end();it++){
        if(*it==actor||!(*it)->isBlockable()){
            continue;
        }
        if(getSqrtOfDistance(x, y, (*it)->getX(), (*it)->getY())<=SPRITE_RADIUS){
            return false;
        }
    }
    if(getSqrtOfDistance(x, y, VIEW_WIDTH/2, VIEW_HEIGHT/2)>=VIEW_RADIUS){
        return false;
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
            if((*it)->canPlaySound()){
                if(!(*it)->isDead()){
                    (*it)->playHurtSound();
                }
                else{
                    
                    (*it)->playDeadSound();
                    increaseScore(100);
                    int temp=randInt(1, 2);
                    if(temp==1){
                        addActors(new food((*it)->getX(),(*it)->getY()));
                    }
                }
            }
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
            if(getSqrtOfDistance(actor->getX(), actor->getY(), (*it)->getX(), (*it)->getY())<=2*SPRITE_RADIUS)
            {
                (*it)->setDead();
                
                return true;
            }
        }
    
    return false;
}
int StudentWorld::virusCanFindFood(Actor *actor){
    double cloest=-1;
    int directionAngle=-1;
    double newX=-1;
    double newY=-1;
    list<Actor*>::iterator it;
    for(it=m_Actor.begin();it!=m_Actor.end();it++){
        if(*it==actor||!(*it)->isEdible()){
            continue;
        }
        if(getSqrtOfDistance(actor->getX(), actor->getY(), (*it)->getX(), (*it)->getY())<128)
        {
            if(cloest==-1||cloest>getSqrtOfDistance(actor->getX(), actor->getY(), (*it)->getX(), (*it)->getY()))
            {
                cloest=getSqrtOfDistance(actor->getX(), actor->getY(), (*it)->getX(), (*it)->getY());
                newX=(*it)->getX();
                newY=(*it)->getY();
            }
        }
    }
    if(newX==-1){
        return directionAngle;
    }
    if((actor->getY()-newY)<0){
        directionAngle=90-atan((actor->getX()-newX)/(actor->getY()-newY))*180/PI;
    }
    else if((actor->getY()-newY)>0){
        directionAngle=270-atan((actor->getX()-newX)/(actor->getY()-newY))*180/PI;
    }
    else{
        if((actor->getX()-newX)<0)
            directionAngle=0;
        else{
            directionAngle=180;
        }
    }
    return directionAngle;
}
int StudentWorld::getDirectionTowardSocrates(Actor *actor){
    int directionAngle;
    if(overlapWithSocrates(actor)){
        return actor->getDirection();
    }
    if((actor->getY()-m_socrates->getY())<0){
        directionAngle=90-atan((actor->getX()-m_socrates->getX())/(actor->getY()-m_socrates->getY()))*180/PI;
    }
    else if((actor->getY()-m_socrates->getY())>0){
        directionAngle=270-atan((actor->getX()-m_socrates->getX())/(actor->getY()-m_socrates->getY()))*180/PI;
    }
    else{
        if((actor->getX()-m_socrates->getX())<0)
            directionAngle=0;
        else{
            directionAngle=180;
        }
    }
    return directionAngle;
}
bool StudentWorld::overlapWithSocrates(Actor *actor){
    if(getSqrtOfDistance(actor->getX(), actor->getY(), m_socrates->getX(), m_socrates->getY())<=2*SPRITE_RADIUS)
    {
        return true;
    }
    return false;
}
bool StudentWorld::virusCloseToSocrates(Actor *actor,int distance){
    if(getSqrtOfDistance(actor->getX(), actor->getY(), m_socrates->getX(), m_socrates->getY())<=distance)
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
        double x;
        double y;
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
        double x=0;
        double y=0;
        while(!checkOverlap(x, y)){
            getPosInDish(x, y);
        }
        addActors(new food(x,y));
    }
}
void StudentWorld::createPit(){
    int pitNum=getLevel();
    for(int i=0;i<pitNum;i++){
        double x=0;
        double y=0;
        while(!checkOverlap(x, y)){
            getPosInDish(x, y);
        }
        addActors(new Pit(this,x,y));
    }
}

void StudentWorld::creatSpray(double x, double y, int dir){
    addActors(new Spray(this,x,y,dir));
}
void StudentWorld::creatFlame(double x, double y, int dir){
    addActors(new Flame(this,x,y,dir));
}
void StudentWorld::creatReg(double x, double y){
    addActors(new RegularSal(this,x,y));
}
void StudentWorld::creatAgg(double x, double y){
    addActors(new AggressiveSal(this,x,y));
}
void StudentWorld::creatEco(double x, double y){
    addActors(new Ecoli(this,x,y));
}
int StudentWorld::init()
{
    levelUp=0;
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
        double x;
        double y;
        getPosAroundDish(x, y);
        addActors(new Fungus(this, x, y));
    }
    int chanceGoodies=max(510 - getLevel() * 10, 250);
    if(randInt(0, chanceGoodies-1)==0){
        int goodieType=randInt(1, 10);
        double x;
        double y;
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
                decLives();
                playSound(SOUND_PLAYER_DIE);
                delete m_socrates;
                return GWSTATUS_PLAYER_DIED;
            }
            if(canLevelUp()){
                playSound(SOUND_FINISHED_LEVEL);
                delete m_socrates;
                return GWSTATUS_FINISHED_LEVEL;
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
bool StudentWorld::isVirusKilled(){
    int num=0;
    list<Actor*>::iterator it;
    for(it=m_Actor.begin();it!=m_Actor.end();it++){
        if((*it)->canPlaySound())
        {
            num++;
        }
    }
    if(num==0){
        return true;
    }
    return false;
    
}
