#include "sword.h"
#include "qdebug.h"
Sword::Sword(b2Body* body, b2Vec2 pointJoint)
{
    std::string errorMsg;
    b2dJson jsonSword;
    jsonSword.readFromFile("json/sword.json", errorMsg);
    b2FixtureDef fixturedefSword;
    fixturedefSword.density = 1;
    fixturedefSword.filter.maskBits = GeneralInfo::CollisionType::BASIC;
    fixturedefSword.filter.categoryBits = GeneralInfo::CollisionType::BODYPART;
    std::vector <Point*> polyline;
    b2Body* oldBody = jsonSword.getBodyByName("Sword");
    polyline = Triangulation::chainToPolyline(oldBody->GetFixtureList());
    b2Body *sword =  Triangulation::triangulateChain(polyline, fixturedefSword, new UserData(new KeyLineData(Color(0, 255, 0),
                                         static_cast<UserData*>(body->GetFixtureList()->GetUserData())->displayData->layer)), pointJoint);


    sword->SetUserData(static_cast<void*>(new UserData(new BodyTextureData(GeneralInfo::getInstance().textures->getTextureID(Textures::Type::SWORD),
                                     static_cast<UserData*>(body->GetFixtureList()->GetUserData())->displayData->layer, polyline, sword))));
    this->body = sword;

    sword->SetTransform(sword->GetPosition(), - M_PI/2);
    createJoint(body, sword, pointJoint);
}

void Sword::attack(bool isRightDirection, GeneralInfo::AttackState attackState, BodyParts *bodyParts)
{
    float direction;
    if (isRightDirection) direction = 1;
    else direction = -1;
    switch (attackState) {
    case GeneralInfo::AS_SWING:
        bodyParts->shoulder2->RJI.desiredAngle = GeneralInfo::D2R (135.0f * direction);
        bodyParts->shoulder2->RJI.motorSpeed = 1;

        bodyParts->forearm2->RJI.desiredAngle = GeneralInfo::D2R (135.0f * direction);
        bodyParts->forearm2->RJI.motorSpeed = 1;

        bodyParts->wrist2->RJI.desiredAngle = GeneralInfo::D2R (0.0f * direction);
        bodyParts->wrist2->RJI.motorSpeed = 0.2;

        break;
    case GeneralInfo::AS_HIT:
        bodyParts->shoulder2->RJI.desiredAngle = GeneralInfo::D2R (0.0f * direction);
        bodyParts->shoulder2->RJI.motorSpeed = 4;

        bodyParts->forearm2->RJI.desiredAngle = GeneralInfo::D2R (0.0f * direction);
        bodyParts->forearm2->RJI.motorSpeed = 4;

        bodyParts->wrist2->RJI.desiredAngle = GeneralInfo::D2R (0.0f * direction);
        bodyParts->wrist2->RJI.motorSpeed = 0.2;
        break;
    }
}

