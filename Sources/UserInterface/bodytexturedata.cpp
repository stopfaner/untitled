#include "bodytexturedata.h"


BodyTextureData::BodyTextureData(Textures::Texture *texture_p, Layer layer, std::vector <Point*> &polyline, b2Body *body) : TextureData (texture_p, layer){

    b2Vec2 size = Triangulation::computePolylineSize(polyline);

    b2BodyDef centerBodyDef;
    centerBodyDef.type = b2_dynamicBody;
    b2Vec2 center = body->GetWorldPoint(Triangulation::computeCenter(polyline, 0, 0));
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
    WJD.Initialize(body, centerBody, centerBody->GetPosition());
    GeneralInfo::getInstance().world->CreateJoint(&WJD);


    this->width = size.x;
    this->height = size.y;
    this->centerBody = centerBody;
}

