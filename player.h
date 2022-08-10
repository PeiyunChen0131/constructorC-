#ifndef _PLAYER_H_
#define _PLAYER_H_
#include <memory>
#include <vector>
#include <string>
#include "resource.h"
#include "dice.h"
#include "edge.h"
#include "vertex.h"

class Player{
    std::shared_ptr<Dice> theDice;
    std::string name;
    Resource theResource; 
    int playerPoints = 0;
    std::vector<std::shared_ptr<Vertex>> theVertex;
    std::vector<std::shared_ptr<Edge>> theEdge;

    public:
    Player(std::string name);

    std::string playerStatus();

    int getResNum();

    std::string getRes(int num);

    //build-res
    void addPoint(int point);

    int getPoint();

    std::string getInitial();

    void playerLoseResources();

    bool playerHasResource();

    std::string playerSteal();

    void playerStealed(std::string res);

    bool playerCheckTrade(std::string resource);

    void playerTrade(std::string give,std::string take);

    //build
    bool playerCheckResource(std::string resType, std::string BuildOrCheck);

    void playerAddVertex(std::shared_ptr<Vertex> toAdd);

    void playerAddEdge(std::shared_ptr<Edge> toAdd);

    void loadResource(int type, int amount);

    void playerAddResource(Resource &toAdd);

    std::string playerSave();

    std::string getName();

    void playerSetDice(std::string diceType);

    int playerRollDice();
};

#endif
