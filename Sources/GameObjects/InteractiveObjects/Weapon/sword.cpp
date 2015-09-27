#include "sword.h"
#include "qdebug.h"
Sword::Sword(b2Body* body, b2Vec2 pointJoint)
{
    std::string errorMsg;
    b2dJson json;
    json.readFromFile("json/sword.json", errorMsg);

    b2FixtureDef fixturedefSword;
    fixturedefSword.density = 0.5;
    fixturedefSword.filter.maskBits = GeneralInfo::CollisionType::BASIC;
    fixturedefSword.filter.categoryBits = GeneralInfo::CollisionType::BODYPART;
    std::vector <Point*> polyline;
    b2Body* oldBody = json.getBodyByName("Sword");
    polyline = triangulation->chainToPolyline(oldBody->GetFixtureList());
    b2Body *sword =  Triangulation::triangulateChain(polyline, fixturedefSword, new UserData(new KeyLineData(Color(0, 255, 0),
                                         static_cast<UserData*>(body->GetFixtureList()->GetUserData())->displayData->layer)), pointJoint);


    sword->SetUserData(static_cast<void*>(new UserData(new BodyTextureData(GeneralInfo::getInstance().textures->getTextureID(Textures::Type::SWORD),
                                     static_cast<UserData*>(body->GetFixtureList()->GetUserData())->displayData->layer, polyline, sword))));
    this->body = sword;

    sword->SetTransform(sword->GetPosition(), - M_PI/2);
    createJoint(body, sword, pointJoint);
}

