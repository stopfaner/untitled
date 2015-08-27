#ifndef CAR_H
#define CAR_H

#include "vehicle.h"
#include "Sources/GameObjects/Characters/player.h"
#include "Sources/UserInterface/keylinedata.h"
#include "Sources/UserInterface/texturedata.h"
#include "Sources/UserInterface/textures.h"
#include "Sources/userdata.h"


class Car : public Vehicle
{
public:
    Car(b2World* world, Textures *textures);
    b2Body *addRect(float x, float y, float w, float h, bool dyn, Textures::Type type,
                    b2World* world, Textures* textures);
    void use(Player* player);
    b2Body * rule;
};

#endif // CAR_H
