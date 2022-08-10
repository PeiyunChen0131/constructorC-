#include <memory>
#include "residence.h"

Residence::Residence(std::shared_ptr<BuildingStrategy> strategy) : _strategy{strategy} { }
// dbc memoryleak virtual destructor
void Residence::setStrategy(std::shared_ptr<BuildingStrategy> strategy) {
    this->_strategy = strategy;
}

Resource Residence::residenceGiveRes(Resource res) {
    return this->_strategy->strategyGiveRes(res);
}

std::string Residence::getName() {
    return this->_strategy->getName();
}
