#ifndef BODYPART_H
#define BODYPART_H

#include "Sources/GameObjects/gameobject.h"
#include "Sources/GameObjects/entity.h"

class b2Body;

class BodyPart : public GameObject
{
public:
    enum Type {
     //   FOOT_SENSOR,
        BODY,
        HIP_LEFT,
        HIP_RIGHT,
        SHIN_LEFT,
        SHIN_RIGHT,
        ARM_RIGHT,
        ARM_LEFT,
        HEAD,
        FOOT
    };

    BodyPart(Entity* entity, Type type, b2Body* body);

    Type type;
    b2Body* body;
    Entity* entity;
};

#endif // BODY_H
