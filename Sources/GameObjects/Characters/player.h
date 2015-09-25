#ifndef PLAYER_H
#define PLAYER_H


#include "Sources/GameObjects/entity.h"

//class Ladder;


class Player : public Entity
{
public:
    Player(float x = 0, float y = 10);
private:
    Textures::Type getTextureIDType(BodyPart::Type bodyPart);
};

#endif // PLAYER_H
