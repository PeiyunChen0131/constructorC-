#ifndef _OBSERVER_H_
#define _OBSERVER_H_
#include <string>
#include <memory>

class Player;
class Subject;

class Observer {
  public:
    virtual void notify( Subject & whoNotified ) = 0; 

    virtual std::string getName() = 0;

    virtual std::shared_ptr<Player> getOwner() = 0;

    virtual ~Observer() = default;
};

#endif
