#include "car.h"

Car::Car(b2Vec2 center, float scale) : Vehicle()
{
    float x = center.x;
    float y = center.y;
    Textures *textures = GeneralInfo::getInstance().textures;
    entityJoint = nullptr;

    GameObject* carPart = new GameObject;
    DisplayData* circleDD = new KeyLineData(Color(0, 255, 255), DisplayData::Layer::OBJECT);

    b2RevoluteJointDef jointDef;

    jointDef.maxMotorTorque = 10000.0f;
    jointDef.motorSpeed = 0.0f;
    jointDef.enableMotor = true;

    b2Body* mainPlank = addRect(x, y + 5*2 * scale, 10*2 * scale, 0.5*2 * scale, true);
    b2Body* rule = addRect(x, y +( 6.5*2-1.5) * scale, 0.5*2 * scale, 3 * scale, true);

    centerBody = rule;

    b2BodyDef bodydef;
    bodydef.position.Set(x + 2.5*2*2 * scale, y + 5*2 * scale);
    bodydef.type = b2_dynamicBody;
    b2Body* circle1 = world->CreateBody(&bodydef);
    circle1->SetUserData((void*) new UserData (carPart));

    b2CircleShape circleShape;
    circleShape.m_p.Set(0, 0);
    circleShape.m_radius = 5 * scale;

    b2FixtureDef fixturedef;
    fixturedef.shape = &circleShape;
    fixturedef.density = 0.8;
    fixturedef.friction = 5;

    b2Fixture* circleFix = circle1->CreateFixture(&fixturedef);
    circleFix->SetUserData((void*) new UserData (carPart, circleDD));

    bodydef.position.Set(x - 10 * scale, y + 10 * scale);
    b2Body* circle2 = world->CreateBody(&bodydef);
    circle2->SetUserData((void*) new UserData (carPart));
    b2Fixture* circleFix2 =  circle2->CreateFixture(&fixturedef);
    circleFix2->SetUserData((void*) new UserData (carPart, circleDD));
    jointDef.Initialize(mainPlank, circle1, b2Vec2(x + 2.5*2*2 * scale, y + 5*2 * scale));
    motor = (b2RevoluteJoint*) world->CreateJoint( &jointDef );
    jointDef.enableMotor = false;
    jointDef.Initialize(mainPlank, circle2, b2Vec2(x - 2.5*2*2 * scale, y + 5*2 * scale));
    world->CreateJoint( &jointDef );
    b2WeldJointDef weldJointDef;
    weldJointDef.Initialize(rule, mainPlank, b2Vec2(x, y + 5*2 * scale));
    world->CreateJoint( &weldJointDef );


    DisplayData* bodyDD = (DisplayData*) new TextureData(textures->getTextureID(Textures::Type::CRATE), DisplayData::Layer::OBJECT);
    mainPlank->SetUserData((void*) new UserData (carPart));
    mainPlank->GetFixtureList()->SetUserData((void*) new UserData (carPart, bodyDD));
    rule->SetUserData((void*) new UserData (static_cast<GameObject*>(this)));
    rule->GetFixtureList()->SetUserData((void*) new UserData (static_cast<GameObject*>(this), bodyDD));
}

b2Body* Car::addRect(float x, float y, float w, float h, bool dyn) {
    b2BodyDef bodydef;
    bodydef.position.Set(x,y);
    if(dyn)
        bodydef.type=b2_dynamicBody;
    b2Body* body=world->CreateBody(&bodydef);
    body->SetUserData(static_cast<void*>(new UserData));
    b2PolygonShape shape;
    shape.SetAsBox(w/2, h/2);

    b2FixtureDef fixturedef;
    fixturedef.shape = &shape;
    fixturedef.density = 0.5;
    fixturedef.filter.groupIndex = 1;

    body->CreateFixture(&fixturedef);


    return body;
}

void Car::use(Entity* entity){
    entity->body->SetFixedRotation(false);
    if (fabs(centerBody->GetWorldCenter().y - entity->body->GetWorldCenter().y) < 2){
        entity->vehicle = static_cast<Vehicle*>(this);
        b2RevoluteJointDef entityJointDef;
        entityJointDef.Initialize(entity->body, centerBody,
                                  b2Vec2(entity->body->GetWorldCenter().x, entity->body->GetWorldCenter().y));
        entityJoint = static_cast<b2RevoluteJoint*>( world->CreateJoint(&entityJointDef));
    }

}

void Car::left(Entity *entity){
    entity->body->SetAngularVelocity(0.5);
    entity->vehicle = nullptr;
    world->DestroyJoint(entityJoint);
    entityJoint = nullptr;
}
