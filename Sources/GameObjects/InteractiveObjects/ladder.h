#ifndef LADDER_H
#define LADDER_H
#include "interactiveobject.h"
#include "Sources/GameObjects/Characters/player.h"

class Ladder : public InteractiveObject{

public:
    Ladder();
    void use(Player *player);
};

#endif // LADDER_H
