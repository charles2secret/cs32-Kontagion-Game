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
                m_StudentWorld->createItems('S', getX(), getY(), getDirection());
                break;
            }
                case KEY_PRESS_ENTER:
            {
                m_StudentWorld->createItems('F', getX(), getY(), getDirection());
                break;
            }
                default:
            {
                
            }
        }
    }
}

void Spray::doSomething(){
    moveAngle(getDirection(),SPRITE_RADIUS*2);
}
