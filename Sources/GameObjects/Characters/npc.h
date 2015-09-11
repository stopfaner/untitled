#ifndef NPC_H
#define NPC_H
#include "Sources/GameObjects/entity.h"


class NPC : public Entity
{
public:
    NPC(float x = 0, float y = 10);
private:
    Textures::Type getTextureType(BodyPart::Type bodyPart);
};

#endif // NPC_H
