#include "ladder.h"

Ladder::Ladder(b2Vec2 center, b2Vec2 size){
    b2World *world = GeneralInfo::getInstance().world;
    Textures *textures = GeneralInfo::getInstance().textures;
    b2BodyDef bodydefLadder;
    bodydefLadder.position.Set(center.x, center.y);
    bodydefLadder.type=b2_staticBody;
    b2Body* body=world->CreateBody(&bodydefLadder);

    b2PolygonShape shape;
    shape.SetAsBox(size.x, size.y);

    b2FixtureDef fixturedefLadder;
    fixturedefLadder.shape = &shape;
    fixturedefLadder.isSensor = true;
    fixturedefLadder.density = 3.0;
    fixturedefLadder.filter.groupIndex = 1;

    b2Fixture* ladderFixture = body->CreateFixture(&fixturedefLadder);

    body->SetUserData((void*) new UserData);
    DisplayData* ladderDD = (DisplayData*) new TextureData(textures->getTexture(Textures::Type::LADDER),
                                                           DisplayData::Layer::BACKGROUND_OBJECT);
    ladderFixture->SetUserData((void*) new UserData(this, ladderDD));

}

void Ladder::use (Entity* entity){
    entity->isOnLadder = true;
}
