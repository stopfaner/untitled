#ifndef VEHICLE_H
#define VEHICLE_H

#include "Sources/GameObjects/InteractiveObjects/interactiveobject.h"
#include "Box2D/Box2D.h"


class Vehicle : public InteractiveObject
{
public:
    Vehicle();
    b2RevoluteJoint* motor;
    b2Body* centerBody;
    Vehicle(DisplayData *displayData);
};

#endif // VEHICLE_H
