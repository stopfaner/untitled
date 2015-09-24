#ifndef PLAYER_H
#define PLAYER_H
#include "Sources/GameObjects/InteractiveObjects/Weapon/sword.h"

#include "Sources/GameObjects/entity.h"

//class Ladder;


class Player : public Entity
{
public:
    Player(float x = 0, float y = 10);
    void constructBody();
private:
    Textures::Type getTextureType(BodyPart::Type bodyPart);
};

#endif // PLAYER_H
