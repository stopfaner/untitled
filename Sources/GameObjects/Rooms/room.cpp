#include "room.h"
/*
#include <QDebug>
b2Body *Room::CreateRoom(b2Vec2 center)
{/*
    b2BodyDef bodydef;
    bodydef.position.Set(center.x, center.y);
    room=world->CreateBody(&bodydef);
    b2PolygonShape shape;
    b2FixtureDef fixturedef;
    b2Fixture* fixture;

float hL = size.y - 2*wallWidth - passageHeightLeft;
    shape.SetAsBox(wallWidth/2, hL/2,
                   b2Vec2 (-size.x/2 + wallWidth/2, size.y/2 - wallWidth -hL/2), 0.0f);
    fixturedef.shape=&shape;
    fixturedef.density = 3.0;
    fixture = room->CreateFixture(&fixturedef);
    fixture->SetUserData((void*) new GameObject (texture_p->getTexture(Textures::Type::WALL)));

float hR = size.y - 2*wallWidth - passageHeightRight;
    shape.SetAsBox(wallWidth/2, hR/2,
                   b2Vec2 (size.x/2 - wallWidth/2, size.y/2 - wallWidth -hR/2), 0.0f);
    fixturedef.shape=&shape;
    fixturedef.density = 3.0;
    fixture = room->CreateFixture(&fixturedef);
    fixture->SetUserData((void*) new GameObject (texture_p->getTexture(Textures::Type::WALL)));

    if(isUpWall){
        shape.SetAsBox(size.x/2, wallWidth/2, b2Vec2(0, size.y/2 - wallWidth/2), 0.0f);
        fixturedef.shape=&shape;
        fixturedef.density = 3.0;
        fixture = room->CreateFixture(&fixturedef);
        fixture->SetUserData((void*) new GameObject (texture_p->getTexture(Textures::Type::WALL)));
    }

    if(isDownWall){
        shape.SetAsBox(size.x/2, wallWidth/2, b2Vec2(0, - size.y/2 + wallWidth/2), 0.0f);
        fixturedef.shape=&shape;
        fixturedef.density = 3.0;
        fixture = room->CreateFixture(&fixturedef);
        fixture->SetUserData((void*) new GameObject (texture_p->getTexture(Textures::Type::WALL)));
    }


    room->SetUserData((void*) new GameObject (texture_p->getTexture(Textures::Type::WALL)));
    return room;
}

Room::Room(Textures *texture_p, b2World *world){
    this->texture_p=texture_p;
    this->world=world;
}
*/
