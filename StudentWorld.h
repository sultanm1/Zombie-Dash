#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <list>
#include "Actor.h"
#include <iostream>
#include <string>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    
    //Make sure that two consecutive calls of cleanup does not do anything or most importantly does not make the pointer access something already deleted(dangling pointer)
    virtual void cleanUp();
    
    StudentWorld* getWorld(){
        return this;
    }
    
    bool isAPersonInTheSameDirection(Actor* x, int dir);
    void infectPlayer();
    
    
    int getXPl(){
        return player->getX();
    }
    int getYPl(){
        return player->getY();
    }
    
    
    //Determining object overlap or blocking
    bool determineBlock(Actor* x, int dir, int steps);   //Can be called with the support of getworld, and pass in "this" when determining if a citizen or person
    

    bool determineBlockN(int xx, int yy, int dir);
    bool determineOverlapP(Actor* x);
    
    
    int getDirectionOfClosestActorToSmartZombie(Actor* x);
    bool isPlayerCloseEnoughToZombie(Actor *x);
    void isCitizenCloseEnoughToZombie(Actor *x);
    
    void makeFlames(int x, int y, int n);
    void makeVomit(int x, int y);
    void makeLandmines(int x, int y);
    
    void increaseCountFlames(int numCount){
        player->incrementFlames(numCount);
    }
    
    void increaseLandmineCount(int numCount){
        player->incrementLandmines(numCount);
    }
    
    bool isVomitCoordinatesInPlayer(Actor *x, int xLoc, int yLoc);
    bool determineOverlapWithZombiesAndPlayers(Actor* x);
    bool determineOverlapWithFlames(Actor *x);
    
    int distanceToPenelope(Actor *x);
    int distanceToZombie(int xx, int yy);
    
    void makeSmartZombie(int xLoc, int yLoc){
        Actor* x = new SmartZombie(xLoc, yLoc, this, IID_ZOMBIE, 0);
        myActors.push_back(x);
    }
    
    void makeDumbZombie(int xLoc, int yLoc){
        Actor* x = new DumbZombie(xLoc, yLoc, this, IID_ZOMBIE, 0);
        myActors.push_back(x);
    }
    void makeMine(int xLoc, int yLoc){
        Actor* x = new Pit(xLoc, yLoc, this, IID_PIT, 0);
        myActors.push_back(x);
    }
    
    bool isPenelopeOnTheSameRowOrColAsZombie(Actor *x);
    int directionOfPlayer();
    
    
    bool determineExit(Actor *x);
    void decrementCit(){
        player->decrementCitz();
    }
    
    void tookVaccine(){
        playerInfected = false;
        numberInfected = 0;
        player->decrementVaccines(1);
    }
    void incrementVaccines(){
        player->incrementVaccines(1);
    }
    
    
    
    //Make sure to call cleanup when calling destructor
    virtual ~StudentWorld(){
        cleanUp();
    }
    
private:
    bool playerInfected;
    int numberInfected;
    list<Actor*> myActors;
    Actor* player;
    
    //Penelope pointer
};

#endif // STUDENTWORLD_H_
