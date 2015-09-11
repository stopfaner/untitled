#include "entity.h"

#include <qdebug.h>

#define M_PI		3.14159265358979323846

float D2R (float degrees){
    return degrees * M_PI / 180.0f;
}
float R2D (float radians){
    return radians * 180.0f / M_PI;
}

Entity::Entity()
{

}

Entity::Entity(Textures *textures , b2World *world) : GameObject ()
{
    moveState = MS_STAND;
    moveStateVertical = MSV_STAND;
    isOnLadder = false;
    isJumping = false;
    isUsingLeftLeg = true;
    isAscendingLeg = true;
    isRightDirection = true;
    jumpCooldown = 0;
    jumpCooldownMax = 40;
    useCooldown = 0;
    useCooldownMax = 10;
    vehicle = nullptr;


    DisplayData* playerDD = (DisplayData*) new KeyLineData (Color(), DisplayData::Layer::PLAYER);
    float x = 20, y = 10;
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
    fixturedef.filter.categoryBits = Enum::CollisionType::BODYPART;
    fixturedef.filter.maskBits = Enum::CollisionType::BASIC;
    fixturedef.density = 0.7;

    b2Fixture* mainFixture = body->CreateFixture(&fixturedef);

    DisplayData* bodyDD = (DisplayData*) new KeyLineData(Color(0, 255, 0), DisplayData::Layer::PLAYER);

    BodyPart* BPBody = new BodyPart(this, BodyPart::Type::BODY, body);
    GameObject* bodyPart = BPBody;

    mainFixture->SetUserData((void*) new UserData (bodyPart, new TextureData(
                                                       textures->getTexture(Textures::Type::BODY), DisplayData::Layer::PLAYER)));

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
                                                            textures->getTexture(Textures::Type::HIP), layer)));


        b2RevoluteJointDef RJDhip;
        RJDhip.enableLimit = true;
        RJDhip.upperAngle = M_PI * 0.9;
        RJDhip.lowerAngle = - M_PI / 6;
        RJDhip.maxMotorTorque = motorTorque * 5.0f;
        RJDhip.Initialize(body, hip, b2Vec2(body->GetWorldCenter().x, body->GetWorldCenter().y - playerHeight * 0.4 / 2.0f));
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

        if (i) mainFixture->SetUserData((void*) new UserData (bodyPart, new TextureData(
                                                                   textures->getTexture(Textures::Type::TEST2), layer)));
        else
            mainFixture->SetUserData((void*) new UserData (bodyPart, new TextureData(
                                                                textures->getTexture(Textures::Type::SHIN), layer)));


        b2RevoluteJointDef RJDknee;
        RJDknee.enableLimit = true;
        RJDknee.upperAngle = 0;
        RJDknee.lowerAngle = - M_PI / 2;
        RJDknee.maxMotorTorque = motorTorque * 1.5;
        RJDknee.Initialize(hip, shin, b2Vec2(hip->GetWorldCenter().x, hip->GetWorldCenter().y - playerHeight * (0.2 - offset) / 2.0f));
        b2RevoluteJoint* RJkneeTemp =  static_cast<b2RevoluteJoint*> (world->CreateJoint(&RJDknee));
        RJkneeTemp->EnableMotor(true);



        b2BodyDef bodydefFoot;
        bodydefFoot.position.Set(x, shin->GetWorldCenter().y - playerHeight *  0.13);
        bodydefFoot.type = b2_dynamicBody;
        bodydefFoot.fixedRotation = false;
        b2Body* foot = world->CreateBody(&bodydefFoot);
        shape.SetAsBox(playerHeight * 0.025 / 2.0f, playerWidth * 0.8 / 2.0f);
        fixturedef.friction = 1.1;
        fixturedef.shape = &shape;

        mainFixture = foot->CreateFixture(&fixturedef);
        fixturedef.friction = 1;
        bodyDD = (DisplayData*) new KeyLineData(Color(0, 255, 255), DisplayData::Layer::PLAYER);


        BodyPart* BPFoot = new BodyPart(this, BodyPart::Type::FOOT, foot);
        bodyPart = BPFoot;

        mainFixture->SetUserData((void*) new UserData (bodyPart, new TextureData(
                                                            textures->getTexture(Textures::Type::FOOT), layer)));

        b2RevoluteJointDef RJDfoot;
        RJDfoot.enableLimit = false;
        RJDfoot.upperAngle = M_PI;
        RJDfoot.lowerAngle = 0;
        RJDfoot.maxMotorTorque = motorTorque;
        RJDfoot.Initialize(shin, foot, b2Vec2(foot->GetWorldCenter().x, foot->GetWorldCenter().y + playerHeight * 0.03));
        b2RevoluteJoint* RJfootTemp =  static_cast<b2RevoluteJoint*> (world->CreateJoint(&RJDfoot));
        RJfootTemp->EnableMotor(true);

        BPHip->RJ = RJhipTemp;
        BPShin->RJ = RJkneeTemp;
        BPFoot->RJ = RJfootTemp;

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
    DisplayData* DD = new TextureData( textures->getTexture(Textures::Type::HEAD), DisplayData::Layer::PLAYER);
    BPHead->DD = DD;
    mainFixture->SetUserData((void*) new UserData (bodyPart, DD));


    b2RevoluteJointDef RJDhead;
    RJDhead.enableLimit = true;
    RJDhead.upperAngle = 0.1;
    RJDhead.lowerAngle = -0.1;
    RJDhead.maxMotorTorque = motorTorque;
    RJDhead.Initialize(body, head, b2Vec2(head->GetWorldCenter().x, head->GetWorldCenter().y - playerWidth * 0.5));
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
            layer = DisplayData::Layer::PLAYER_FAR;
            bodyDD = (DisplayData*) new KeyLineData(Color(0, 255, 0), DisplayData::Layer::PLAYER);
        }
        else{
            bodyDD = (DisplayData*) new KeyLineData(Color(255, 255, 0), DisplayData::Layer::PLAYER);
            layer = DisplayData::Layer::PLAYER_NEAR;
        }
        BodyPart* BPshoulder = new BodyPart(this, BodyPart::Type::SHOULDER, shoulder);
        bodyPart = BPshoulder;


        mainFixture->SetUserData((void*) new UserData (bodyPart,  new TextureData(
                                                            textures->getTexture(Textures::Type::SHOULDER), layer)));


        b2RevoluteJointDef RJDshoulder;
        RJDshoulder.enableLimit = true;
        RJDshoulder.upperAngle = M_PI;
        RJDshoulder.lowerAngle = - M_PI / 2;
        RJDshoulder.maxMotorTorque = motorTorque;
        RJDshoulder.Initialize(body, shoulder, b2Vec2(body->GetWorldCenter().x, body->GetWorldCenter().y + playerHeight * 0.4 / 2.0f));
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
                                                            textures->getTexture(Textures::Type::FOREARM), layer)));


        b2RevoluteJointDef RJDforearm;
        RJDforearm.enableLimit = true;
        RJDforearm.upperAngle = M_PI;
        RJDforearm.lowerAngle = 0;
        RJDforearm.maxMotorTorque = motorTorque;
        //RJDforearm.Initialize(shoulder, forearm, b2Vec2(shoulder->GetWorldCenter().x, shoulder->GetWorldCenter().y - playerHeight * (0.2 - offset) / 2.0f));
        RJDforearm.Initialize(shoulder, forearm, b2Vec2(shoulder->GetWorldCenter().x, body->GetWorldCenter().y));

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
                                                            textures->getTexture(Textures::Type::WRIST), layer)));

        b2RevoluteJointDef RJDwrist;
        RJDwrist.enableLimit = true;
        RJDwrist.upperAngle = 1;
        RJDwrist.lowerAngle = -1;
        RJDwrist.maxMotorTorque = motorTorque;
        //RJDwrist.Initialize(forearm, wrist, b2Vec2(wrist->GetWorldCenter().x - 0.9 / 2.0f, wrist->GetWorldCenter().y));
        RJDwrist.Initialize(forearm, wrist, b2Vec2(wrist->GetWorldCenter().x, wrist->GetWorldCenter().y + playerWidth * 0.9 / 2.0f));
        b2RevoluteJoint* RJwristTemp =  static_cast<b2RevoluteJoint*> (world->CreateJoint(&RJDwrist));
        RJwristTemp->EnableMotor(true);

        BPshoulder->RJ = RJshoulderTemp;
        BPforearm->RJ = RJforearmTemp;
        BPwrist->RJ = RJwristTemp;

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
}


