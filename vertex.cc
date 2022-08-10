#include <string>
#include <vector>
#include <memory>
#include "edge.h"
#include "vertex.h"
#include "subject.h"
#include "buildingstrategy.h"
#include "basementstrategy.h"
#include "housestrategy.h"
#include "towerstrategy.h"

#include <iostream>
using std::cout;

Vertex::Vertex(int number, std::string display) : vertexNumber{number}, owner{nullptr}, display{display} {}

void Vertex::addAdjEdges(Edge *e) {
    adjEdges.emplace_back(e);
}

void Vertex::notify(Subject& whoNotified) {
    if (owner) {
        Resource temp = whoNotified.getResource();
        temp = theResidence->residenceGiveRes(temp);
        owner->playerAddResource(temp);
        std::cout << "Builder " << owner->getName() << " gained:" << std::endl;
        
        std::cout << temp.total() << " " << temp.whatResource() << std::endl;
        //std::cout << "Player acquired resources" << std::endl;
    }

}

void Vertex::addAdjVertices(Vertex *v) {
    adjVertices.emplace_back(v);
}

std::string Vertex::getDisplay() {
    return display;
}

bool Vertex::hasRoadColour(std::string colour) {
    for (auto n : adjEdges) {
        if (n->getOwner()->getName() == colour) {
            return true;
        }
    }
    return false;
}

std::string Vertex::getResidenceName() {
    if (theResidence) {
        return theResidence->getName();
    } else {
        return "empty";
    }
}

int Vertex::getVertexNum() { return vertexNumber; }

bool Vertex::buildRes(std::shared_ptr<Player> player, std::string resType) {
    // did everything except player connect to this vertice
    // call player.addpoints
    //std::cout << resType << std::endl;
    if (resType == "Basement") {
        
        if (owner == nullptr) {
            for (auto n : adjVertices) {
                if (n->owner != nullptr) {
                    return false;
                }
            }
            for (auto n : adjEdges) {
                    if (n->getOwner() != nullptr) {
                        //std::cout << "here" << std::endl;
                        if (n->getOwner()->getName() == player->getName()) {
                            owner = player;
                            std::shared_ptr<BuildingStrategy> temp = std::make_shared<BasementStrategy>();
                            theResidence = std::make_shared<Residence>(temp);
                            player->addPoint(1);
                            display = owner->getInitial() + theResidence->getName();
                            //player keeps track of this vertex
                            return true;
                        } else {
                            return false;
                        }
                }
                
            }
            owner = player;
            std::shared_ptr<BuildingStrategy> temp = std::make_shared<BasementStrategy>();
            theResidence = std::make_shared<Residence>(temp);
            player->addPoint(1);
            display = owner->getInitial() + theResidence->getName();
            return true;
        } else {
            return false;
        }

    } else if (resType == "House") {
        if (owner && theResidence) {
            if ((owner->getName() == player->getName()) && (theResidence->getName() == "B")) {
                std::shared_ptr<BuildingStrategy> temp = std::make_shared<HouseStrategy>();
                theResidence->setStrategy(temp);
                player->addPoint(1);
                display = owner->getInitial() + theResidence->getName();
                
                return true;
            }   
        }
        return false;
    } else if (resType == "Tower") {
        if (owner && theResidence) {
            if ((owner->getName() == player->getName()) && (theResidence->getName() == "H")) {
                std::shared_ptr<BuildingStrategy> temp = std::make_shared<TowerStrategy>();
                theResidence->setStrategy(temp);
                player->addPoint(1);
                display = owner->getInitial() + theResidence->getName();
                return true;
            }   
        }
        return false;            
    } else {
        return false;
    }
}

bool Vertex::startBuild(std::shared_ptr<Player> player) {
    if (owner == nullptr) {
        for (auto n : adjVertices) {
            if (n->owner != nullptr) {
                return false;
            }
        }
        owner = player;
        std::shared_ptr<BuildingStrategy> temp = std::make_shared<BasementStrategy>();
        theResidence = std::make_shared<Residence>(temp);
        player->addPoint(1);
        display = player->getInitial() + theResidence->getName();
        return true;
    }
    return false;
}

void Vertex::forceBuild(std::shared_ptr<Player> player, std::string resType) {
    if (resType == "Basement") {
        std::shared_ptr<BuildingStrategy> temp = std::make_shared<BasementStrategy>();
        owner = player;
        theResidence = std::make_shared<Residence>(temp);
        player->addPoint(1);
        display = player->getInitial() + theResidence->getName();
    } else if (resType == "House") {
        std::shared_ptr<BuildingStrategy> temp = std::make_shared<HouseStrategy>();
        owner = player;
        theResidence = std::make_shared<Residence>(temp); 
        player->addPoint(2);          
        display = player->getInitial() + theResidence->getName();
    } else if (resType == "Tower") {
        std::shared_ptr<BuildingStrategy> temp = std::make_shared<TowerStrategy>();
        owner = player;
        theResidence = std::make_shared<Residence>(temp);      
        player->addPoint(3);      
        display = player->getInitial() + theResidence->getName();
    }
}

std::shared_ptr<Player> Vertex::getOwner() {return owner;}

std::string Vertex::getName() {
    return display;
}

// for debuging
void Vertex::printInfo() {
	int el = adjEdges.size();

	for (int i = 0; i < el; ++i) {
		cout << adjEdges[i]->getEdgeNumber() << " ";
	}
	cout << "/";

	int vl = adjVertices.size();

	for (int i = 0; i < vl; ++i) {
                cout << adjVertices[i]->getVertexNumber() << " ";
        }
}

int Vertex::getVertexNumber() {
	return vertexNumber;
}
