#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include "GameWorld.h"
using namespace std;


int counter = 0;



//// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
//

Actor::Actor(int IDPLAYAER, int startX, int startY, int dir, int depth, StudentWorld* ptr)  //Add depth
:GraphObject(IDPLAYAER, startX, startY, dir, depth){
    setState(true);
    setWorld(ptr);
    setOverlapD(false);
    setPenepOrCitz(false);
    setVomitStatus(false);
    setIfVomit(false);
    setIfFlameOverlap(true);
    setisInfectedbyFlame(false);
    numVaccines = 0;
    numLandmines = 0;
    numFlames = 0;
    
    setIfZombie(false);
}

void Actor::setIfVomit(bool setter){
    ifAbleToBeVomited = setter;
}
bool Actor::getIffVomit(){
    return ifAbleToBeVomited;
}

bool Actor::getVomitStatus(){
    return vomitStatus;
}

void Actor::setVomitStatus(bool setter){
    vomitStatus = setter;
}



void Actor::setState(bool state){
    stateOfLife = state;
}
void Actor::setPenepOrCitz(bool parem){
    penepOrCitiz = parem;
}

bool Actor::getPenepOrCitzizen(){
    return penepOrCitiz;
}

bool Actor::getState(){
    return stateOfLife;
}

void Actor::setOverlap(bool overlap){
    overlapCond = overlap;
}

bool Actor::getOverlap(){
    return overlapCond;
}


//Wall branch
Wall::Wall(int startX, int startY , StudentWorld* ptr, int ID)
:Actor(ID, startX, startY, right, 0, ptr){
    //A wall can never have a state of life that is false
    setOverlap(false);
    setBlock(true);
    setOverlapD(false);
}


////Exit branch

Exit::Exit(int startX, int startY, StudentWorld* ptr, int ID)
:Actor(ID, startX, startY, right, 1, ptr){
    setBlock(false);
    setOverlap(true);
    setIfFlameOverlap(false);

}

void Exit::doSomething(){
    //If exit overlaps with a citizen increase points by 500
    //Set object's state to dead, in which the move function in studentworld will eliminate that object
    //Play the sound that the citizen was saved by using the exit SOUND_CITIZEN_SAVED
    
    
    //If the exit overlaps with penelope(make a function in studentworld and pass in exit as a parameter)
    if(getWorld()->determineExit(this)){
        //If all citizens died then we end the level

        setLevel(true);
    }
    return;
}


////Immovable branch
immovable::immovable(int startX, int startY, StudentWorld* ptr, int ID, int depth)
:Actor(ID, startX, startY, right, depth, ptr){
    setOverlap(true);
    setBlock(false);
    setOverlapD(true);
    setIfFlameOverlap(false);

}

Pit::Pit(int startX, int startY, StudentWorld* ptr, int ID, int depth)
:immovable(startX, startY, ptr, ID, depth){
}

void Pit::doSomething(){
    if(getWorld()->determineOverlapP(this)){
        
        //Have to account for the fact that a pit blocks flames
        
        //Literally just loses 1000 points because pits act the same if penelope or a citizen dies
    }
}

Landmine::Landmine(int startX, int startY, StudentWorld* ptr, int ID, int depth)
:immovable(startX, startY, ptr, ID, depth){
    safetyTicks = 30;
    setIfFlameOverlap(true);
}

