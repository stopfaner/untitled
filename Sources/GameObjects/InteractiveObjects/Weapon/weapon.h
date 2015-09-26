#ifndef WEAPON_H
#define WEAPON_H

#include <Box2D/Box2D.h>
#include "Box2D/Json/b2dJson.h"


#include "Sources/GameObjects/InteractiveObjects/interactiveobject.h"
#include "Sources/GameObjects/Characters/bodyparts.h"
#include "Sources/UserInterface/displaydata.h"
#include "Sources/UserInterface/keylinedata.h"
#include "Sources/UserInterface/bodytexturedata.h"
#include "Sources/Process/triangulation.h"
#include "Sources/Process/jointmanager.h"

class Entity;

class Weapon : public GameObject
{
public:
    enum TypeWeapon{
        TW_SPEAR,
        TW_SWORD,
    };

    TypeWeapon typeWeapon;
    Weapon();
    virtual void attack(bool isRightDirection,GeneralInfo::AttackState attackState,BodyParts* bodyParts)=0;
    b2Body *body;
    std::string errorMsg;
    void rotate(bool isRightDirection);
    float damage;
    float density;
    Entity *entity;
    void transform(b2Body *body, b2Vec2 weldPoint, b2Vec2 transformPoint, float angle);
    b2WeldJoint *WJ;
    void createJoint(b2Body *body, b2Body *weapon, b2Vec2 pointJoint);
};

#endif // WEAPON_H
