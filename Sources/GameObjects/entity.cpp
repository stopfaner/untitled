#include "entity.h"

#include <qdebug.h>

#define M_PI		3.14159265358979323846

float D2R (float degrees){
    return degrees * M_PI / 180.0f;
}
float R2D (float radians){
    return radians * 180.0f / M_PI;
}

Entity::Entity(float x, float y) : GameObject ()
{
    this->x = x;
    this->y = y;
    surfaceAngle = 0;

    EntityList::getInstance().list.push_back(this);

    moveState = MS_STAND;
    moveStateVertical = MSV_STAND;
    isOnLadder = false;
    isJumping = false;
    isSitting = false;
    isStanding = false;
    isUsingLeftLeg = true;
    isAscendingLeg = true;
    isRightDirection = true;
    jumpCooldown = 0;
    jumpCooldownMax = 40;
    useCooldown = 0;
    useCooldownMax = 10;
    vehicle = nullptr;
    weapon =nullptr;
}

void Entity::attack()
{
    float direction;
    if (isRightDirection) direction = 1;
    else direction = -1;
    switch (attackState) {
    case AS_SWING:
        bodyParts.shoulder2->RJI.desiredAngle = D2R (135.0f * direction);
        bodyParts.shoulder2->RJI.motorSpeed = 1;

        bodyParts.forearm2->RJI.desiredAngle = D2R (135.0f * direction);
        bodyParts.forearm2->RJI.motorSpeed = 1;

        break;
    case AS_HIT:
        bodyParts.shoulder2->RJI.desiredAngle = D2R (0.0f * direction);
        bodyParts.shoulder2->RJI.motorSpeed = 5;

        bodyParts.forearm2->RJI.desiredAngle = D2R (0.0f * direction);
        bodyParts.forearm2->RJI.motorSpeed = 4;
        break;
    }


}