void Landmine::doSomething(){
    if(!getState())
        return;
    if(safetyTicks > 0){
        safetyTicks--;
        return;
    }
    if((getWorld()->determineOverlapWithZombiesAndPlayers(this)) && safetyTicks <= 0){
        setState(false);
        getWorld()->playSound(SOUND_LANDMINE_EXPLODE);
        
        getWorld()->makeMine(getX(), getY());
        getWorld()->makeFlames(getX(), getY(), 1);
        getWorld()->makeFlames(getX(), getY() + SPRITE_HEIGHT - 1, 1);
        getWorld()->makeFlames(getX(), getY() - (SPRITE_HEIGHT - 1), 1);
        getWorld()->makeFlames(getX() - SPRITE_WIDTH - 1, getY() + (SPRITE_HEIGHT - 1), 1);
        getWorld()->makeFlames(getX() - SPRITE_WIDTH - 1, getY() - (SPRITE_HEIGHT - 1), 1);
        getWorld()->makeFlames(getX() - SPRITE_WIDTH - 1, getY(), 1);
        getWorld()->makeFlames(getX() - 2*(SPRITE_WIDTH - 1), getY(), 1);
        getWorld()->makeFlames(getX() - 2*(SPRITE_WIDTH - 1), getY() + (SPRITE_HEIGHT - 1), 1);
        getWorld()->makeFlames(getX() - 2*(SPRITE_WIDTH - 1), getY() - (SPRITE_HEIGHT - 1), 1);

        //Make a pit in the middle
    }
//    if(getifIsInfectedCuurentlyByFlame() && safetyTicks <= 0){
//        setState(false);
//        getWorld()->playSound(SOUND_LANDMINE_EXPLODE);
//        
//        getWorld()->makeMine(getX(), getY());
//        getWorld()->makeFlames(getX(), getY(), 1);
//        getWorld()->makeFlames(getX(), getY() + SPRITE_HEIGHT - 1, 1);
//        getWorld()->makeFlames(getX(), getY() - (SPRITE_HEIGHT - 1), 1);
//        getWorld()->makeFlames(getX() - SPRITE_WIDTH - 1, getY() + (SPRITE_HEIGHT - 1), 1);
//        getWorld()->makeFlames(getX() - SPRITE_WIDTH - 1, getY() - (SPRITE_HEIGHT - 1), 1);
//        getWorld()->makeFlames(getX() - SPRITE_WIDTH - 1, getY(), 1);
//        getWorld()->makeFlames(getX() - 2*(SPRITE_WIDTH - 1), getY(), 1);
//        getWorld()->makeFlames(getX() - 2*(SPRITE_WIDTH - 1), getY() + (SPRITE_HEIGHT - 1), 1);
//        getWorld()->makeFlames(getX() - 2*(SPRITE_WIDTH - 1), getY() - (SPRITE_HEIGHT - 1), 1);
//    }
}






//Goodie branch

Goodie::Goodie(int startX, int startY, StudentWorld* ptr, int ID, int depth)
:Actor(ID, startX, startY, right, 1, ptr){
    setOverlap(true);
    setBlock(false);
    setOverlapD(false);
}

void Goodie::goodieOverlap(){
    if(getWorld()->isPlayerCloseEnoughToZombie(this)){
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(50);
        setState(false);
    }
}


//Gas can goodie

GasCanGoodie::GasCanGoodie (int startX, int startY, StudentWorld* ptr, int ID, int depth)
:Goodie(startX, startY, ptr, ID, depth){
    
}

void GasCanGoodie::doSomething(){
    if(getifIsInfectedCuurentlyByFlame()){
        setState(false);
        return;
    }
    
    if(getState()){
        goodieOverlap();
        if(!getState()){
            //Tells object that we have flames now
            getWorld()->increaseCountFlames(5);
        }

    }
    
        return;
}

VaccineGoodie::VaccineGoodie(int startX, int startY, StudentWorld* ptr, int ID, int depth)
:Goodie(startX, startY, ptr, ID, depth){
}

void VaccineGoodie::doSomething(){
    if(getifIsInfectedCuurentlyByFlame()){
        setState(false);
    }
    if(getState()){
        goodieOverlap();
        if(!getState()){
            getWorld()->incrementVaccines();
            //Tells object that we have Vaccines now
        }
        

    }
    return;
}

LandmineGoodie::LandmineGoodie(int startX, int startY, StudentWorld* ptr, int ID, int depth)
:Goodie(startX, startY, ptr, ID, depth){
}

void LandmineGoodie::doSomething(){
    if(getState()){
        goodieOverlap();
        if(!getState()){
            //Tells object that we have landmines now
            getWorld()->increaseLandmineCount(2);
        }
        if(getifIsInfectedCuurentlyByFlame()){
            setState(false);
        }
    }
    return;
}

//Movable objects flames or vomits

Movable::Movable(int startX, int startY, StudentWorld* ptr, int ID, int depth, int dir)
:Actor(ID, startX, startY, dir, depth, ptr)
{
    setBlock(false);
    setIfFlameOverlap(false);

}

