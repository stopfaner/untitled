#include "sword.h"

Sword::Sword(b2Body* body, b2Vec2 pointJoint)
{
    b2World* world=GeneralInfo::getInstance().world;
    b2dJson jsonSword;
    jsonSword.readFromFile("json/sword.json", errorMsg);

    b2FixtureDef fixturedefSword;
    fixturedefSword.density = 1.0;
    fixturedefSword.filter.maskBits = GeneralInfo::CollisionType::BASIC;
    fixturedefSword.filter.categoryBits = GeneralInfo::CollisionType::BODYPART;

    b2Body *sword =  triangulation->triangulateChain(triangulation->chainToPolyline(jsonSword.getBodyByName("Sword")->GetFixtureList()),
                                     fixturedefSword, new UserData(new KeyLineData(Color(0, 255, 0),
                                                                                   DisplayData::Layer::PLAYER_NEAR)), pointJoint).at(0);
    this->body = sword;

    sword->SetTransform(sword->GetPosition(), -M_PI/2);
    createJoint(body,sword,pointJoint);


}