void Entity::constructBody (){
    b2World *world = GeneralInfo::getInstance().world;
    Textures *textures = GeneralInfo::getInstance().textures;

    DisplayData* playerDD = (DisplayData*) new KeyLineData (Color(), DisplayData::Layer::PLAYER);
    float playerWidth = 1; float playerHeight = 6;
    b2BodyDef bodydef;
    bodydef.position.Set(x, y);
    bodydef.type = b2_dynamicBody;
    //bodydef.fixedRotation = true;
    b2Body* body = world->CreateBody(&bodydef);
    setBody(body);
    //  playerDD->isShifting = false;

    body->SetUserData((void*) new UserData(this, playerDD));
    b2PolygonShape shape;
    b2Vec2 points[4] = {b2Vec2(- playerWidth * 1 /2.0f, playerHeight * 0.4 / 2.0f), b2Vec2(playerWidth * 1 /2.0f, playerHeight * 0.4 / 2.0f),
                        b2Vec2(- playerWidth * 1 /3.0f, -playerHeight * 0.4 / 2.0f), b2Vec2(playerWidth * 1 /3.0f, -playerHeight * 0.4 / 2.0f)};
    shape.Set(points, 4);
    //shape.SetAsBox(playerWidth * 1 / 2.0f, playerHeight * 0.4 / 2.0f, b2Vec2(0, 0), 0);

    b2FixtureDef fixturedef;
    fixturedef.shape = &shape;
    fixturedef.filter.categoryBits = GeneralInfo::CollisionType::BODYPART;
    fixturedef.filter.maskBits = GeneralInfo::CollisionType::BASIC;
    fixturedef.density = 0.65f;

    b2Fixture* mainFixture = body->CreateFixture(&fixturedef);

    DisplayData* bodyDD = (DisplayData*) new KeyLineData(Color(0, 255, 0), DisplayData::Layer::PLAYER);

    BodyPart* BPBody = new BodyPart(this, BodyPart::Type::BODY, body);
    GameObject* bodyPart = BPBody;

    mainFixture->SetUserData((void*) new UserData (bodyPart, new TextureData(
                                                       textures->getTexture(this->getTextureType(BodyPart::Type::BODY)), DisplayData::Layer::PLAYER)));





    ///

    float motorTorque = body->GetMass() * 13;

    //leg

    b2RevoluteJoint* RJhip, *RJhip2, *RJknee, *RJknee2, *RJfoot, *RJfoot2;
    for (int i = 0; i < 2; ++i){
        b2BodyDef bodydefHip;
        bodydefHip.position.Set(x, y - playerHeight * (0.4 + 0.2) / 2.0f);
        bodydefHip.type = b2_dynamicBody;
        bodydefHip.fixedRotation = false;
        b2Body* hip = world->CreateBody(&bodydefHip);
        shape.SetAsBox(playerWidth * 0.4 / 2.0f, playerHeight * 0.2 / 2.0f);

        fixturedef.density = 1.025f;
        fixturedef.shape = &shape;

        mainFixture = hip->CreateFixture(&fixturedef);
        if (i)
            bodyDD = (DisplayData*) new KeyLineData(Color(0, 255, 0), DisplayData::Layer::PLAYER);
        else
            bodyDD = (DisplayData*) new KeyLineData(Color(255, 255, 0), DisplayData::Layer::PLAYER);

        BodyPart* BPHip = new BodyPart(this, BodyPart::Type::HIP, hip);
        bodyPart = BPHip;

        DisplayData::Layer layer;
        if (i) layer = DisplayData::Layer::PLAYER_FAR;
        else layer = DisplayData::Layer::PLAYER_NEAR;
        mainFixture->SetUserData((void*) new UserData (bodyPart, new TextureData(
                                                           textures->getTexture(getTextureType(BodyPart::Type::HIP)), layer)));


        b2RevoluteJointDef RJDhip;
        RJDhip.enableLimit = true;
        RJDhip.upperAngle = M_PI * 0.9;
        RJDhip.lowerAngle = - M_PI / 6;
        RJDhip.maxMotorTorque = motorTorque * 5.0f;
        RJDhip.Initialize(body, hip, b2Vec2(body->GetPosition().x, body->GetPosition().y - playerHeight * 0.4 / 2.0f));
        b2RevoluteJoint* RJhipTemp = static_cast<b2RevoluteJoint*> (world->CreateJoint(&RJDhip));
        RJhipTemp->EnableMotor(true);


        float offset = 0.05;

        b2BodyDef bodydefShin;
        bodydefShin.position.Set(x, y - playerHeight * ( (0.4 + 0.2) / 2.0f + 0.2 - offset  ));
        bodydefShin.type = b2_dynamicBody;
        bodydefShin.fixedRotation = false;
        b2Body* shin = world->CreateBody(&bodydefShin);
        shape.SetAsBox(playerWidth * 0.4 / 2.0f, playerHeight * 0.2 / 2.0f);

        fixturedef.shape = &shape;

        mainFixture = shin->CreateFixture(&fixturedef);

        BodyPart* BPShin = new BodyPart(this, BodyPart::Type::SHIN, shin);
        bodyPart = BPShin;

        mainFixture->SetUserData((void*) new UserData (bodyPart, new TextureData(
                                                           textures->getTexture(getTextureType(BodyPart::Type::SHIN)), layer)));


        b2RevoluteJointDef RJDknee;
        RJDknee.enableLimit = true;
        RJDknee.upperAngle = 0;
        //RJDknee.lowerAngle = - M_PI + R2D(10);
        RJDknee.lowerAngle = - D2R (170.0f);
        RJDknee.maxMotorTorque = motorTorque * 2;
        RJDknee.Initialize(hip, shin, b2Vec2(hip->GetPosition().x, hip->GetPosition().y - playerHeight * (0.2 - offset) / 2.0f));
        b2RevoluteJoint* RJkneeTemp =  static_cast<b2RevoluteJoint*> (world->CreateJoint(&RJDknee));
        RJkneeTemp->EnableMotor(true);



        b2BodyDef bodydefFoot;
        bodydefFoot.position.Set(x, shin->GetPosition().y - playerHeight *  0.13);
        bodydefFoot.type = b2_dynamicBody;
        bodydefFoot.fixedRotation = false;
        b2Body* foot = world->CreateBody(&bodydefFoot);
        shape.SetAsBox(playerHeight * 0.04 / 2.0f, playerWidth * 0.8 / 2.0f);
        fixturedef.friction = 3;
        fixturedef.shape = &shape;

        mainFixture = foot->CreateFixture(&fixturedef);
        fixturedef.friction = 1;
        bodyDD = (DisplayData*) new KeyLineData(Color(0, 255, 255), DisplayData::Layer::PLAYER);


        BodyPart* BPFoot = new BodyPart(this, BodyPart::Type::FOOT, foot);
        bodyPart = BPFoot;

        mainFixture->SetUserData((void*) new UserData (bodyPart, new TextureData(
                                                           textures->getTexture(getTextureType(BodyPart::Type::FOOT)), layer)));

        b2RevoluteJointDef RJDfoot;
        RJDfoot.enableLimit = false;
        RJDfoot.upperAngle = M_PI;
        RJDfoot.lowerAngle = 0 + M_PI / 4;
        RJDfoot.maxMotorTorque = motorTorque;
        RJDfoot.Initialize(shin, foot, b2Vec2(foot->GetPosition().x, foot->GetPosition().y + playerHeight * 0.03));
        b2RevoluteJoint* RJfootTemp =  static_cast<b2RevoluteJoint*> (world->CreateJoint(&RJDfoot));
        RJfootTemp->EnableMotor(true);

        BPHip->RJI.RJ = RJhipTemp;
        BPShin->RJI.RJ = RJkneeTemp;
        BPFoot->RJI.RJ = RJfootTemp;

        if (i){
            RJfoot = RJfootTemp;
            RJknee = RJkneeTemp;
            RJhip = RJhipTemp;
        }
        else{
            RJfoot2 = RJfootTemp;
            RJknee2 = RJkneeTemp;
            RJhip2 = RJhipTemp;
        }
        bodyParts.setPart(BPHip);
        bodyParts.setPart(BPShin);
        bodyParts.setPart(BPFoot);
    }
    //

    b2BodyDef bodydefHead;
    bodydefHead.position.Set(x, y + playerHeight * (0.4 / 2.0f) + playerWidth * 0.5);
    bodydefHead.type = b2_dynamicBody;
    //bodydefHead.fixedRotation = false;
    b2Body* head = world->CreateBody(&bodydefHead);
    b2CircleShape circleShape;
    circleShape.m_radius = playerWidth * 0.5;

    fixturedef.shape = &circleShape;

    mainFixture = head->CreateFixture(&fixturedef);

    bodyDD = (DisplayData*) new KeyLineData(Color(255, 255, 100), DisplayData::Layer::PLAYER);

    BodyPart* BPHead = new BodyPart(this, BodyPart::Type::HEAD, head);
    bodyPart = BPHead;
    DisplayData* DD = new TextureData( textures->getTexture(getTextureType(BodyPart::Type::HEAD)), DisplayData::Layer::PLAYER);
    BPHead->DD = DD;
    mainFixture->SetUserData((void*) new UserData (bodyPart, DD));


    b2RevoluteJointDef RJDhead;
    RJDhead.enableLimit = true;
    RJDhead.upperAngle = 0.1;
    RJDhead.lowerAngle = -0.1;
    RJDhead.maxMotorTorque = motorTorque;
    RJDhead.Initialize(body, head, b2Vec2(head->GetPosition().x, head->GetPosition().y - playerWidth * 0.5));
    world->CreateJoint(&RJDhead);


    bodyParts.setPart(BPBody);
    bodyParts.setPart(BPHead);

    ///

    //arms
    b2RevoluteJoint* RJshoulder, *RJshoulder2, *RJforearm, *RJforearm2, *RJwrist, *RJwrist2;
    for (int i = 0; i < 2; ++i){
        b2BodyDef bodydefshoulder;
        bodydefshoulder.position.Set(x, y + playerHeight * (0.4 - 0.2) / 2.0f);
        bodydefshoulder.type = b2_dynamicBody;
        bodydefshoulder.fixedRotation = false;
        b2Body* shoulder = world->CreateBody(&bodydefshoulder);
        shape.SetAsBox(playerWidth * 0.4 / 2.0f, playerHeight * 0.2 / 2.0f);

        fixturedef.shape = &shape;

        mainFixture = shoulder->CreateFixture(&fixturedef);
        DisplayData::Layer layer;
        if (i){
            layer = DisplayData::Layer::PLAYER_FAR_FAR;
            bodyDD = (DisplayData*) new KeyLineData(Color(0, 255, 0), DisplayData::Layer::PLAYER);
        }
        else{
            bodyDD = (DisplayData*) new KeyLineData(Color(255, 255, 0), DisplayData::Layer::PLAYER);
            layer = DisplayData::Layer::PLAYER_NEAR;
        }
        BodyPart* BPshoulder = new BodyPart(this, BodyPart::Type::SHOULDER, shoulder);
        bodyPart = BPshoulder;


        mainFixture->SetUserData((void*) new UserData (bodyPart,  new TextureData(
                                                           textures->getTexture(getTextureType(BodyPart::Type::SHOULDER)), layer)));


        b2RevoluteJointDef RJDshoulder;
        RJDshoulder.enableLimit = true;
        RJDshoulder.upperAngle = M_PI;
        RJDshoulder.lowerAngle = - M_PI / 2;
        RJDshoulder.maxMotorTorque = motorTorque*2;
        RJDshoulder.Initialize(body, shoulder, b2Vec2(body->GetPosition().x, body->GetPosition().y + playerHeight * 0.4 / 2.0f));
        b2RevoluteJoint* RJshoulderTemp = static_cast<b2RevoluteJoint*> (world->CreateJoint(&RJDshoulder));
        RJshoulderTemp->EnableMotor(true);


        float offset = 0.1;

        b2BodyDef bodydefforearm;
        // bodydefforearm.position.Set(x, y - playerHeight * ( (0.4 + 0.2) / 2.0f + 0.2 - offset  ));
        bodydefforearm.position.Set(x, y - playerHeight * (0.2 / 2.0f));
        bodydefforearm.type = b2_dynamicBody;
        bodydefforearm.fixedRotation = false;
        b2Body* forearm = world->CreateBody(&bodydefforearm);
        shape.SetAsBox(playerWidth * 0.4 / 2.0f, playerHeight * 0.2 / 2.0f);

        fixturedef.shape = &shape;

        mainFixture = forearm->CreateFixture(&fixturedef);

        BodyPart* BPforearm = new BodyPart(this, BodyPart::Type::FOREARM, forearm);
        bodyPart = BPforearm;


        mainFixture->SetUserData((void*) new UserData (bodyPart, new TextureData(
                                                           textures->getTexture(getTextureType(BodyPart::Type::FOREARM)), layer)));


        b2RevoluteJointDef RJDforearm;
        RJDforearm.enableLimit = true;
        RJDforearm.upperAngle = M_PI;
        RJDforearm.lowerAngle = 0;
        RJDforearm.maxMotorTorque = motorTorque*2;
        //RJDforearm.Initialize(shoulder, forearm, b2Vec2(shoulder->GetPosition().x, shoulder->GetPosition().y - playerHeight * (0.2 - offset) / 2.0f));
        RJDforearm.Initialize(shoulder, forearm, b2Vec2(shoulder->GetPosition().x, body->GetPosition().y));

        b2RevoluteJoint* RJforearmTemp =  static_cast<b2RevoluteJoint*> (world->CreateJoint(&RJDforearm));
        RJforearmTemp->EnableMotor(true);



        b2BodyDef bodydefwrist;
        //bodydefwrist.position.Set(x, y - playerHeight * ( (0.4 / 2.0f + 0.2 + 0.2 - offset - 0.025 / 2  )));
        bodydefwrist.position.Set(x, y - playerHeight * (0.4 / 2.0f));
        bodydefwrist.type = b2_dynamicBody;
        bodydefwrist.fixedRotation = false;
        b2Body* wrist = world->CreateBody(&bodydefwrist);
        shape.SetAsBox(playerHeight * 0.025 / 2.0f, playerWidth * 0.9 / 2.0f);

        fixturedef.shape = &shape;

        mainFixture = wrist->CreateFixture(&fixturedef);

        bodyDD = (DisplayData*) new KeyLineData(Color(0, 255, 255), DisplayData::Layer::PLAYER);


        BodyPart* BPwrist = new BodyPart(this, BodyPart::Type::WRIST, wrist);
        bodyPart = BPwrist;

        mainFixture->SetUserData((void*) new UserData (bodyPart, new TextureData(
                                                           textures->getTexture(getTextureType(BodyPart::Type::WRIST)), layer)));

        b2RevoluteJointDef RJDwrist;
        RJDwrist.enableLimit = true;
        RJDwrist.upperAngle = 1;
        RJDwrist.lowerAngle = -1;
        RJDwrist.maxMotorTorque = motorTorque;
        //RJDwrist.Initialize(forearm, wrist, b2Vec2(wrist->GetPosition().x - 0.9 / 2.0f, wrist->GetPosition().y));
        RJDwrist.Initialize(forearm, wrist, b2Vec2(wrist->GetPosition().x, wrist->GetPosition().y + playerWidth * 0.9 / 2.0f));
        b2RevoluteJoint* RJwristTemp =  static_cast<b2RevoluteJoint*> (world->CreateJoint(&RJDwrist));
        RJwristTemp->EnableMotor(true);

        BPshoulder->RJI.RJ = RJshoulderTemp;
        BPforearm->RJI.RJ = RJforearmTemp;
        BPwrist->RJI.RJ = RJwristTemp;

        if (i){
            RJwrist = RJwristTemp;
            RJforearm = RJforearmTemp;
            RJshoulder = RJshoulderTemp;
        }
        else{
            RJwrist2 = RJwristTemp;
            RJforearm2 = RJforearmTemp;
            RJshoulder2 = RJshoulderTemp;
        }
        this->bodyParts.setPart(BPshoulder);
        this->bodyParts.setPart(BPforearm);
        this->bodyParts.setPart(BPwrist);
    }

    body->SetAngularDamping(5);
}


