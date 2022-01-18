#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// //Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
//
class StudentWorld;

class Actor: public GraphObject{
public:
    Actor(int IDPLAYAER, int startX, int startY, int dir, int depth, StudentWorld* ptr);
    //Actor constructor should take the exact parameters of Graph Object that will be passed in from derived objects
    virtual void doSomething() = 0;
    bool getState();
    void setState(bool state);
    virtual ~Actor(){
    }
    void setOverlap(bool overlap);
    bool getOverlap();
    
    bool getPenepOrCitzizen();
    void setPenepOrCitz(bool parem);
    
    bool getIffVomit();
    void setIfVomit(bool setter);
    
    bool getVomitStatus();
    void setVomitStatus(bool setter);
    
    bool getBlock(){
        return blockCond;
    }
    void setBlock(bool cond){
        blockCond = cond;
    }
    
    bool getOverlapD(){
        return setOverlapDies;
    }
    void setOverlapD(bool cond){
        setOverlapDies = cond;
    }
    
    bool getFinished(){
        return finishedLev;
    }
    void setLevel(bool x){
        finishedLev = x;
    }
    void setWorld(StudentWorld* ptr){
        ptrr = ptr;
    }
    StudentWorld* getWorld(){
        return ptrr;
    }
    
    virtual void infectCitizen(){
        
    }
    
    int getCitz(){
        return numCitz;
    }
    void decrementCitz(){
        numCitz--;
    }
    void incrementCitz(){
        numCitz++;
    }
    
    
    void incrementFlames(int num){
        numFlames += num;
    }
    void decrementFlames(int num){
        numFlames -= num;
    }
    void incrementLandmines(int num){
        numLandmines += num;
    }
    void decrementLandmines(int num){
        numLandmines -= num;
    }
    void incrementVaccines(int num){
        numVaccines += num;
    }
    void decrementVaccines(int num){
        numVaccines -= num;
    }
    
    
    int getFlames(){
        return numFlames;
    }
    int getLandmines(){
        return numLandmines;
    }
    int getVaccines(){
        return numVaccines;
    }
    

    
    void setIfZombie(bool setter){
        ifZombie = setter;
    }
    bool getIfZombie(){
        return ifZombie;
    }
    
    bool ifFlameOverlapsDies(){
        return ifOverlapsWithFlameDies;
    }
    void setIfFlameOverlap(bool setter){
        ifOverlapsWithFlameDies = setter;
    }
    
    void setisInfectedbyFlame(bool setter){
        isCurrentlyInfectedByFlame = setter;
    }
    
    bool getifIsInfectedCuurentlyByFlame(){
        return isCurrentlyInfectedByFlame;
    }
    

    
private:
    bool isCurrentlyInfectedByFlame;
    
    int numCitz;

    bool ifOverlapsWithFlameDies;
    bool ifZombie;
    
    bool ifAbleToBeVomited;
    bool vomitStatus;
    
    bool penepOrCitiz;
    int numFlames;
    int numLandmines;
    int numVaccines;
    
    bool setOverlapDies;
    bool overlapCond = true;
    bool blockCond;
    
    StudentWorld* ptrr;
    bool finishedLev = false;
    bool stateOfLife; //Keeps track if any actors are alive or not
};
////Each direct derived class(Person, Goodie, Wall, Exit, Zombie, Moving Things) should give Actor the exact parameters that it needs for GraphObject in the initializer list

//Wall class branch

class Wall: public Actor{
public:
    Wall(int startX, int startY, StudentWorld* ptr,  int ID);
    virtual void doSomething(){
        return;
    }

};

////Exit class branch

class Exit: public Actor{
public:
    Exit(int startX, int startY, StudentWorld* ptr, int ID);
    virtual void doSomething();

    
private:
    
};

//
////Immovable class branch
class immovable: public Actor{
public:
    immovable(int startX, int startY, StudentWorld* ptr, int ID, int depth);

};

class Pit: public immovable{
public:
    Pit(int startX, int startY, StudentWorld* ptr, int ID, int depth);
    virtual void doSomething();
};

class Landmine: public immovable{
public:
    Landmine(int startX, int startY, StudentWorld* ptr, int ID, int depth);
    virtual void doSomething();

private:
    int safetyTicks;
};


//Movable branch
class Movable: public Actor{
public:
    Movable(int startX, int startY, StudentWorld* ptr, int ID, int depth, int dir);

};

class Flame: public Movable{
public:
    Flame(int startX, int startY, StudentWorld* ptr, int ID, int depth, int dir);
    virtual void doSomething();

private:
    int tickNumFlame;
};

class Vomit: public Movable{
public:
    Vomit(int startX, int startY, StudentWorld* ptr, int ID, int depth, int dir);
    virtual void doSomething();


private:
    int tickAlive;
};

//Immovable class branch



//Goodie branch

class Goodie: public Actor{
public:
    Goodie(int startX, int startY, StudentWorld* ptr, int ID, int depth);
    void goodieOverlap();

private:
};

class GasCanGoodie: public Goodie{
public:
    GasCanGoodie(int startX, int startY, StudentWorld* ptr, int ID, int depth);
    virtual void doSomething();
};

class VaccineGoodie: public Goodie{
public:
    VaccineGoodie(int startX, int startY, StudentWorld* ptr, int ID, int depth);
    virtual void doSomething();
};

class LandmineGoodie: public Goodie{
public:
    LandmineGoodie(int startX, int startY, StudentWorld* ptr, int ID, int depth);
    virtual void doSomething();
};



//Person branch

class Person: public Actor{
public:
    Person(int ID, int stX, int stY, int dir, StudentWorld* ptr);
    virtual void doSomething() = 0;
private:
};


class Penelope: public Person{
public:
    Penelope(int startX, int startY, StudentWorld* ptr, int ID);
    virtual void doSomething();


private:
    
};

class Zombie: public Person{
public:
    Zombie(int startX, int startY, StudentWorld* ptr, int ID);
    virtual void doSomething();
    int getXVomit();
    int getYVomit();
    int getPlan();
    void setPlan(int setter);

private:
    int xVomit;
    int yVomit;
    int paralyzedCounter;
    int movementPlanDistance;
};

class DumbZombie: public Zombie{
public:
    DumbZombie(int startX, int startY, StudentWorld* ptr, int ID, int depth);
    virtual void doSomething();
private:
};

class SmartZombie: public Zombie{
public:
    SmartZombie(int startX, int startY, StudentWorld* ptr, int ID, int depth);
    virtual void doSomething();
private:
};

class Citizen: public Person{
public:
    Citizen(int startX, int startY, StudentWorld* ptr, int ID);
    virtual void doSomething();
    void infectCitizen(){
        infectionStatus = true;
    }
    int getInfectionCount(){
        return infectionCount;
    }
private:
    bool infectionStatus;
    int infectionCount;
    int paralyzedCounter;
};



#endif // ACTOR_H_

