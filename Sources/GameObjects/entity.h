#ifndef ENTITY_H
#define ENTITY_H
#include "Sources/GameObjects/userdata.h"


class Entity : public UserData
{
public:
    Entity(Textures::Texture* texture_p);
};

#endif // ENTITY_H