Flame::Flame(int startX, int startY, StudentWorld* ptr, int ID, int depth, int dir)
:Movable(startX, startY, ptr, ID, depth, dir)
{
    tickNumFlame = 0;
    setOverlap(true);
}

void Flame::doSomething(){
    tickNumFlame++;
    
    if(tickNumFlame == 2){
        tickNumFlame = 0;
        setState(false);
        return;
    }
    if(getState())
        getWorld()->determineOverlapWithFlames(this);

}

Vomit::Vomit(int startX, int startY, StudentWorld* ptr, int ID, int depth, int dir)
:Movable(startX, startY, ptr, ID, depth, dir)
{
    setBlock(true);
    setIfVomit(true);
    tickAlive = 0;
}

void Vomit::doSomething(){
    tickAlive++;
    getWorld()->isCitizenCloseEnoughToZombie(this);
    if(getWorld()->isPlayerCloseEnoughToZombie(this))
        getWorld()->infectPlayer();
    
    if(tickAlive == 2){
        tickAlive = 0;
        setState(false);
    }
}



//Person branch

Person::Person(int ID, int stX, int stY, int dir, StudentWorld* ptr)
:Actor(ID, stX, stY, dir, 0, ptr)
{
    setIfFlameOverlap(true);
}

Citizen::Citizen(int startX, int startY, StudentWorld* ptr, int ID)
:Person(ID, startX, startY, right, ptr)
{
    setBlock(true);
    setPenepOrCitz(true);
    setOverlap(false);
    setOverlapD(false);
    setIfVomit(true);
    infectionCount = 0;
    infectionStatus = false;
    paralyzedCounter = 0;
}

void Citizen::doSomething(){
    if(getifIsInfectedCuurentlyByFlame()){
        setState(false);
    }
    paralyzedCounter++;
    if(!getState())
        return;
    if(infectionCount == 500){
        setState(false);
        getWorld()->increaseScore(-1000);
        getWorld()->playSound(SOUND_ZOMBIE_BORN);
        int possibilityZombieBorn = randInt(1, 10);
        if(possibilityZombieBorn < 3){
            getWorld()->makeSmartZombie(getX(), getY());
            //Make smart zombie object
        }else{
            getWorld()->makeDumbZombie(getX(), getY());
            //Make dumb zombie object
        }
        return;
        
    }
    if(paralyzedCounter % 2 == 0)
        return;
    if(infectionStatus)
        infectionCount++;
    
    
    int distToP = getWorld()->distanceToPenelope(this);
    int distToZ = getWorld()->distanceToZombie(getX(), getY());
    
    
    if((distToZ > distToP || distToZ == -1) && distToP < 80){

        if(getWorld()->isPenelopeOnTheSameRowOrColAsZombie(this)){
            int dirOfPen;
            
            if(getWorld()->getXPl() > getX())
                dirOfPen = 0;
            else if(getWorld()->getXPl() < getX())
                dirOfPen = 180;
            else if(getWorld()->getYPl() > getY())
                dirOfPen = 90;
            else
                dirOfPen = 270;
            
            
            
            
            if(getWorld()->determineBlock(this, dirOfPen, 2)){
                setDirection(dirOfPen);
                int dir = getDirection();
                if(dir == 0)
                    moveTo(getX() + 2, getY());
                else if(dir == 180)
                    moveTo(getX() - 2, getY());
                else if(dir == 90)
                    moveTo(getX(), getY() + 2);
                else if(dir == 270)
                    moveTo(getX(), getY() - 2);
                return;
            }
        }
        else{
            int dirX = -1;
            int dirY = -1;
            if(getWorld()->getXPl() > getX())
                dirX = 0;
            else
                dirX = 180;
            
            if(getWorld()->getYPl() > getY())
                dirY = 90;
            else
                dirX = 270;

            int rand = randInt(1,2);
            int finalDirection;
            int otherDirection;
            if(rand == 1){
                finalDirection = dirX;
                otherDirection = dirY;
            }
            else{
                finalDirection = dirY;
                otherDirection = dirX;
            }

            if(getWorld()->determineBlock(this, finalDirection, 4)){
                setDirection(finalDirection);
                int dir = getDirection();
                if(dir == 0)
                    moveTo(getX() + 2, getY());
                else if(dir == 180)
                    moveTo(getX() - 2, getY());
                else if(dir == 90)
                    moveTo(getX(), getY() + 2);
                else if(dir == 270)
                    moveTo(getX(), getY() - 2);
                return;
            }
            else if(getWorld()->determineBlock(this, otherDirection, 4)){
                setDirection(otherDirection);
                int dir = getDirection();
                if(dir == 0)
                    moveTo(getX() + 2, getY());
                else if(dir == 180)
                    moveTo(getX() - 2, getY());
                else if(dir == 90)
                    moveTo(getX(), getY() + 2);
                else if(dir == 270)
                    moveTo(getX(), getY() - 2);
                return;
            }

        }
    }
    else if(distToZ < 80){
        int newLocX, newLocY;
        int xDir;
        newLocX = min(getWorld()->distanceToZombie(getX() + 2, getY()), getWorld()->distanceToZombie(getX()-2, getY()));
        
        if(newLocX == getWorld()->distanceToZombie(getX() + 2, getY()))
           xDir = 0;
        else
            xDir = 180;
        
        int yDir;
        newLocY = min(getWorld()->distanceToZombie(getX(), getY() + 2), getWorld()->distanceToZombie(getX(), getY() - 2));
        if(newLocX == getWorld()->distanceToZombie(getX(), getY() + 2))
            yDir = 90;
        else
            yDir = 270;
        
        if(newLocX <= 80)
            newLocX = -1;
        if(newLocY <= 80)
            newLocY = -1;
        if(newLocY == -1 && newLocX == -1)
            return;
        

        if(newLocX == -1){
            if(xDir == 0)
                moveTo(getX() + 2, getY());
            else
                moveTo(getX() - 2, getY());
        }
        else{
            if(xDir == 90)
                moveTo(getX(), getY() + 2);
            else
                moveTo(getX(), getY()  - 2);
        }
        
            
    }
}

