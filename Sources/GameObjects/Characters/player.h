#ifndef PLAYER_H
#define PLAYER_H


#include "Sources/GameObjects/entity.h"

//class Ladder;


class Player : public Entity
{
public:
    Player(Textures* textures, b2World* world);

};

#endif // PLAYER_H