void Entity::rotate(bool right)
{
    if ( (right && !isRightDirection) || (!right && isRightDirection) ){
        if (isRightDirection) isRightDirection = false;
        else isRightDirection = true;
        bodyParts.mirror();

    }
}

void Entity::move()
{
    float direction;
    if (isRightDirection) direction = 1;
    else direction = -1;
/*
    if (isGrounded() && fabs(bodyParts.foot->RJ->GetJointAngle() - bodyParts.foot->desiredAngle < D2R(10.0f))){
        qDebug()<<"1";
        bodyParts.foot->angleDeviation = M_PI / 2;
    }
    if (isGrounded() && fabs(bodyParts.foot2->RJ->GetJointAngle() - bodyParts.foot2->desiredAngle < D2R(10.0f))){
        qDebug()<<"2";
        bodyParts.foot2->angleDeviation = M_PI / 2;
    }
*/
    BodyPart *hip, *shin;
    BodyPart *shoulder, *forearm;
    if (isUsingLeftLeg){
        hip = bodyParts.hip;
        shin = bodyParts.shin;
        shoulder = bodyParts.shoulder2;
        forearm = bodyParts.forearm2;
    }
    else{
        hip = bodyParts.hip2;
        shin = bodyParts.shin2;
        shoulder = bodyParts.shoulder;
        forearm = bodyParts.forearm;
    }
    if (isAscendingLeg){
        if ( (hip->RJ->GetJointAngle() < D2R (70.0f) && isRightDirection ) ||
             (hip->RJ->GetJointAngle() > D2R (-70.0f) && !isRightDirection)){
            hip->desiredAngle = D2R (80.0f * direction);
            hip->motorSpeed = 1.8;
            shin->desiredAngle = D2R (-50.0f * direction);
            shin->motorSpeed = 2;

            shoulder->desiredAngle = D2R (30.0f * direction);
            shoulder->motorSpeed = 1;

            forearm->desiredAngle = D2R (40.0f * direction);
            forearm->motorSpeed = 1;
        }
        else{
            isAscendingLeg = false;

            shoulder->desiredAngle = D2R (-5.0f * direction);
            forearm->desiredAngle = 0;

            hip->desiredAngle = D2R (-15.0f * direction);
            shin->desiredAngle = 0;
            body->ApplyLinearImpulse(b2Vec2 (body->GetMass() * 8 * direction, body->GetMass() * 3), body->GetWorldCenter(), true);

        }

    }
    else{
        if ((hip->RJ->GetJointAngle() < 0 && isRightDirection) ||
                (hip->RJ->GetJointAngle() > 0 && !isRightDirection)){
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
        float desiredVelMax = 30 * 360 / 180 * M_PI;
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

        if (isJumping)
            jump();

        BodyPart* foot;
        BodyPart* hip;
        BodyPart* shin;

        BodyPart* forearm;
        BodyPart* shoulder;
        BodyPart* wrist;

        bodyParts.head->desiredAngle = 0;
        bodyParts.body->desiredAngle = 0;


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
                foot->desiredAngle = M_PI / 2;
            else foot->desiredAngle = M_PI / 2 + M_PI;
            foot->angleDeviation = D2R(20.0f);
            if (moveState == MoveState::MS_STAND){
                shoulder->desiredAngle = 0;
                forearm->desiredAngle = 0;
                shin->desiredAngle = 0;
                hip->desiredAngle = 0;
                wrist->desiredAngle = 0;
            }
        }

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
                crouch();//TODO
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
    bodyParts.forearm->RJ->SetMotorSpeed(2);
    bodyParts.forearm2->RJ->SetMotorSpeed(2);
    float crouchAngle = 90.0f;
    if (R2D(bodyParts.shin->RJ->GetJointAngle()) > - crouchAngle){
        bodyParts.shin->RJ->SetMotorSpeed(-0.75);
        bodyParts.hip->RJ->SetMotorSpeed(0.6);

        if (fabs(bodyParts.shin->RJ->GetJointAngle() - crouchAngle) > 3 ){
            body->ApplyAngularImpulse(-1, true);
        }
        body->SetAngularVelocity(-1.75);
    }
    else{
        body->SetAngularVelocity(0);


    }
    if (R2D(bodyParts.shin2->RJ->GetJointAngle()) > - crouchAngle){
        bodyParts.shin2->RJ->SetMotorSpeed(-0.75);
        bodyParts.hip2->RJ->SetMotorSpeed(0.6);

        if (fabs(bodyParts.shin2->RJ->GetJointAngle() - crouchAngle) > 3 ){
            body->ApplyAngularImpulse(-1, true);
        }
        body->SetAngularVelocity(-1.75);
    }
    else{
        body->SetAngularVelocity(0);

    }
}

void Entity::fall()
{
    body->ApplyTorque(30000, true);
}

bool Entity::isGrounded(){
    for (b2ContactEdge* ce = bodyParts.foot->body->GetContactList(); ce; ce = ce->next)
    {
        return true;
        break;
    }
    for (b2ContactEdge* ce = bodyParts.foot2->body->GetContactList(); ce; ce = ce->next)
    {
        return true;
        break;
    }
    return false;
}

void Entity::jump(){
    float jumpHeight;

    jumpHeight = 30;

    isOnLadder = false;
    if (!jumpCooldown){
        jumpCooldown = jumpCooldownMax;
        if (isGrounded())
            body->ApplyLinearImpulse(b2Vec2(0, body->GetMass() * jumpHeight), body->GetWorldCenter(), true);

    }
}

void Entity::update(Textures* textures)
{
    //rotate body
    if (!vehicle && !body->IsFixedRotation()){
        if (body->GetAngle() > 0.01)
            body->SetAngularVelocity (-3 - body->GetAngularVelocity());
        else
            if (body->GetAngle() < - 0.01)
                body->SetAngularVelocity (3 - body->GetAngularVelocity());
        if (fabs(body->GetAngle()) < 0.02)
            body->SetAngularVelocity(body->GetAngularVelocity() / 2.0f);

    }
    if (jumpCooldown) --jumpCooldown;
    if (useCooldown) --useCooldown;
    if (isOnLadder && !checkForLadder()) isOnLadder = false;
    if (isRightDirection) static_cast<TextureData*>(bodyParts.head->DD)->isMirrored = false;
    else static_cast<TextureData*>(bodyParts.head->DD)->isMirrored = true;
    applyForce();
}

