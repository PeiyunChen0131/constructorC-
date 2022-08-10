#include <vector>
#include <string>
#include <memory>
#include "edge.h"
#include "vertex.h"

#include <iostream>
using std::cout;

Edge::Edge(int number, std::string display) : edgeNumber{number}, owner{nullptr}, display{display} {}

std::shared_ptr<Player> Edge::getOwner() {return owner;}

int Edge::getEdgeNum() {return edgeNumber;}

void Edge::addAdjEdges(Edge *e) {
    adjEdges.emplace_back(e);
}

void Edge::addAdjVertices(Vertex *v) {
    adjVertices.emplace_back(v);
    v->addAdjEdges(this);
}

void Edge::forceBuildRoad(std::shared_ptr<Player> player) {
    owner = player;
    display = player->getInitial() + "R";
}

bool Edge::buildRoad(std::shared_ptr<Player> player) {
    if (owner == nullptr) {
        for (auto n : adjVertices) {
            //std::cout << "here" << std::endl;
            if (n->getOwner() != nullptr) {
                //std::cout << n->getOwner()->getName() << std::endl;
                if (n->getOwner()->getName() == player->getName()) {
                    
                    owner = player;
                    display = player->getInitial() + "R";
                    return true;
                }
            }
            /*if ((n->getOwner() != nullptr) && (n->getOwner()->getName() == player->getName())) {
                std::cout << n->getOwner()->getName() << std::endl;
                owner = player;
                display = player->getInitial() + "R";
                return true;
            }*/
        } 
        for (auto n : adjEdges) {
            if ((n->getOwner() != nullptr) && (n->getOwner()->getName() == player->getName())) {
                for (auto v : adjVertices) {
                    //std::cout << "here" << std::endl;
                    if (v->getOwner() != nullptr) {
                        //std::cout << n->getOwner()->getName() << std::endl;
                        if (v->getOwner()->getName() != player->getName()) {
                            
                            owner = player;
                            display = player->getInitial() + "R";
                            return true;
                        }
                    }
                    /*if ((v->getOwner() != nullptr) && (v->getOwner()->getName() != player->getName())) {
                        bool buildAcross = v->hasRoadColour(player->getName());
                        if (buildAcross) {
                            return false;
                        }
                    }*/
                }
                owner = player;
                display = player->getInitial() + "R";
                return true;
            }
        }
        return false;             
    }
    return false;
    
}

std::string Edge::getDisplay() {
    return display;
}

void Edge::printInfo() {
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

int Edge::getEdgeNumber() {
        return edgeNumber;
}
