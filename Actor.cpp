#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
void Socrates::doSomething(){
    int ch;
    
    if(m_StudentWorld->getKey(ch)){
        switch(ch){
            case KEY_PRESS_LEFT:
            {
                positionAngle+=5;
                moveTo(128+128*cos(positionAngle*PI/180),128+128*sin(positionAngle*PI/180));
                setDirection(positionAngle+180);
                break;
            }
                case KEY_PRESS_RIGHT:
            {
                positionAngle-=5;
                moveTo(128+128*cos(positionAngle*PI/180),128+128*sin(positionAngle*PI/180));
                setDirection(positionAngle+180);
                break;
            }
                case KEY_PRESS_SPACE:
            {
                if(sprayNum>0){
                    double x;
                    double y;
                    getPositionInThisDirection(getDirection(), SPRITE_RADIUS*2, x, y);
                    m_StudentWorld->createItems('S', x, y, getDirection());
                    m_StudentWorld->playSound(SOUND_PLAYER_SPRAY);
                    sprayNum--;
                }
                
                break;
            }
                case KEY_PRESS_ENTER:
            {
                if(flameNum>0){
                    for(int i=0;i<16;i++){
                        Direction tempDir=getDirection()+i*22;
                        double x;
                        double y;
                        getPositionInThisDirection(tempDir, 2*SPRITE_RADIUS, x, y);
                        m_StudentWorld->createItems('F', x, y, tempDir);
                        m_StudentWorld->playSound(SOUND_PLAYER_FIRE);
                    }
                    flameNum--;
                }
                
                break;
            }
                default:
            {
                
            }
        }
    }
    else{
        if(sprayNum<20){
            sprayNum++;
        }
    }
}

void Weapon::doSomething(){
    if(isDead()){
        return;
    }
    m_StudentWorld->determineDamageable(this,getDamage());
    moveAngle(getDirection(),SPRITE_RADIUS*2);
    decDistance(SPRITE_RADIUS*2);
    
}
int Goodies::getCurrentLevel() {
    return m_studentWorld->getLevel();
}
int Goodies::getLifeTime() {
    return max(rand() % (300 - 10 * m_studentWorld->getLevel()), 50);
}
void Goodies::doSomething(){
    if(isDead()){
        return;
    }
    
    if(m_studentWorld->overlapWithSocrates(this)){
        getworld()->increaseScore(getScore());
        setDead();
        getworld()->playSound(SOUND_GOT_GOODIE);
        performOwnStuff();
    }
    decreseLifeTime();
    if(m_lifeTime<=0){
        setDead();
    }
}

void ResHealthGoodie::performOwnStuff(){
    getworld()->getSocrates()->restoreHealth();
    return;
    
}
void FlameThrowerGoodie::performOwnStuff(){
    getworld()->getSocrates()->addFlame(5);
    return;
}
void ExtraLifeGoodie::performOwnStuff(){
    getworld()->incLives();
    return;
}
void Fungus::performOwnStuff(){
    getworld()->getSocrates()->takeDamage(20);
    return;
}