void Entity::rotate(bool right)
{
    if ( (right && !isRightDirection) || (!right && isRightDirection) ){
        if (isRightDirection) isRightDirection = false;
        else isRightDirection = true;
        bodyParts.mirror();
        if(weapon) weapon->rotate(right);
    }
}

void Entity::move()
{
    float direction;
    if (isRightDirection) direction = 1;
    else direction = -1;

    if (isRightDirection){
        bodyParts.foot->RJI.desiredAngle = surfaceAngle + M_PI / 2;
        bodyParts.foot2->RJI.desiredAngle = surfaceAngle + M_PI / 2;
    }
    else{
        bodyParts.foot->RJI.desiredAngle = surfaceAngle + M_PI / 2 + M_PI;
        bodyParts.foot2->RJI.desiredAngle = surfaceAngle + M_PI / 2 + M_PI;
    }
    BodyPart *hip, *shin;
    BodyPart *shoulder, *forearm, *foot;
    if (isUsingLeftLeg){
        hip = bodyParts.hip;
        shin = bodyParts.shin;
        foot = bodyParts.foot;
        shoulder = bodyParts.shoulder2;
        forearm = bodyParts.forearm2;
    }
    else{
        hip = bodyParts.hip2;
        shin = bodyParts.shin2;
        foot = bodyParts.foot2;
        shoulder = bodyParts.shoulder;
        forearm = bodyParts.forearm;
    }
    if (isAscendingLeg){
        float hipMaxAngle = D2R(70.0f) + surfaceAngle * 2;
        if (moveStateVertical == MSV_UP)
            hipMaxAngle = D2R(80.0f);
        float shinMaxAngle = D2R(-50.0f) - surfaceAngle;
        if ( (hip->RJI.RJ->GetJointAngle() < hipMaxAngle && isRightDirection ) ||
             (hip->RJI.RJ->GetJointAngle() > - hipMaxAngle && !isRightDirection)){
            hip->RJI.desiredAngle = (hipMaxAngle + D2R(10)) * direction;
            hip->RJI.motorSpeed = 2.3;
            shin->RJI.desiredAngle = shinMaxAngle * direction;
            shin->RJI.motorSpeed = 3;
            if (moveStateVertical == MSV_UP)
                if (isRightDirection)
                    foot->RJI.desiredAngle = D2R(110);
                else foot->RJI.desiredAngle = D2R(110 + 90);

            if(attackState != AS_SWING && isUsingLeftLeg)
            {
                shoulder->RJI.desiredAngle = D2R (30.0f * direction);
                shoulder->RJI.motorSpeed = 1;

                forearm->RJI.desiredAngle = D2R (40.0f * direction);
                forearm->RJI.motorSpeed = 1;
            }

        }
        else{
            isAscendingLeg = false;

            if (isUsingLeftLeg) bodyParts.foot->RJI.desiredAngle = M_PI / 2;
            else bodyParts.foot2->RJI.desiredAngle = M_PI / 2;

            shoulder->RJI.desiredAngle = D2R (-5.0f * direction);
            forearm->RJI.desiredAngle = 0;

            hip->RJI.desiredAngle = D2R (-15.0f * direction);
            if (moveStateVertical == MSV_UP){
                shin->RJI.desiredAngle = D2R(-20) * direction;
            }
            else
                shin->RJI.desiredAngle = D2R(0) * direction;
            float verticalImpulse = body->GetMass() * (8 + surfaceAngle * 12);
            if (moveStateVertical == MSV_UP){
                body->ApplyLinearImpulse(b2Vec2 (body->GetMass() * 8 * direction, verticalImpulse * 2), body->GetPosition() - b2Vec2(0, 3), true);
                float footAngle = 110;
                if (!isRightDirection)
                    footAngle = 180 + 90 - 10;
                bodyParts.foot->RJI.desiredAngle = D2R(footAngle);
                bodyParts.foot2->RJI.desiredAngle = D2R(footAngle);
            }
            else
                body->ApplyLinearImpulse(b2Vec2 (body->GetMass() * 6 * direction, verticalImpulse), body->GetPosition() - b2Vec2(0, 3), true);

        }

    }
    else{
        //descending leg

        float ascendLimit = 5;

        if (moveStateVertical == MSV_UP)
            ascendLimit = D2R(10);
        BodyPart* curFoot;
        if (isUsingLeftLeg) curFoot = bodyParts.foot;
        else curFoot = bodyParts.foot2;

        // if (fabs(bodyParts.hip->RJI.RJ->GetJointAngle() - bodyParts.hip2->RJI.RJ->GetJointAngle()) > 3.0f)
        if ((hip->RJI.RJ->GetJointAngle() < ascendLimit && isRightDirection) ||
                (hip->RJI.RJ->GetJointAngle() > - ascendLimit && !isRightDirection) ||
                ( isRightDirection && isGrounded(isUsingLeftLeg) && hip->RJI.RJ->GetJointAngle() > D2R(10.0f)) ||
                ( !isRightDirection && isGrounded(isUsingLeftLeg) && hip->RJI.RJ->GetJointAngle() < D2R(-10.0f))
                ){
            if (( isRightDirection && isGrounded(isUsingLeftLeg) && hip->RJI.RJ->GetJointAngle() > D2R(10.0f)) ||
                    ( !isRightDirection && isGrounded(isUsingLeftLeg) && hip->RJI.RJ->GetJointAngle() < D2R(-10.0f)))
                body->ApplyLinearImpulse(b2Vec2(0, body->GetMass() * 10), body->GetPosition(), true);
            /*
                BodyPart* foot;
                if (isUsingLeftLeg) foot = bodyParts.foot;
                else foot = bodyParts.foot2;
                if (isRightDirection)
                    surfaceAngle =  foot->RJI.RJ->GetJointAngle() - M_PI / 2;
                else surfaceAngle = M_PI / 2 - foot->RJI.RJ->GetJointAngle() + M_PI;
                qDebug()<<R2D(surfaceAngle);
                */
            isAscendingLeg = true;
            changeLeg();
        }
    }
}

