#include "car.h"

Car::Car(DisplayData *displayData, b2Body* centerBody, b2RevoluteJoint* motor) : Vehicle(displayData){
    this->centerBody = centerBody;
    this->motor = motor;
}

Car::Car(b2World *world, Textures* textures) : Vehicle()
{
    DisplayData* circleDD = new KeyLineData(Color(0, 255, 0));
    carData = new Car (circleDD, centerBody, motor);

    b2RevoluteJointDef jointDef;

    jointDef.maxMotorTorque = 10000.0f;
    jointDef.motorSpeed = 0.0f;
    jointDef.enableMotor = true;

    b2Body* mainPlank = addRect(0, 5*2, 10*2, 0.5*2, true, Textures::Type::CRATE, world, textures);
    b2Body* rule = addRect(0, 6.5*2-1.5, 0.5*2, 3, true, Textures::Type::CRATE, world, textures);

    carData->centerBody = mainPlank;

//mainPlank->SetUserData((void*)this);
//rule->SetUserData((void*)this);
    b2BodyDef bodydef;
    bodydef.position.Set(2.5*2*2, 5*2);
    bodydef.type = b2_dynamicBody;
    b2Body* circle1 = world->CreateBody(&bodydef);
    circle1->SetUserData((void*) carData);

    b2CircleShape circleShape;
    circleShape.m_p.Set(0, 0);
    circleShape.m_radius = 5;

    b2FixtureDef fixturedef;
    fixturedef.shape = &circleShape;
    fixturedef.density = 0.8;

    b2Fixture* circleFix = circle1->CreateFixture(&fixturedef);
    circleFix->SetUserData((void*) carData);

fixturedef.density = 0.8;

    bodydef.position.Set(-10, 10);
    b2Body* circle2 = world->CreateBody(&bodydef);
    circle2->SetUserData((void*) carData);
    b2Fixture* circleFix2 =  circle2->CreateFixture(&fixturedef);
    circleFix2->SetUserData((void*) carData);
    jointDef.Initialize(mainPlank, circle1, b2Vec2(2.5*2*2, 5*2));
    carData->motor = (b2RevoluteJoint*) world->CreateJoint( &jointDef );
    jointDef.Initialize(mainPlank, circle2, b2Vec2(-2.5*2*2, 5*2));
    world->CreateJoint( &jointDef );
    b2WeldJointDef weldJointDef;
    weldJointDef.Initialize(rule, mainPlank, b2Vec2(0, 5*2));
    world->CreateJoint( &weldJointDef );
    carData->rule = rule;
}

b2Body* Car::addRect(float x, float y, float w, float h, bool dyn, Textures::Type type, b2World *world, Textures *textures) {
    b2BodyDef bodydef;
    bodydef.position.Set(x,y);
    if(dyn)
        bodydef.type=b2_dynamicBody;
    b2Body* body=world->CreateBody(&bodydef);
    b2PolygonShape shape;
    shape.SetAsBox(w/2, h/2);

    b2FixtureDef fixturedef;
    fixturedef.shape = &shape;
    fixturedef.density = 0.5;
    fixturedef.filter.groupIndex = 1;

    b2Fixture* bodyFix = body->CreateFixture(&fixturedef);

    DisplayData* bodyDD = (DisplayData*) new TextureData(textures->getTexture(type));
 //   displayData = bodyDD;
    bodyFix->SetUserData((void*) carData);
    body->SetUserData((void*) carData);
    return body;
}

void Car::use(Player *player){
    if (player->vehicle)
        player->vehicle = nullptr;
    else
    player->vehicle = this;
}
