#include <memory>
#include <iostream>
#include <vector>
#include <string>
#include "dicestrategy.h"
#include "loadedstrategy.h"
#include "randomstrategy.h"
#include "player.h"
#include "resource.h"

Player::Player(std::string name) : name{name} {
    theResource = Resource{};
    std::shared_ptr<DiceStrategy> temp = std::make_shared<LoadedStrategy>();
    std::shared_ptr<Dice> tempDice = std::make_shared<Dice>(temp);
    theDice = tempDice;
}

std::string Player::playerStatus() {
    std::string status;
    status = std::to_string(playerPoints);
    status.append(" building points");
    for (int i = 0; i < 5; ++i) {
        status.append(", ");
        status.append(std::to_string(theResource.getResourceNum(i)));
        status.append(" ");
        status.append(theResource.toStr(i));
    }
    status.append(".");
    return status;
}

int Player::getResNum() {
    return theVertex.size();
}

std::string Player::getRes(int num) {
    std::string res;
    res.append(std::to_string(theVertex.at(num)->getVertexNum()));
    res.append(" ");
    res.append(theVertex.at(num)->getResidenceName());
    return res;
}

void Player::addPoint(int point) {
    this->playerPoints += point;
}

int Player::getPoint() {
    return this->playerPoints;
}

std::string Player::getInitial() {
    return name.substr(0,1);
}

//geese
void Player::playerLoseResources() {
    std::pair<int, std::string> lost = theResource.lose();
    if (lost.first > 0) {
        std::cout << "Builder " << this->name << " loses " << lost.first;
        std::cout << " resources to the geese. They loose:" << std::endl;
        std::cout << lost.first << " " << lost.second << std::endl;
    } 
}

bool Player::playerHasResource() {
    return !theResource.isEmpty();
}

std::string Player::playerSteal() {    
    return theResource.steal();
}

void Player::playerStealed(std::string res) {
    theResource.stealed(res);
}

bool Player::playerCheckTrade(std::string resource) {
    return theResource.checkTrade(resource);
}

void Player::playerTrade(std::string give,std::string take) {
    theResource.trade(give, take);
}

//build
bool Player::playerCheckResource(std::string resType, std::string BuildOrCheck) {
    return theResource.checkResource(resType, BuildOrCheck);
}

void Player::playerAddVertex(std::shared_ptr<Vertex> toAdd) {
    theVertex.emplace_back(toAdd);
}

void Player::playerAddEdge(std::shared_ptr<Edge> toAdd) {
    theEdge.emplace_back(toAdd);
}

void Player::loadResource(int type, int amount) {
    /*brick = 0;
    energy = 1;
    glass = 2;
    heat = 3;
    wifi = 4;
    park = 5; <- barely used
    */
    theResource.addThisResource(type,amount);
}

void Player::playerAddResource(Resource &toAdd) {
    theResource.addResource(toAdd);
}

std::string Player::playerSave() {
    std::string resources = theResource.resourceSave();
    std::string residences = "r ";
    if (!theEdge.empty()) {
        for (auto n : theEdge) { //dbc if this will fail
            residences = residences + std::to_string(n->getEdgeNum()) + " ";
        }
    }
    residences = residences + "h";
    if (!theVertex.empty()) {
        for (auto n : theVertex) {
            residences =  residences  + " " + std::to_string(n->getVertexNum()) + " " + n->getResidenceName();
        }
    }
    return resources + residences;
}

std::string Player::getName() {return name;}

void Player::playerSetDice(std::string diceType) {
    //std::cout << "playersetdice" << std::endl;
    if (diceType == "load") {
        std::shared_ptr<DiceStrategy> temp = std::make_shared<LoadedStrategy>();
        theDice->setStrategy(temp);
    } else if (diceType == "fair") {
        std::shared_ptr<DiceStrategy> temp = std::make_shared<RandomStrategy>();
        theDice->setStrategy(temp);
    }
}

int Player::playerRollDice() {
    return theDice->rollDice();
}
