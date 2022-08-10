#include "subject.h"
#include "observer.h"
#include <vector>
#include <string>
#include <memory>
#include <algorithm>

#include <iostream>
using std::cout;

void Subject::attach(std::shared_ptr<Observer> o) {
    observers.emplace_back(o);
}

void Subject::notifyObservers() {
 
    for (auto &ob : observers) {
        ob->notify(*this);
    }
}

std::string Subject::getObserverNames() {
    std::string nameList{};
    std::vector<std::string> names{};
    for (auto &ob : observers) names.push_back( ob->getName() );
    std::sort( names.begin(),names.end() );
    for (auto n: names) nameList += n + " ";
    return nameList;
}

/*
// for debugging
void Subject::printOb() {
	int l = observers.size();

	for (int i = 0; i < l; ++i) {
		cout << observers[i]->getObserverNames() << " ";
	}
}
*/
