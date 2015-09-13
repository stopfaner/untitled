#ifndef BODYPART_H
#define BODYPART_H

#include "Sources/GameObjects/gameobject.h"

class Entity;
class b2RevoluteJoint;
class b2Body;
class DisplayData;

class BodyPart : public GameObject
{
public:
    enum Type {
     //   FOOT_SENSOR,
        BODY,
        HIP,
        SHIN,
        HEAD,
        FOOT,
        WRIST,
        FOREARM,
        SHOULDER
    };
    void setDefaultSpeed(float speed);
    BodyPart(Entity* entity, Type type, b2Body* body, b2RevoluteJoint* RJ = nullptr);
    b2RevoluteJoint* RJ;
    float angleDeviation;
    float desiredAngle;
    float motorSpeed;
    float defaultMotorSpeed;
    Type type;
    b2Body* body;
    Entity* entity;
    DisplayData* DD;
};

#endif // BODY_H
