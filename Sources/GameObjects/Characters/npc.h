#ifndef NPC_H
#define NPC_H
#include "Sources/GameObjects/entity.h"


class NPC : public Entity
{
public:
    NPC(Textures::Texture* texture_p);
};

#endif // NPC_H