Penelope::Penelope(int startX, int startY, StudentWorld* ptr, int ID)
:Person(ID, startX, startY, right, ptr)
{
    setBlock(true);
    setPenepOrCitz(true);
    setOverlap(true);
    setOverlapD(true);
    setIfVomit(true);
    setIfFlameOverlap(true);
}

void Penelope::doSomething(){
    if(getifIsInfectedCuurentlyByFlame()){
        setState(false);
    }
    if(getState()){
        int ch;
        if(getWorld()->getKey(ch)){
            if(ch == KEY_PRESS_RIGHT){
                setDirection(right);
                if(getWorld()->determineBlock(this, getDirection(), 4)){
                    if(getX() <= VIEW_WIDTH - 4 - SPRITE_WIDTH)
                        moveTo(getX() + 4, getY());
                }
            }
            else if(ch == KEY_PRESS_UP){
                setDirection(up);
                if(getWorld()->determineBlock(this, getDirection(), 4)){
                    if(getY() <= VIEW_HEIGHT - 4 - SPRITE_HEIGHT){
                        moveTo(getX(), getY() + 4);
                    }
                }
            }
            else if(ch == KEY_PRESS_LEFT){
                setDirection(left);
                if(getWorld()->determineBlock(this, getDirection(), 4)){
                    if(getX() >= 4){
                        moveTo(getX() - 4, getY());
                    }
                }
            }
            else if(ch == KEY_PRESS_DOWN){
                setDirection(down);
                if(getWorld()->determineBlock(this, getDirection(), 4)){
                    if(getY() >= 4){
                        moveTo(getX(), getY() - 4);
                    }
                }
            }
            else if(ch == KEY_PRESS_SPACE && getFlames() != 0){
                decrementFlames(1);
                getWorld()->playSound(SOUND_PLAYER_FIRE);
                getWorld()->makeFlames(getX(), getY(), 3);
                //Tell student world add flames
            }
            else if(ch == KEY_PRESS_TAB && getLandmines() != 0){
                decrementLandmines(1);
                getWorld()->makeLandmines(getX(), getY());
                //Tell student world to make landmines
            }
            else if(ch == KEY_PRESS_ENTER && getVaccines() != 0){
                getWorld()->tookVaccine();
            }
        }
        return;
    }
}


