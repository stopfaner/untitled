#include "weapon.h"

void Weapon::transform(b2Body *body, b2Vec2 weldPoint, b2Vec2 transformPoint, float angle)
{
    b2Body *bodyA = this->body;
    b2Body *bodyB = body;
    if (WJ)
        GeneralInfo::getInstance().world->DestroyJoint(WJ);
    this->body->SetTransform(transformPoint, angle);
    createJoint(bodyA, bodyB, weldPoint);
}

void Weapon::createJoint(b2Body* body, b2Body* weapon, b2Vec2 pointJoint )
{
    b2WeldJointDef WJD;
    WJD.Initialize(body,weapon,pointJoint);
    WJ = static_cast<b2WeldJoint*>(GeneralInfo::getInstance().world->CreateJoint(&WJD));
}

Weapon::Weapon(){

}

