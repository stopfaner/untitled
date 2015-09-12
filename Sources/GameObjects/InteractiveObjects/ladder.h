#ifndef LADDER_H
#define LADDER_H
#include "interactiveobject.h"
#include "Sources/GameObjects/entity.h"
#include <Box2D/Box2D.h>
class Textures;

class Ladder : public InteractiveObject{

public:
    Ladder(b2Vec2 center, b2Vec2 size);
    void use(Entity* entity);
};

#endif // LADDER_H