Zombie::Zombie(int startX, int startY, StudentWorld* ptr, int ID)
:Person(ID, startX, startY, right, ptr){
    paralyzedCounter = 0;
    setState(true);
    setOverlapD(false);
    setBlock(true);
    movementPlanDistance = 0;
    setIfZombie(true);
    setIfFlameOverlap(true);
}

int Zombie::getXVomit(){
    return xVomit;
}

int Zombie::getYVomit(){
    return yVomit;
}

void Zombie::setPlan(int setter){
    movementPlanDistance = setter;
}

int Zombie::getPlan(){
    return movementPlanDistance;
}

void Zombie::doSomething(){
    
    if(!getState())
        return;
    if(getifIsInfectedCuurentlyByFlame()){
        setState(false);
    }
    paralyzedCounter++;
    if(paralyzedCounter % 2 == 0){
        return;
    }

    int xVomitLoc = getX();
    int yVomitLoc = getY();

    int dir = getDirection();
    if(dir == 0)
        xVomitLoc = getX() + SPRITE_WIDTH;
    else if(dir == 180)
        xVomitLoc = getX() - SPRITE_WIDTH;
    else if(dir == 90)
        yVomitLoc = getY() + SPRITE_HEIGHT;
    else if(dir == 270)
        yVomitLoc = getY() - SPRITE_HEIGHT;

    xVomit = xVomitLoc;
    yVomit = yVomitLoc;
    
//    if(getWorld()->isAPersonInTheSameDirection(this, getDirection()) && getWorld()->isPlayerCloseEnoughToZombie(this)){
    if(getWorld()->isVomitCoordinatesInPlayer(this, xVomit, yVomit)){
        int n = randInt(1, 3);
        if(n == 1){
            int x = getXVomit();
            int y = getYVomit();
            getWorld()->makeVomit(x, y);
            getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
            //Make vomit at location xVomit and yVomit
            //Play sound  SOUND_ZOMBIE_VOMIT
        }
    }
}

DumbZombie::DumbZombie(int startX, int startY, StudentWorld* ptr, int ID, int depth)
:Zombie(startX, startY, ptr, ID){
}



void DumbZombie::doSomething(){
    Zombie::doSomething();
    
    if(getPlan() == 0){
        setPlan(randInt(3, 10));
        int dir = randInt(1, 4);
        if(dir == 1)
            setDirection(0);
        else if(dir == 2)
            setDirection(90);
        else if(dir == 3)
            setDirection(180);
        else if(dir == 4)
            setDirection(270);
    }
    else if(getPlan() != 0){
        if(getDirection() == 0){
            if(getWorld()->determineBlock(this, 0, 1))
                moveTo(getX() + 1, getY());
        }
        else if(getDirection() == 90){
            if(getWorld()->determineBlock(this, 90, 1))
                moveTo(getX(), getY() + 1);
        }
        else if(getDirection() == 180){
            if(getWorld()->determineBlock(this, 180, 1))
                moveTo(getX() - 1, getY());
        }
        else if(getDirection() == 270){
            if(getWorld()->determineBlock(this, 270, 1))
                moveTo(getX(), getY() - 1);
        }
        setPlan(getPlan() - 1);
    }
}

SmartZombie::SmartZombie(int startX, int startY, StudentWorld* ptr, int ID, int depth)
:Zombie(startX, startY, ptr, ID){
}

void SmartZombie::doSomething(){
    Zombie::doSomething();
    if(getPlan() == 0){
        setPlan(randInt(3, 10));
        int dir = randInt(1, 4);
        if(dir == 1)
            setDirection(0);
        else if(dir == 2)
            setDirection(90);
        else if(dir == 3)
            setDirection(180);
        else if(dir == 4)
            setDirection(270);
    }
    else if(getPlan() != 0){
        if(getDirection() == 0){
            if(getWorld()->determineBlock(this, 0, 1))
                moveTo(getX() + 1, getY());
        }
        else if(getDirection() == 90){
            if(getWorld()->determineBlock(this, 90, 1))
                moveTo(getX(), getY() + 1);
        }
        else if(getDirection() == 180){
            if(getWorld()->determineBlock(this, 180, 1))
                moveTo(getX() - 1, getY());
        }
        else if(getDirection() == 270){
            if(getWorld()->determineBlock(this, 270, 1))
                moveTo(getX(), getY() - 1);
        }
        setPlan(getPlan() - 1);
    }
}

