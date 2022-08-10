#ifndef _RESOURCE_H_
#define _RESOURCE_H_
#include <vector>
#include <string>

class Resource {
    std::vector<int> vecOfResource;

    public:
    Resource();

    void addThisResource (int type, int amount);

    void addResource(Resource &toAdd);

    int getResourceNum(int type);

    std::string whatResource();

    std::string toStr(int type);

    int toInt(std::string type);

    int total();

    bool isEmpty();

    std::vector<std::string> avaliableResources();

    std::pair<int, std::string> lose();

    std::string steal();

    void stealed(std::string res);

    bool checkTrade(std::string resource);

    void trade(std::string give, std::string take);

    //build
    bool checkResource(std::string type, std::string BuildOrCheck);

    std::string resourceSave();

    std::string resourceOnTile();

    Resource operator+(Resource toAdd);
};

#endif
