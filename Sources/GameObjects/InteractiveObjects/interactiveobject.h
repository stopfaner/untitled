#ifndef INTERACTIVEOBJECT_H
#define INTERACTIVEOBJECT_H
#include "Sources/GameObjects/noninteractive.h"

class Entity;

class InteractiveObject : public NonInteractive{

public:
    InteractiveObject();

    virtual void use (Entity* entity) = 0;
};

#endif // INTERACTIVEOBJECT_H