void Entity::useObject(){
    if (!useCooldown){
        useCooldown = useCooldownMax;
        if (vehicle) vehicle->left(this);
        else
            if (isOnLadder) isOnLadder = false;
            else
                for (b2ContactEdge* ce = body->GetContactList(); ce; ce = ce->next)
                {
                    b2Contact* c = ce->contact;
                    UserData* dataA = static_cast<UserData*>(c->GetFixtureA()->GetUserData());
                    if (dataA->gameObject){
                        InteractiveObject* objA = dynamic_cast<InteractiveObject*> (dataA->gameObject);
                        if (objA){
                            objA->use(this);
                            break;
                        }
                    }

                    UserData* dataB = static_cast<UserData*>(c->GetFixtureB()->GetUserData());
                    if (dataB->gameObject){
                        InteractiveObject* objB = dynamic_cast<InteractiveObject*> (dataB->gameObject);
                        if (objB){
                            objB->use(this);
                            break;
                        }
                    }
                }
    }
}

bool Entity::checkForLadder(){
    bool isLadder = false;
    for (b2ContactEdge* ce = body->GetContactList(); ce; ce = ce->next)
    {
        b2Contact* c = ce->contact;
        UserData* dataA = static_cast<UserData*>(c->GetFixtureA()->GetUserData());
        GameObject* objA = dataA->gameObject;
        Ladder* ladderA = dynamic_cast<Ladder*> (objA);

        UserData* dataB = static_cast<UserData*>(c->GetFixtureB()->GetUserData());
        GameObject* objB = dataB->gameObject;
        Ladder* ladderB = dynamic_cast<Ladder*> (objB);

        if (ladderA || ladderB){
            isLadder = true;
            break;
        }
    }
    return isLadder;
}

