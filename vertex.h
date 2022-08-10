#ifndef _VERTEX_H_
#define _VERTEX_H_
#include <memory>
#include <string>
#include <vector>
#include "observer.h"
#include "residence.h"
#include "player.h"

class Edge;

class Vertex : public Observer {
    int vertexNumber;
    std::shared_ptr<Player> owner;
    std::string display;
    std::shared_ptr<Residence> theResidence = nullptr;
    std::vector<Edge *> adjEdges;
    std::vector<Vertex *> adjVertices;

    public:
    Vertex(int number, std::string display);

    void notify(Subject&) override;

    void addAdjEdges(Edge *e);

    void addAdjVertices(Vertex *v);

    bool hasRoadColour(std::string colour);

    std::string getDisplay();

    std::string getResidenceName();

    int getVertexNum();

    bool buildRes(std::shared_ptr<Player> player, std::string resType);

    bool startBuild(std::shared_ptr<Player> player);

    void forceBuild(std::shared_ptr<Player> player, std::string resType);

    std::shared_ptr<Player> getOwner() override;

    virtual std::string getName() override;

    // for debugging
    void printInfo();

    //for debugging
    int getVertexNumber();
};

#endif
