#ifndef WEAPON_H
#define WEAPON_H

#include <Box2D/Box2D.h>
#include "Box2D/Json/b2dJson.h"


#include "Sources/GameObjects/InteractiveObjects/interactiveobject.h"
#include "Sources/UserInterface/displaydata.h"
#include "Sources/UserInterface/keylinedata.h"
#include "Sources/Process/triangulation.h"
#include "Sources/Process/jointmanager.h"

class Entity;

class Weapon : public InteractiveObject
{
public:
    b2Body *body;
    std::string errorMsg;
    float damage;
    float density;
    Triangulation* triangulation;
    Entity *entity;
    void use(Entity *entity){}
    void rotate(bool isRightDirection);
    b2WeldJoint *WJ;
    Weapon();
    void createJoint(b2Body *body, b2Body *weapon, b2Vec2 pointJoint);
};

#endif // WEAPON_H
