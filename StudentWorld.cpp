#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include "GraphObject.h"
#include <string>
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw
#include <iostream>
using namespace std;

int numm = 0;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

//// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp
//
StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{

}
//
int StudentWorld::init()
{
    int numOfCitzents = 0;
  
    numberInfected = 0;
    playerInfected = false;
    int num = getLevel();
    if(num == 1 || num == 2)
        num = 3;
    
    Level lev(assetPath());
    string levv = "level0" + to_string(num) + ".txt";
    Level::LoadResult result = lev.loadLevel(levv);
    if (result == Level::load_fail_file_not_found){
        cerr << "Cannot find level0" << num << ".txt data file" << endl;
        return GWSTATUS_PLAYER_WON;
    }
    else if (result == Level::load_fail_bad_format){
        cerr << "Your level was improperly formatted" << endl;
        return GWSTATUS_LEVEL_ERROR;
    }
    
    else if(result == Level::load_success){
        //For loop through every index of the 16 by 16 character array and initialize every proper
        
        for(int i = 0; i < 16; i++){
            for(int j = 0; j < 16; j++){
                Level::MazeEntry ge = lev.getContentsOf(i,j);
                if(ge == Level::player){
                    player = new Penelope(SPRITE_WIDTH * i, SPRITE_HEIGHT * j, this, IID_PLAYER);
                       //Add parameters of a player, i.e. make player have constr. that takes args

                }
                else if(ge == Level::wall){
                    Actor* x = new Wall(SPRITE_WIDTH * i, SPRITE_HEIGHT * j, this, IID_WALL);
                    myActors.push_back(x);
                    //Make a new wall
                }
                else if(ge == Level::exit){
                    Actor* x = new Exit(SPRITE_WIDTH * i, SPRITE_HEIGHT * j, this, IID_EXIT);
                    myActors.push_back(x);
                }
                else if(ge == Level::pit){
                    Actor* x = new Pit(SPRITE_WIDTH * i, SPRITE_HEIGHT * j, this, IID_PIT, 0);
                    myActors.push_back(x);
                }
                else if(ge == Level::gas_can_goodie){
                    Actor* x = new GasCanGoodie(SPRITE_WIDTH * i, SPRITE_HEIGHT * j, this, IID_GAS_CAN_GOODIE, 1);
                    myActors.push_back(x);
                }
                else if(ge == Level::vaccine_goodie){
                    Actor* x = new VaccineGoodie(SPRITE_WIDTH * i, SPRITE_HEIGHT * j, this, IID_VACCINE_GOODIE, 1);
                    myActors.push_back(x);
                }
                else if(ge == Level::landmine_goodie){
                    Actor* x = new LandmineGoodie(SPRITE_WIDTH * i, SPRITE_HEIGHT * j, this, IID_LANDMINE_GOODIE, 1);
                    myActors.push_back(x);
                }
                else if(ge == Level::citizen){
                    Actor* x = new Citizen(SPRITE_WIDTH * i, SPRITE_HEIGHT * j, this, IID_CITIZEN);
                    myActors.push_back(x);
                    numOfCitzents++;
                    //Make citizents
                    //Something something incremrntCitizents()
                }
                else if(ge == Level::dumb_zombie){
                    Actor* x = new DumbZombie(SPRITE_WIDTH * i, SPRITE_HEIGHT * j, this, IID_ZOMBIE, 0);
                    myActors.push_back(x);
                }
                else if(ge == Level::smart_zombie){
                    Actor* x = new SmartZombie(SPRITE_WIDTH * i, SPRITE_HEIGHT * j, this, IID_ZOMBIE, 0);
                    myActors.push_back(x);
                }

            }
        }
     }
    
    for(int i = 0; i < numOfCitzents; i++){
        player->incrementCitz();
    }
    //Use make level to initialize every single object
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    if(!(player)->getState()){
        player->setLevel(false);
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    if(playerInfected)
        numberInfected++;
    if(numberInfected == 500)
        return GWSTATUS_PLAYER_DIED;
    player->doSomething();
    list<Actor*>::iterator it;
    it = myActors.begin();
    
    
    while(it != myActors.end()){
        if(!(*it)->getState()){
            delete (*it);
            it = myActors.erase(it);
        }
        else{
            (*it)->doSomething();
            if((*it)->getFinished()){
                (*it)->setLevel(false);
                increaseScore(500);
                return GWSTATUS_FINISHED_LEVEL;
            }
            it++;
        }

    }
    ostringstream oss;
    oss.fill('0');
    oss << "Score:  " << setw(6) << getScore() << "  " << "Level: " << setw(1) << getLevel() << "  " << "Lives: " << setw(1) << getLives() << "  " << "Vaccines: " << setw(1) << player->getVaccines() << "  " << "Flames: " << setw(1) << player->getFlames() << "  " << "Mines: " << player->getLandmines() << "  " << "Infected: " << setw(1) << numberInfected << endl;
    
    string Game = oss.str();

    setGameStatText(Game);
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    if(player != nullptr){

        delete player;
        player = nullptr;
        
        list<Actor*>::iterator it;
        it = myActors.begin();
        while(it != myActors.end()){
            it = myActors.erase(it);
            delete *it;

        }
    }
    //Delete all other dynamically allocated players
}


bool StudentWorld::determineBlock(Actor* x, int dir, int steps){
    if(abs((player)->getX() - (x->getX() + SPRITE_WIDTH - 1)) < steps &&
       abs((player)->getY() - x->getY()) < SPRITE_HEIGHT)
        return false;
    else if(abs((player)->getX() - (x->getX() - (SPRITE_WIDTH - 1))) < steps &&
       abs((player)->getY() - x->getY()) < SPRITE_HEIGHT)
        return false;
    else if(abs((player)->getY() - (x->getY() - (SPRITE_HEIGHT - 1))) < steps &&
            abs((player)->getX() - x->getX()) < SPRITE_WIDTH)
        return false;
    else if(abs((player)->getY() - (x->getY() + (SPRITE_HEIGHT - 1))) < steps &&
            abs((player)->getX() - x->getX()) < SPRITE_WIDTH)
        return false;
    
    list<Actor*>::iterator it;
    it = myActors.begin();
    while(it != myActors.end()){
        if((*it)->getBlock()){
            if(dir == 0){
                if(abs((*it)->getX() - (x->getX() + SPRITE_WIDTH - 1)) < steps &&
                   abs((*it)->getY() - x->getY()) < SPRITE_HEIGHT){
                    return false;
                }
            }
            else if(dir == 90){
                if(abs((*it)->getY() - (x->getY() + SPRITE_HEIGHT - 1)) < steps &&
                   abs((*it)->getX() - x->getX()) < SPRITE_WIDTH){
                    return false;
                }
            }
            else if(dir == 180){
                if(abs(((*it)->getX() + SPRITE_WIDTH - 1) - (x->getX())) < steps &&
                   abs((*it)->getY() - x->getY()) < SPRITE_HEIGHT - 1){
                    return false;
                }
            }
            else if(dir == 270){
                if(abs(((*it)->getY() + SPRITE_HEIGHT - 1) - (x->getY())) < steps &&
                   abs((*it)->getX() - x->getX()) < SPRITE_WIDTH - 1){
                    return false;
                }
            }
        }
        it++;
            
    }
    return true;
}


bool StudentWorld::determineExit(Actor *x){
    int numX = abs(x->getX() + SPRITE_WIDTH/2 - ((player)->getX() + SPRITE_WIDTH/2));
    int numY = abs(x->getY() + SPRITE_HEIGHT/2 - ((player)->getY() + SPRITE_HEIGHT/2));
    
    if(player->getCitz() == 0){
        if(numX*numX + numY*numY < 100){
            return true;
        }
    }
    list<Actor*>::iterator it;
    it = myActors.begin();
    while(it != myActors.end()){
        int numXx = abs(x->getX() + SPRITE_WIDTH/2 - ((*it)->getX() + SPRITE_WIDTH/2));
        int numYy = abs(x->getY() + SPRITE_HEIGHT/2 - ((*it)->getY() + SPRITE_HEIGHT/2));
        if(numXx*numXx + numYy*numYy < 100){
            if((*it)->getPenepOrCitzizen()){ //If the actor overlapping with the object causes the object to die
                increaseScore(500);
                (*it)->setState(false);
                decrementCit();
                //Decrement the number of citizens in the player variable
                return false;
            }
        }
        it++;
    }
    return false;
}


bool StudentWorld::determineBlockN(int xx, int yy, int dir){
    list<Actor*>::iterator it;
    it = myActors.begin();
    while(it != myActors.end()){
        
        int numX = abs(xx + SPRITE_WIDTH/2 - ((*it)->getX() + SPRITE_WIDTH/2));
        int numY = abs(yy + SPRITE_HEIGHT/2 - ((*it)->getY() + SPRITE_HEIGHT/2));
        if((*it)->getBlock()){
            if(numX*numX + numY*numY < 100){
                return false;
            }
        }
        it++;
            
        
    }
    
    return true;
}




bool StudentWorld::determineOverlapP(Actor* x){
    list<Actor*>::iterator it;
    it = myActors.begin();
    //Then we have to look at if exit overlaps with penelope
    
    int numX = abs(x->getX() + SPRITE_WIDTH/2 - ((player)->getX() + SPRITE_WIDTH/2));
    int numY = abs(x->getY() + SPRITE_HEIGHT/2 - ((player)->getY() + SPRITE_HEIGHT/2));
    
    if(numX*numX + numY*numY < 100){
        if(x->getOverlapD())  //If the player overlapping with the object causes the player to die
            player->setState(false);
        return true;
    }
    
    while(it != myActors.end()){
        if((*it) == x){
            it++;
        }
        int numX = abs(x->getX() + SPRITE_WIDTH/2 -
                       ((*it)->getX() + SPRITE_WIDTH/2));
        int numY = abs(x->getY() + SPRITE_HEIGHT/2 - ((*it)->getY() + SPRITE_HEIGHT/2));
        
        if((*it)->ifFlameOverlapsDies()){
            if(numX*numX + numY*numY < 100){
                
                (*it)->setisInfectedbyFlame(true);
                return true;
            }
        }
        if(x->getOverlap()){
            if((*it)->getOverlap()){

            int numX = abs(x->getX() + SPRITE_WIDTH/2 - ((*it)->getX() + SPRITE_WIDTH/2));
            int numY = abs(x->getY() + SPRITE_HEIGHT/2 - ((*it)->getY() + SPRITE_HEIGHT/2));

            if(numX*numX + numY*numY < 100){
                if((*it)->getOverlapD() && x->getOverlapD())   //If the actor overlapping with the object causes the object to die
                    (*it)->setState(false);
                return true;
            }
            }
        }
        
        if((*it)->getIffVomit() && x->getIffVomit()){
            int numX = abs(x->getX() + SPRITE_WIDTH/2 - ((*it)->getX() + SPRITE_WIDTH/2));
            int numY = abs(x->getY() + SPRITE_HEIGHT/2 - ((*it)->getY() + SPRITE_HEIGHT/2));

            if(numX*numX + numY*numY < 100){
                (*it)->setVomitStatus(true);
                return true;
            }
        }
        it++;
    }
    return false;
}

void StudentWorld::makeVomit(int x, int y){
    Actor* xx = new Vomit(x, y, this, IID_VOMIT, 0, player->getDirection());
    myActors.push_back(xx);
}



void StudentWorld::makeFlames(int x, int y, int n){
    int xValue = x;
    int yValue = y;
    
    int dir = player->getDirection();
    for(int i = 1; i <= n; i++){
        if(dir == 0){
            if(determineBlockN(xValue + i * SPRITE_WIDTH , yValue, dir) && !(xValue + (i * SPRITE_WIDTH) > SPRITE_WIDTH * 15)){
                Actor *x = new Flame(xValue + i * SPRITE_WIDTH, yValue, this, IID_FLAME, 0, dir);
                myActors.push_back(x);
            }
        }
        else if(dir == 90){
            if(determineBlockN(xValue , yValue  + i * SPRITE_HEIGHT, dir) && !(yValue + (i * SPRITE_HEIGHT) > SPRITE_HEIGHT * 15)){
                Actor *x = new Flame(player->getX() , player->getY() + i * SPRITE_HEIGHT, this, IID_FLAME, 0, dir);
                myActors.push_back(x);
            }
        }
        else if(dir == 180){
            if(determineBlockN(xValue + i * SPRITE_WIDTH, yValue , dir) && (xValue - i * SPRITE_WIDTH  > SPRITE_WIDTH)){
                Actor *x = new Flame(player->getX() - i * SPRITE_WIDTH, player->getY(), this, IID_FLAME, 0, dir);
                myActors.push_back(x);
            }
        }
        else if(dir == 270){
            if(determineBlockN(xValue , yValue + i * SPRITE_HEIGHT , dir) && (yValue - i * SPRITE_HEIGHT  > SPRITE_HEIGHT)){
                Actor *x = new Flame(player->getX() , player->getY() - i * SPRITE_HEIGHT, this, IID_FLAME, 0, dir);
                myActors.push_back(x);
            }
        }
    }
}

void StudentWorld::makeLandmines(int x, int y){
    int dir = player->getDirection();
    Actor* xx = new Landmine(x, y, this, IID_LANDMINE, 1);
    myActors.push_back(xx);
}

bool StudentWorld::isAPersonInTheSameDirection(Actor *x, int dir){
    int xLoc = x->getX();
    int yLoc = x->getY();
    
    if((dir == 0 || dir == 180) && abs((yLoc) - player->getY()) < SPRITE_HEIGHT){
        return true;
    }
    else if((dir == 90 || dir == 270) && abs((xLoc) - player->getX()) < SPRITE_WIDTH){
        return true;
    }
    
    list<Actor*>::iterator it;
    it = myActors.begin();
    while(it != myActors.end()){
        if((*it)->getPenepOrCitzizen()){
            if((dir == 0 || dir == 180) && abs((yLoc) - (*it)->getY()) < SPRITE_HEIGHT){
                
                return true;
            }
            else if((dir == 90 || dir == 270) && abs((xLoc) - (*it)->getX()) < SPRITE_WIDTH){
                return true;
            }
        }
        it++;
    }
    
    return false;
}


bool StudentWorld::isPlayerCloseEnoughToZombie(Actor *x){
    int numX = abs(x->getX() + SPRITE_WIDTH/2 - ((player)->getX() + SPRITE_WIDTH/2));
    int numY = abs(x->getY() + SPRITE_HEIGHT/2 - ((player)->getY() + SPRITE_HEIGHT/2));
    
    if(numX*numX + numY*numY < 100){
        return true;
    }
    return false;
}



void StudentWorld::isCitizenCloseEnoughToZombie(Actor *x){
    list<Actor*>::iterator it;
    it = myActors.begin();
    while(it != myActors.end()){
        int numX = abs(x->getX() + SPRITE_WIDTH/2 - (((*it))->getX() + SPRITE_WIDTH/2));
        int numY = abs(x->getY() + SPRITE_HEIGHT/2 - ((*it)->getY() + SPRITE_HEIGHT/2));
        
        if((*it)->getPenepOrCitzizen()){
            if(numX*numX + numY*numY < 100){
                (*it)->infectCitizen();
            }
        }
        it++;
    }
}


int StudentWorld::getDirectionOfClosestActorToSmartZombie(Actor* x){
    
    //Return -1 if there isn't a close penelope/citizen, otherwise it'll return the closest citizen/penelope
    int direction;

    return -1;
}


void StudentWorld::infectPlayer(){
    playerInfected = true;
}


bool StudentWorld::isVomitCoordinatesInPlayer(Actor *x, int xLoc, int yLoc){
    if(abs(xLoc - player->getX()) < SPRITE_WIDTH/2 &&
       abs(yLoc - player->getY()) < SPRITE_HEIGHT/2)
        return true;
    
    list<Actor*>::iterator it;
    it = myActors.begin();
        while(it != myActors.end()){
            
            if((*it)->getPenepOrCitzizen()){
    
                if(abs(xLoc - (*it)->getX()) < SPRITE_WIDTH/2 &&
                   abs(yLoc - (*it)->getY()) < SPRITE_HEIGHT/2)
                    return true;
            }
            it++;
            
        }
    

    return false;
}


bool StudentWorld::determineOverlapWithZombiesAndPlayers(Actor* x){
    int numX = abs(x->getX() + SPRITE_WIDTH/2 - ((player)->getX() + SPRITE_WIDTH/2));
    int numY = abs(x->getY() + SPRITE_HEIGHT/2 - ((player)->getY() + SPRITE_HEIGHT/2));
    
    if(numX*numX + numY*numY < 100){
        return true;
    }
    list<Actor*>::iterator it;
    it = myActors.begin();
    while(it != myActors.end()){
        
        int numX = abs(x->getX() + SPRITE_WIDTH/2 - (((*it))->getX() + SPRITE_WIDTH/2));
        int numY = abs(x->getY() + SPRITE_HEIGHT/2 - ((*it)->getY() + SPRITE_HEIGHT/2));
        
        if((*it)->getPenepOrCitzizen()){
            if(numX*numX + numY*numY < 100){
                return true;
            }
        }
        it++;
    }
    return false;
}

bool StudentWorld::determineOverlapWithFlames(Actor* x){
    int numX = abs(x->getX() + SPRITE_WIDTH/2 - ((player)->getX() + SPRITE_WIDTH/2));
    int numY = abs(x->getY() + SPRITE_HEIGHT/2 - ((player)->getY() + SPRITE_HEIGHT/2));
    
    if(numX*numX + numY*numY < 100){
        return true;
    }
    list<Actor*>::iterator it;
    it = myActors.begin();
    while(it != myActors.end()){
        int numX = abs(x->getX() + SPRITE_WIDTH/2 - (((*it))->getX() + SPRITE_WIDTH/2));
        int numY = abs(x->getY() + SPRITE_HEIGHT/2 - ((*it)->getY() + SPRITE_HEIGHT/2));
        
        if((*it)->ifFlameOverlapsDies()){
            if(numX*numX + numY*numY < 100){
                (*it)->setisInfectedbyFlame(true);
                return true;
            }
        }
        it++;
    }
    
    return false;
}

int StudentWorld::distanceToPenelope(Actor *x){
    int xLoc = abs(x->getX() - player->getX()) * abs(x->getX() - player->getX());
    int yLoc = abs(x->getY() - player->getY()) * abs(x->getY() - player->getY());
    return sqrt(xLoc + yLoc);
}

int StudentWorld::distanceToZombie(int xx, int yy){
    list<Actor*>::iterator it;
    it = myActors.begin();
    int xLoc = -1;
    int yLoc = -1;
    int total = -1;
    int temp = -1;
    while(it != myActors.end()){
        if((*it)->getIfZombie()){
            xLoc = abs(xx - (*it)->getX()) * abs(xx - (*it)->getX());
            yLoc = abs(yy - (*it)->getY()) * abs(yy - (*it)->getY());
            total = sqrt(xLoc + yLoc);

            if(temp == -1)
                temp = total;
            
            total = min(temp, total);
        }
        it++;
    }
    return total;
}

bool StudentWorld::isPenelopeOnTheSameRowOrColAsZombie(Actor *x){
    if(abs(player->getX() - x->getX()) < SPRITE_WIDTH)
        return true;
    else if(abs(player->getY() - x->getY()) < SPRITE_HEIGHT)
        return true;
    return false;
}

int StudentWorld::directionOfPlayer(){
    return player->getDirection();
}

