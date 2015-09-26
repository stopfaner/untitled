#ifndef SPEAR_H
#define SPEAR_H
#include "weapon.h"

class Spear : public Weapon
{
public:
    Spear(b2Body* bodyA, b2Vec2 pointJointA, b2Body* bodyB, b2Vec2 pointJointB );
    void attack(bool isRightDirection, GeneralInfo::AttackState attackState, BodyParts* bodyParts);

};

#endif // SPEAR_H