void Entity::setBody (b2Body* body){
    this->body = body;
}


void Entity::applyForce(){
    if (vehicle){
        float desiredVel = 0;
        float desiredVelMax = 1 * 360 / 180 * M_PI;
        switch (moveState)
        {
        case MS_LEFT:  desiredVel = desiredVelMax; break;
        case MS_STAND:  desiredVel =  0; break;
        case MS_RIGHT: desiredVel =  - desiredVelMax; break;
        }
        vehicle->motor->SetMotorSpeed(desiredVel);
    }
    else {

        float direction;
        if (isRightDirection) direction = 1;
        else direction = -1;


        BodyPart* foot;
        BodyPart* hip;
        BodyPart* shin;

        BodyPart* forearm;
        BodyPart* shoulder;
        BodyPart* wrist;

        bodyParts.head->RJI.desiredAngle = 0;
        bodyParts.body->RJI.desiredAngle = 0;


        for (int i = 0; i < 2; ++i){
            if (i){
                hip = bodyParts.hip;
                shin = bodyParts.shin;
                shoulder = bodyParts.shoulder;
                forearm = bodyParts.forearm;
                foot = bodyParts.foot;
                wrist = bodyParts.wrist;
            }
            else {
                hip = bodyParts.hip2;
                shin = bodyParts.shin2;
                shoulder = bodyParts.shoulder2;
                forearm = bodyParts.forearm2;
                foot = bodyParts.foot2;
                wrist = bodyParts.wrist2;
            }
            //straight body parts
            if (isRightDirection)
                foot->RJI.desiredAngle = M_PI / 2;
            else foot->RJI.desiredAngle = M_PI / 2 + M_PI;
            foot->RJI.angleDeviation = D2R(10.0f);
            if (moveState == MoveState::MS_STAND){
                body->SetLinearDamping(5);
                shoulder->RJI.desiredAngle = 0;
                forearm->RJI.desiredAngle = 0;
                shin->RJI.desiredAngle = 0;
                hip->RJI.desiredAngle = 0;
                wrist->RJI.desiredAngle = 0;

                bodyParts.resetSpeed();
            }
            else body->SetLinearDamping(0);
        }


        if (fabs(shin->RJI.desiredAngle - shin->RJI.RJ->GetJointAngle() < shin->RJI.angleDeviation / 2)){
            isStanding = true;
        }
        else isStanding = false;

        //jumping

        if (isJumping)
            jump();


        //moving
        switch ( moveState ){
        case MS_LEFT :
            rotate (false);

            move ();
            break;
        case MS_RIGHT :
            rotate (true);
            move ();
            break;
        case MS_STAND:
            if (R2D(shin->body->GetAngle() - shin->body->GetAngle()) < 10.0f)
                isAscendingLeg = true;
            break;
        }

        // attacking
        switch (attackState) {
        case AS_SWING:
            attack();
            break;
        case AS_HIT:
            attack();
            break;
        }

        //vertical forces
        if (isOnLadder){
            body->SetLinearVelocity(b2Vec2(0, 0));
            body->ApplyForceToCenter(b2Vec2 (0, 9.82 * body->GetMass()), true);
            if (moveStateVertical == MSV_UP)
                body->ApplyForceToCenter(b2Vec2 (0, 100 * body->GetMass()), true);
            if (moveStateVertical == MSV_DOWN)
                body->ApplyForceToCenter(b2Vec2 (0, - 100 * body->GetMass()), true);
        }
        else{
            if (moveStateVertical == MSV_UP)
                if (checkForLadder()) isOnLadder = true;
            if (moveStateVertical == MSV_DOWN)
                crouch();
        }
    }
    bodyParts.setSpeed();


}


