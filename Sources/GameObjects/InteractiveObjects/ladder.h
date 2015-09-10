#ifndef LADDER_H
#define LADDER_H
#include "interactiveobject.h"
#include "Sources/GameObjects/Characters/player.h"
#include <Box2D/Box2D.h>
class Textures;

class Ladder : public InteractiveObject{

public:
    Ladder(Textures* textures, b2World* world,  b2Vec2 center, b2Vec2 size);
    void use(Player *player);
};

#endif // LADDER_H
