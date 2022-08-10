#ifndef SUBJECT_H
#define SUBJECT_H
#include <vector>
#include <string>
#include <memory>
#include "resource.h"

class Observer;

class Subject {
    protected:
    std::vector<std::shared_ptr<Observer>> observers;

    public:
    void attach(std::shared_ptr<Observer> o);

    void notifyObservers();

    std::string getObserverNames();

    virtual Resource getResource() = 0;

    // for debugging
    //void printOb();
};

#endif
