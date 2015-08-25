#ifndef BODYPART_H
#define BODYPART_H

#include "Sources/GameObjects/userdata.h"

class BodyPart : public UserData
{
public:
    enum Type {
        FOOT_SENSOR,
        BODY,
        HIP_LEFT,
        HIP_RIGHT,
        SHIN_LEFT,
        SHIN_RIGHT,
        ARM_RIGHT,
        ARM_LEFT,
        HEAD
    };

    BodyPart(DisplayData* displayData, Type type);

    Type type;
};

#endif // BODY_H
