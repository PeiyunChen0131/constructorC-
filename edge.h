#ifndef _EDGE_H_
#define _EDGE_H_
#include <vector>
#include <string>
#include <memory>

class Vertex;

class Player;

class Edge {
    int edgeNumber;
    std::shared_ptr<Player> owner;
    std::string display;
    std::vector<Edge *> adjEdges;
    std::vector<Vertex *> adjVertices;

    public:
    Edge(int number, std::string display);

    void addAdjEdges(Edge *e);

    void addAdjVertices(Vertex *v);

    std::shared_ptr<Player> getOwner();

    int getEdgeNum();

    void forceBuildRoad(std::shared_ptr<Player> player);

    bool buildRoad(std::shared_ptr<Player> player);

    std::string getDisplay();
   
    // for debugging
    void printInfo();
    
    //for  debugging
    int getEdgeNumber();
};

#endif
