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
bool StudentWorld::checkOverlap(int x, int y){
    if(x==0&&y==0){
        return false;
    }
    list<Actor*>::iterator it;
    for(it=m_Actor.begin();it!=m_Actor.end();it++){
        if(getSqrtOfDistance(x, y, (*it)->getX(), (*it)->getY())<2*SPRITE_RADIUS){
            return false;
        }
    }
    return true;
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
        addActors(new Pit(x,y));
    }
}
void StudentWorld::createItems(char ch, int x,int y,Direction dir){
    switch (ch) {
        case 'F':
            addActors(new Flame(x,y,dir));
            break;
        case 'S':
            addActors(new Spray(x,y,dir));
            break;
        default:
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

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    m_socrates->doSomething();
    list<Actor*>::iterator it;
    for(it=m_Actor.begin();it!=m_Actor.end();it++){
        (*it)->doSomething();
    }
    
    
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
