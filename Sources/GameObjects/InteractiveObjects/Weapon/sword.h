#ifndef SWORD_H
#define SWORD_H
#include "weapon.h"


class Sword : public Weapon
{
public:
    Sword(b2Body* body, b2Vec2 pointJoint);
    void attack(bool isRightDirection, GeneralInfo::AttackState attackState,BodyParts* bodyParts);
};

#endif // SWORD_H
