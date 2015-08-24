#ifndef BODYPART_H
#define BODYPART_H

#include "Sources/GameObjects/userdata.h"

class BodyPart : public UserData
{
public:
    enum Type {
        FootSensor,
        MainFixture
    };

    BodyPart (Textures::Texture* texture_p, Type type);
    BodyPart (Type type);

    Type type;
};

#endif // BODY_H
