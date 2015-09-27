#ifndef PLAYER_H
#define PLAYER_H
#include "Sources/GameObjects/InteractiveObjects/Weapon/sword.h"
#include "Sources/GameObjects/InteractiveObjects/Weapon/spear.h"
#include "Sources/GameObjects/entity.h"

//class Ladder;


class Player : public Entity
{
public:
    Player();
    void constructBody(bool isMirrored, float x, float y);
private:
    Textures::Type getTextureType(BodyPart::Type bodyPart);
};

#endif // PLAYER_H
