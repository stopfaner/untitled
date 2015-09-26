#ifndef NPC_H
#define NPC_H
#include "Sources/GameObjects/entity.h"


class NPC : public Entity
{
public:
    NPC();
private:
    Textures::Type getTextureType(BodyPart::Type bodyPart);
};

#endif // NPC_H
