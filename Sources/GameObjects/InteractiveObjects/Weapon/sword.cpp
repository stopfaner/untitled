#include "sword.h"
#include "qdebug.h"
Sword::Sword(b2Body* body, b2Vec2 pointJoint)
{
    std::string errorMsg;

    b2dJson jsonSword;
    jsonSword.readFromFile("json/sword.json", errorMsg);

    b2FixtureDef fixturedefSword;
    fixturedefSword.density = 0.5;
    fixturedefSword.filter.maskBits = GeneralInfo::CollisionType::BASIC;
    fixturedefSword.filter.categoryBits = GeneralInfo::CollisionType::BODYPART;
    std::vector <Point*> polyline;
    b2Body* oldBody = jsonSword.getBodyByName("Sword");
    polyline = triangulation->chainToPolyline(oldBody->GetFixtureList());
    b2Body *sword =  Triangulation::triangulateChain(polyline, fixturedefSword, new UserData(new KeyLineData(Color(0, 255, 0),
                                         static_cast<UserData*>(body->GetFixtureList()->GetUserData())->displayData->layer)), pointJoint);
    b2Vec2 size = Triangulation::computePolylineSize(polyline);

    b2BodyDef centerBodyDef;
    centerBodyDef.type = b2_dynamicBody;
    b2Vec2 center = sword->GetWorldPoint(Triangulation::computeCenter(polyline, 0, 0));

    qDebug()<<size.x<<size.y;
    qDebug()<<body->GetPosition().x<<body->GetPosition().y;
    centerBodyDef.position.Set(center.x, center.y);
    b2Body* centerBody = GeneralInfo::getInstance().world->CreateBody(&centerBodyDef);
    centerBody->ResetMassData();
    centerBody->SetUserData(static_cast<void*>(new UserData));
    b2FixtureDef fixturedef;
    b2PolygonShape shape;
    shape.SetAsBox(size.x / 2.0f, size.y / 2.0f);
    fixturedef.shape = &shape;
    fixturedef.isSensor = true;
    fixturedef.density = 0.01;
    centerBody->CreateFixture(&fixturedef)->SetUserData(static_cast<void*>(new UserData));
    b2WeldJointDef WJD;
    WJD.Initialize(sword, centerBody, centerBody->GetPosition());
    GeneralInfo::getInstance().world->CreateJoint(&WJD);

    sword->SetUserData(static_cast<void*>(new UserData(new BodyTextureData(GeneralInfo::getInstance().textures->getTextureID(Textures::Type::SWORD),
                                     static_cast<UserData*>(body->GetFixtureList()->GetUserData())->displayData->layer, size.x, size.y, centerBody))));
    this->body = sword;


    sword->SetTransform(sword->GetPosition(), - M_PI/2);
    createJoint(body,sword,pointJoint);
}