void Entity::changeLeg(){
    if (isUsingLeftLeg)
        isUsingLeftLeg = false;
    else isUsingLeftLeg = true;
}

void Entity::crouch(){
    for (int i = 0; i < 2; ++i){
        BodyPart *shoulder, *forearm, *wrist, *hip, *shin, *foot;
        if (i){
            shoulder = bodyParts.shoulder;
            forearm = bodyParts.forearm;
            wrist = bodyParts.wrist;
            hip = bodyParts.hip;
            shin = bodyParts.shin;
            foot = bodyParts.foot;
        }
        else{
            shoulder = bodyParts.shoulder2;
            forearm = bodyParts.forearm2;
            wrist = bodyParts.wrist2;
            hip = bodyParts.hip2;
            shin = bodyParts.shin2;
            foot = bodyParts.foot2;
        }
        float direction;
        if (isRightDirection)
            direction = 1;
        else direction = -1;

        forearm->RJI.desiredAngle = D2R (90) * direction;

        hip->RJI.desiredAngle = D2R (90) * direction;
        shin->RJI.desiredAngle = D2R (-130) * direction;
        if (isRightDirection)
            foot->RJI.desiredAngle = M_PI / 2 + D2R(45);
        else foot->RJI.desiredAngle = M_PI / 2 - D2R(45) + M_PI;


        if (fabs(fabs(shin->RJI.desiredAngle - shin->RJI.RJ->GetJointAngle() < shin->RJI.angleDeviation / 4) &&
                 fabs(hip->RJI.desiredAngle - hip->RJI.RJ->GetJointAngle() < hip->RJI.angleDeviation / 4))){
            isSitting = true;
        }
        else isSitting = false;
    }
}

