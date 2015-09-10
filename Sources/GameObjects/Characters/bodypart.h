#ifndef BODYPART_H
#define BODYPART_H

#include "Sources/GameObjects/gameobject.h"
#include "Sources/GameObjects/entity.h"

class b2RevoluteJoint;
class b2Body;

class BodyPart : public GameObject
{
public:
    enum Type {
     //   FOOT_SENSOR,
        BODY,
        HIP,
        SHIN,
        ARM,
        HEAD,
        FOOT,
        WRIST,
        FOREARM,
        SHOULDER
    };

    BodyPart(Entity* entity, Type type, b2Body* body, b2RevoluteJoint* RJ = nullptr);
    b2RevoluteJoint* RJ;
    float desiredAngle;
    Type type;
    b2Body* body;
    Entity* entity;
};

#endif // BODY_H
