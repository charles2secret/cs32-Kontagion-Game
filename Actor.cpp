#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
void Socrates::doSomething(){
    if(isDead()){
        return;
    }
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
                setDirection((positionAngle+180)%360);
                break;
            }
                case KEY_PRESS_SPACE:
            {
                if(sprayNum>0){
                    double x;
                    double y;
                    getPositionInThisDirection(getDirection(), SPRITE_RADIUS*2, x, y);
                    m_StudentWorld->creatSpray( x, y, getDirection());
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
                        m_StudentWorld->creatFlame( x, y, tempDir);
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
    if(isDead()){
        return;
    }
    moveAngle(getDirection(),SPRITE_RADIUS*2);
    decDistance(SPRITE_RADIUS*2);
   
}

void Goodies::getLifeTime() {
    m_lifeTime= max(randInt(0, 300 - 10 * m_studentWorld->getLevel()-1), 50);
}
void Goodies::doSomething(){
    if(isDead()){
        return;
    }
    
    if(m_studentWorld->overlapWithSocrates(this)){
        getworld()->increaseScore(getScore());
        setDead();
        
        performOwnStuff();
        return;
    }
    decreseLifeTime();
    if(m_lifeTime<=0){
        setDead();
    }
}

void ResHealthGoodie::performOwnStuff(){
    getworld()->playSound(SOUND_GOT_GOODIE);
    getworld()->socratesRestoreHealth();
    return;
    
}
void FlameThrowerGoodie::performOwnStuff(){
    getworld()->playSound(SOUND_GOT_GOODIE);
    getworld()->socratesAddFlame(5);
    return;
}
void ExtraLifeGoodie::performOwnStuff(){
    getworld()->playSound(SOUND_GOT_GOODIE);
    getworld()->incLives();
    return;
}
void Fungus::performOwnStuff(){
    getworld()->socratesTakeDamage(20);
    getworld()->playSound(SOUND_PLAYER_HURT);
    return;
}
void Pit::doSomething(){
    if(emitAll()){
        setDead();
        m_studentWorld->setStatus();
        return;
    }
    else{
        int emitOrNot=randInt(1, 50);
        if(emitOrNot==36){
            int emitWhich=0;
            while(!emit(emitWhich)){
                emitWhich=randInt(1, 10);
            }
            m_studentWorld->playSound(SOUND_BACTERIUM_BORN);
        }
    }
}
bool Pit::emitAll()const{
    if((regularSalNum==0)&&(aggSalNum==0)&&(EcoliNum==0)){
        return true;
    }
    return false;
}
bool Pit::emit(int num){
    if(num==0){
        return false;
    }
    if(num>=1&&num<=5){
        if(regularSalNum>0){
            m_studentWorld->creatReg(getX(), getY());
            regularSalNum--;
            return true;
        }
    }
    else if(num>=6&&num<=8){
        if(aggSalNum>0){
            m_studentWorld->creatAgg(getX(), getY());
            aggSalNum--;
            return true;
        }
    }
    else if(num==9||num==10){
        if(EcoliNum>0){
            m_studentWorld->creatEco(getX(), getY());
            EcoliNum--;
            return true;
        }
    }
    return false;
}
void Virus::doSomething(){
    if(isDead()){
        return;
    }
    int i=performOwnStuff();
    if(i==1){
        return;
    }
    if(m_movementPlan>0){
        m_movementPlan--;
        double x;
        double y;
        getPositionInThisDirection(getDirection(), 3, x, y);
        if(m_studentWorld->virusCanMove(this, x, y)){
            moveAngle(getDirection(),3);
        }
        else{
            int newDirection=randInt(0, 359);
            setDirection(newDirection);
            m_movementPlan=10;
        }
        return;
    }
    else{
        int directionGo=m_studentWorld->virusCanFindFood(this);
        
        if(directionGo==-1){
            int newDirection=randInt(0, 359);
            setDirection(newDirection);
            m_movementPlan=10;
            return;
        }
        else{
            double x;
            double y;
            getPositionInThisDirection(directionGo, 3, x, y);
            if(m_studentWorld->virusCanMove(this, x, y)){
                moveAngle(directionGo,3);
                setDirection(directionGo);
            }
            else{
                int newDirection=randInt(0, 359);
                setDirection(newDirection);
                m_movementPlan=10;
            }
            return;
        }
    }
    
}
void Virus::allVirusDo(){
    if(m_studentWorld->overlapWithSocrates(this)){
        m_studentWorld->socratesTakeDamage(m_damagePoint);
        m_studentWorld->playSound(SOUND_PLAYER_HURT);
       
    }
    else if(eatEnoughFood()){
        double newX=getX();
        double newY=getY();
        if(getX()<VIEW_WIDTH/2){
            newX=getX()+SPRITE_RADIUS;
        }
        else if(getX()>VIEW_WIDTH/2){
            newX=getX()-SPRITE_RADIUS;
        }
        if(getY()<VIEW_WIDTH/2){
            newY=getY()+SPRITE_RADIUS;
        }
        else if(getY()>VIEW_WIDTH/2){
            newY=getY()-SPRITE_RADIUS;
        }
        createOwn(newX, newY);
        m_foodEaten=0;
    }
    else if(m_studentWorld->determineEdible(this)){
      
        m_foodEaten++;
    }
}
int RegularSal::performOwnStuff(){
    allVirusDo();
    return 0;
}
int AggressiveSal::performOwnStuff(){
    if(getWorld()->virusCloseToSocrates(this,72)){
        Direction temp=getWorld()->getDirectionTowardSocrates(this);
        double x;
        double y;
        getPositionInThisDirection(temp, 3, x, y);
        if(getWorld()->virusCanMove(this, x, y)){
            setDirection(temp);
            moveAngle(getDirection(),3);
        }
        setDirection(temp);
        allVirusDo();
        return 1;
    }
    allVirusDo();
    return 0;
}
int Ecoli::performOwnStuff(){
    allVirusDo();
    if(getWorld()->virusCloseToSocrates(this, 256)){
        Direction temp=getWorld()->getDirectionTowardSocrates(this);
        for(int i=0;i<10;i++){
            double x;
            double y;
            getPositionInThisDirection(temp, 2, x, y);
            if(getWorld()->virusCanMove(this, x, y)){
                setDirection(temp);
                moveAngle(getDirection(),2);
                return 1;
            }
            else{
                temp=(temp+10)%360;
            }
        }
    }
    return 1;
}
void Virus::playHurtSound(){
    m_studentWorld->playSound(SOUND_SALMONELLA_HURT);
}
void Ecoli::playHurtSound(){
    getWorld()->playSound(SOUND_ECOLI_HURT);
}
void Virus::playDeadSound(){
    m_studentWorld->playSound(SOUND_SALMONELLA_DIE);
}
void Ecoli::playDeadSound(){
    getWorld()->playSound(SOUND_ECOLI_DIE);
}
void RegularSal::createOwn(double x, double y){
    getWorld()->creatReg(x, y);
}
void AggressiveSal::createOwn(double x, double y){
    getWorld()->creatAgg(x, y);
}
void Ecoli::createOwn(double x, double y){
    getWorld()->creatEco(x, y);
}
