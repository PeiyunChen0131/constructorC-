#include <string>
#include <vector>
#include "resource.h"
#include "player.h"

Resource::Resource() {
    for(int i = 0; i < 6; i++) {
        vecOfResource.emplace_back(0);
    }
}

void Resource::addThisResource(int type, int amount) {
    vecOfResource.at(type) += amount;
}

void Resource::addResource(Resource &toAdd) {
    for(int i = 0; i < 6; i++) {
        this->vecOfResource.at(i) += toAdd.vecOfResource.at(i);
    }
}

int Resource::getResourceNum(int type) {
    return vecOfResource.at(type);
}

std::string Resource::whatResource() {
    for(int i = 0; i < 5; i++) {
        if (vecOfResource.at(i) > 0) {
            std::string toRet = "";
            toRet = this->toStr(i);
            return toRet;
        }
    }
    return "nothing";
}

std::string Resource::toStr(int type) {
    if (type == 0) {
        return "BRICK";
    } else if (type == 1) {
        return "ENERGY";
    } else if (type == 2) {
        return "GLASS";
    } else if (type == 3) {
        return "HEAT";
    } else if (type == 4) {
        return "WIFI";
    } else if (type == 5) {
        return "PARK";
    } else {
        return "nothing";
    }
}

int Resource::toInt(std::string type) {
    if (type == "BRICK") {
        return 0;
    } else if (type == "ENERGY") {
        return 1;
    } else if (type == "GLASS") {
        return 2;
    } else if (type == "HEAT") {
        return 3;
    } else if (type == "WIFI") {
        return 4;
    } else if (type == "PARK") {
        return 5;
    } else {
        return -1;
    }
}

int Resource::total() {
    int total = 0;
    for (int i = 0; i < 5; i++) {
        total += vecOfResource.at(i);
    }
    return total;
}

bool Resource::isEmpty() {
    int total = this->total();
    if (total == 0) {
        return true;
    } else {
        return false;
    }
}

std::vector<std::string> Resource::avaliableResources() {
    std::vector<std::string> toReturn;
    for (unsigned i = 0; i < vecOfResource.size(); i++) {
        if (vecOfResource[i] > 0) {
            toReturn.emplace_back(toStr(i));
        }
    }
    return toReturn;
}

std::pair<int, std::string> Resource::lose() {
    std::vector<std::string> avaliableResources = this->avaliableResources();
    std::pair<int, std::string> toRet;
    if (avaliableResources.empty() || this->total() < 10) {
        toRet.first = 0;
        toRet.second = "nothing";
        return toRet;
    } else {
        int n = avaliableResources.size();
        int toSteal = rand() % n;
        std::string toStealStr = avaliableResources.at(toSteal);
        int toStealInt = this->toInt(toStealStr);
        int prevAmount = vecOfResource.at(toStealInt);
        vecOfResource.at(toStealInt) = vecOfResource.at(toStealInt) / 2;
        toRet.first = prevAmount - vecOfResource.at(toStealInt);
        toRet.second = toStealStr;
        return toRet;
    }
}

std::string Resource::steal() {
    int total = this->total();
    if (total == 0) {
        return "nothing";
    } else {
        int random = rand() % total + 1;
        int count = random;
        int toSteal = 0;
        for (int i = 0; i < 5; i++) {
            count = count - vecOfResource.at(i);
            if (count <= 0) {
                toSteal = i;
                // stealing....
                vecOfResource.at(i) -= 1;
                i = 6;
            }
        }
        std::string toRet = toStr(toSteal);
        return toRet;
    }

}

void Resource::stealed(std::string res) {
    if (res != "nothing") {
        int resource = this->toInt(res);
        vecOfResource.at(resource) += 1;
    }
}

bool Resource::checkTrade(std::string resource) {
    int intResource = this->toInt(resource);
    if (intResource != -1) {
        if (vecOfResource.at(intResource) > 0) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

void Resource::trade(std::string give, std::string take) {
    int intGive = this->toInt(give);
    vecOfResource.at(intGive) -= 1;
    int intTake = this->toInt(take);
    vecOfResource.at(intTake) += 1;
}

bool Resource::checkResource(std::string type, std::string BuildOrCheck) {
    if (type == "Basement") {
        if (vecOfResource.at(0)>1 && vecOfResource.at(1)>1 && vecOfResource.at(2)>1) {
            if (BuildOrCheck == "Build") {
                vecOfResource.at(0)-=1;
                vecOfResource.at(1)-=1;
                vecOfResource.at(2)-=1;
            }
            return true;
        } else {
            return false;
        }
    } else if (type == "House") {
        if (vecOfResource.at(2)>2 && vecOfResource.at(3)>3) {
            if (BuildOrCheck == "Build") {
                vecOfResource.at(2)-=2;
                vecOfResource.at(3)-=3;
            }
            return true;
        } else {
            return false;
        }
    } else if (type == "Tower") {
        if (vecOfResource.at(0)>3 && vecOfResource.at(1)>2 && vecOfResource.at(2)>2 &&
                    vecOfResource.at(3)>2 && vecOfResource.at(4)>1) {
            if (BuildOrCheck == "Build") {
                vecOfResource.at(0)-=3;
                vecOfResource.at(1)-=2;
                vecOfResource.at(2)-=2;
                vecOfResource.at(3)-=2;
                vecOfResource.at(4)-=1;
            }
            return true;
        } else {
            return false;
        }
    } else if (type == "Road") {
        if (vecOfResource.at(3)>1 && vecOfResource.at(4)>1) {
            if (BuildOrCheck == "Build") {
                vecOfResource.at(3)-=1;
                vecOfResource.at(4)-=1;
            }
            return true;
        } else {
            return false;
        }            
    } else {
        return false;
    }
}

std::string Resource::resourceSave() {
    std::string toRet;
    for(int i = 0; i < 5; i++) {
        toRet = toRet + std::to_string(vecOfResource.at(i)) + " ";
    }
    return toRet;
}

std::string Resource::resourceOnTile() {
    std::string toRet = "";
    bool count = true;
    for (int i = 0; i < 6; i++) {
        if (vecOfResource.at(i) > 0 && count) {
            toRet = std::to_string(i);
            count = false;
        } else if (vecOfResource.at(i) > 0) {
            toRet = toRet + " " + std::to_string(i);
        }
    }
    return toRet;
}

Resource Resource::operator+(Resource toAdd) {
    for(int i = 0; i < 6; i++) {
        this->vecOfResource.at(i) += toAdd.vecOfResource.at(i);
    } // dbc not sure
    return *this;
}
