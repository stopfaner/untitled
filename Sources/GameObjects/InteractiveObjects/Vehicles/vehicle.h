#ifndef VEHICLE_H
#define VEHICLE_H

#include "Sources/GameObjects/InteractiveObjects/interactiveobject.h"
#include "Box2D/Box2D.h"

class Vehicle : public InteractiveObject
{
public:
    Vehicle(b2World* world);
    b2RevoluteJoint* motor;
    b2Body* centerBody;
    b2World* world;
    b2RevoluteJoint* entityJoint;

    virtual void left(Entity* entity) = 0;
};

#endif // VEHICLE_H