void Entity::fall()
{
    body->ApplyTorque(30000, true);
}

bool Entity::isGrounded(){
    for (b2ContactEdge* ce = bodyParts.foot->body->GetContactList(); ce; ce = ce->next)
    {
        if (ce->contact->IsTouching())
            return true;
    }
    for (b2ContactEdge* ce = bodyParts.foot2->body->GetContactList(); ce; ce = ce->next)
    {
        if (ce->contact->IsTouching())
            return true;
    }
    return false;
}

bool Entity::isGrounded(bool leftLeg){
    if (leftLeg)
        for (b2ContactEdge* ce = bodyParts.foot->body->GetContactList(); ce; ce = ce->next)
        {
            if (ce->contact->IsTouching())
                return true;
        }
    else
        for (b2ContactEdge* ce = bodyParts.foot2->body->GetContactList(); ce; ce = ce->next)
        {
            if (ce->contact->IsTouching())
                return true;
        }
    return false;
}

void Entity::jump(){
    if (isGrounded())
        if (!isSitting)
            crouch();
        else{
            for (int i = 0; i < 2; ++i){
                BodyPart *shoulder, *forearm, *wrist, *hip, *shin, *foot;
                if (i){
                    hip = bodyParts.hip;
                    shin = bodyParts.shin;
                    shoulder = bodyParts.shoulder;
                    forearm = bodyParts.forearm;
                    foot = bodyParts.foot;
                    wrist = bodyParts.wrist;
                }
                else {
                    hip = bodyParts.hip2;
                    shin = bodyParts.shin2;
                    shoulder = bodyParts.shoulder2;
                    forearm = bodyParts.forearm2;
                    foot = bodyParts.foot2;
                    wrist = bodyParts.wrist2;
                }
                float jumpK = 2;
                hip->RJI.motorSpeed *= jumpK;
                shin->RJI.motorSpeed *= jumpK;
                foot->RJI.motorSpeed *= jumpK;

            }
            if (isStanding){
                isSitting = false;
                float angle = body->GetAngle();
                float impulse = body->GetMass() * 100;
                body->ApplyLinearImpulse(b2Vec2(impulse * sin(angle), impulse * cos(angle)), body->GetWorldCenter(), true);
            }
        }

    /*
    float jumpHeight;

    jumpHeight = 30;

    isOnLadder = false;
    if (!jumpCooldown){
        jumpCooldown = jumpCooldownMax;
        if (isGrounded())
            body->ApplyLinearImpulse(b2Vec2(0, body->GetMass() * jumpHeight), body->GetPosition(), true);
    }
            */

}

void Entity::update()
{

    //rotate body
    float maxDeviation = 0.005;
    float maxDeviationSpeed = 20;
    if (moveStateVertical == MSV_UP){
        maxDeviation *= 2;
        maxDeviationSpeed /= 3;
    }
    float deviationSpeed = fabs( maxDeviationSpeed  * body->GetAngle() );
    if (!vehicle){
        if (body->GetAngle() > maxDeviation)
            body->SetAngularVelocity (-deviationSpeed);
        else
            if (body->GetAngle() < - maxDeviation)
                body->SetAngularVelocity (deviationSpeed);
        // if (body->GetAngle() > maxDeviation * 2.0f)
        //     body->SetAngularVelocity(body->GetAngularVelocity() / 2.0f);
    }

    if (!isJumping) isSitting = false;
    if (jumpCooldown) --jumpCooldown;
    if (useCooldown) --useCooldown;
    if (isOnLadder && !checkForLadder()) isOnLadder = false;
    if (isRightDirection) static_cast<TextureData*>(bodyParts.head->DD)->isMirrored = false;
    else static_cast<TextureData*>(bodyParts.head->DD)->isMirrored = true;
    applyForce();
}

