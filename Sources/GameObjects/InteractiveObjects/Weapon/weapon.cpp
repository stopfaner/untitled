#include "weapon.h"

void Weapon::rotate( bool isRightDirection)
{
    float weaponAngle;
    if(isRightDirection) weaponAngle = -M_PI/2;
    else weaponAngle = M_PI/2;
    b2Body *bodyA = WJ->GetBodyA();
    b2Body *bodyB = WJ->GetBodyB();
    b2Vec2  point = WJ->GetAnchorA();
    GeneralInfo::getInstance().world->DestroyJoint(WJ);
    body->SetTransform(body->GetPosition(), weaponAngle );
    createJoint(bodyA,bodyB,point);
}

void Weapon::createJoint(b2Body* body, b2Body* weapon, b2Vec2 pointJoint )
{
    b2WeldJointDef WJD;
    WJD.Initialize(body,weapon,pointJoint);
    WJ = static_cast<b2WeldJoint*>(GeneralInfo::getInstance().world->CreateJoint(&WJD));
}

Weapon::Weapon(){

}

