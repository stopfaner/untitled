#ifndef LADDER_H
#define LADDER_H
#include "interactiveobject.h"
#include "Sources/GameObjects/Characters/player.h"

class Player;

class Ladder : public InteractiveObject{

public:
    Ladder(Textures::Texture* texture_p);
    void use(Player *player);
};

#endif // LADDER_H
