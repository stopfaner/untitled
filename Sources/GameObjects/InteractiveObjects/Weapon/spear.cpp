#include "spear.h"

Spear::Spear(b2Body* bodyA, b2Vec2 pointJointA, b2Body *bodyB, b2Vec2 pointJointB)
{
    std::string errorMsg;
    b2dJson jsonSpear;
    jsonSpear.readFromFile("json/spear.json", errorMsg);

    b2FixtureDef fixturedefSpear;
    fixturedefSpear.density = 1.0;
    fixturedefSpear.filter.maskBits = GeneralInfo::CollisionType::BASIC;
    fixturedefSpear.filter.categoryBits = GeneralInfo::CollisionType::BODYPART;
    std::vector <Point*> polyline;
    b2Body* oldBody = jsonSpear.getBodyByName("Spear");
    polyline = Triangulation::chainToPolyline(oldBody->GetFixtureList(),b2Vec2(1,3));
    b2Body *spear = Triangulation::triangulateChain(polyline, fixturedefSpear, new UserData(new KeyLineData(Color(0, 255, 0),
                                        static_cast<UserData*>(bodyB->GetFixtureList()->GetUserData())->displayData->layer)), pointJointB);

    spear->SetUserData(static_cast<void*>(new UserData(new BodyTextureData(GeneralInfo::getInstance().textures->getTextureID(Textures::Type::SPEAR),
                                     static_cast<UserData*>(bodyB->GetFixtureList()->GetUserData())->displayData->layer, polyline, spear))));
    this->body = spear;

    spear->SetTransform(spear->GetPosition()+b2Vec2(3,0), -M_PI/2);
    bodyA->SetTransform(bodyA->GetPosition()+b2Vec2(1,0),bodyA->GetAngle());
    bodyB->SetTransform(bodyB->GetPosition()-b2Vec2(1,0),bodyB->GetAngle());
    createJoint(bodyA,spear,pointJointA);
    createJoint(bodyB,spear,pointJointB);
}


void Spear::attack(bool isRightDirection, GeneralInfo::AttackState attackState, BodyParts *bodyParts)
{
    float direction;
    if (isRightDirection) direction = 1;
    else direction = -1;
    switch (attackState) {
    case GeneralInfo::AS_SWING:
        bodyParts->shoulder2->RJI.desiredAngle = GeneralInfo::D2R (135.0f * direction);
        bodyParts->shoulder2->RJI.motorSpeed = 1;

        bodyParts->shoulder->RJI.desiredAngle = GeneralInfo::D2R (235.0f * direction);
        bodyParts->shoulder->RJI.motorSpeed = 1;

        bodyParts->forearm2->RJI.desiredAngle = GeneralInfo::D2R (45.0f * direction);
        bodyParts->forearm2->RJI.motorSpeed = 1;

        bodyParts->forearm->RJI.desiredAngle = GeneralInfo::D2R (45.0f * direction);
        bodyParts->forearm->RJI.motorSpeed = 1;


        break;
    case GeneralInfo::AS_HIT:

        bodyParts->shoulder2->RJI.desiredAngle = GeneralInfo::D2R (0.0f * direction);
        bodyParts->shoulder2->RJI.motorSpeed = 4;

        bodyParts->shoulder->RJI.desiredAngle = GeneralInfo::D2R (0.0f * direction);
        bodyParts->shoulder->RJI.motorSpeed = 4;

        bodyParts->forearm2->RJI.desiredAngle = GeneralInfo::D2R (0.0f * direction);
        bodyParts->forearm2->RJI.motorSpeed = 4;

        bodyParts->forearm->RJI.desiredAngle = GeneralInfo::D2R (0.0f * direction);
        bodyParts->forearm->RJI.motorSpeed = 4;




        break;
    }
}
