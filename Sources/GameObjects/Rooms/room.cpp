#include "room.h"


b2Body *Room::CreateRoom(float x,float y)
{
    Point size(18,8);
    Point pos(x,y);
    b2BodyDef bodydef;
    bodydef.position.Set(pos.x,pos.y);
    room=world->CreateBody(&bodydef);
    b2PolygonShape shape;
    b2FixtureDef fixturedef;
    b2Fixture* fixture;

    shape.SetAsBox(1,size.y-3,b2Vec2(-size.x,2.25),0.0f);
    fixturedef.shape=&shape;
    fixturedef.density=3.0;
    fixture = room->CreateFixture(&fixturedef);
    fixture->SetUserData((void*) new UserData (texture_p->getTexture(Textures::Type::WALL)));

    shape.SetAsBox(1,size.y-3,b2Vec2(size.x,2.25),0.0f);
    fixturedef.shape=&shape;
    fixturedef.density=3.0;
    fixture = room->CreateFixture(&fixturedef);
    fixture->SetUserData((void*) new UserData (texture_p->getTexture(Textures::Type::WALL)));

    shape.SetAsBox(size.x,1,b2Vec2(0,size.y),0.0f);
    fixturedef.shape=&shape;
    fixturedef.density=3.0;
    fixture = room->CreateFixture(&fixturedef);
    fixture->SetUserData((void*) new UserData (texture_p->getTexture(Textures::Type::WALL)));

    shape.SetAsBox(size.x+1,1,b2Vec2(0,-size.y),0.0f);
    fixturedef.shape=&shape;
    fixturedef.density=3.0;
    fixture = room->CreateFixture(&fixturedef);
    fixture->SetUserData((void*) new UserData (texture_p->getTexture(Textures::Type::WALL)));

    room->SetUserData((void*) new UserData (texture_p->getTexture(Textures::Type::WALL)));
    return room;
}

Room::Room(Textures *texture_p, b2World *world){
    this->texture_p=texture_p;
    this->world=world;
}

