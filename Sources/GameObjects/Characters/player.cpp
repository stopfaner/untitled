#include "player.h"

#include <qdebug.h>

#define M_PI		3.14159265358979323846

float D2R (float degrees){
    return degrees * M_PI / 180.0f;
}
float R2D (float radians){
    return radians * 180.0f / M_PI;
}


Player::Player(DisplayData* displayData) : Entity ()
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
    this->displayData = displayData;
}

void Player::rotate(bool right)
{
    if ( (right && !isRightDirection) || (!right && isRightDirection) ){
        if (isRightDirection) isRightDirection = false;
        else isRightDirection = true;
        bodyParts.mirror();

    }
}

void Player::move()
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
        if ( (hip->RJ->GetJointAngle() < D2R (hipAngle) && isRightDirection ) ||
             (hip->RJ->GetJointAngle() > D2R (-hipAngle) && !isRightDirection)){
            hip->desiredAngle = D2R ((hipAngle + 10.0f) * direction);
            hip->motorSpeed = 1.8;

            float shinAngle = 50.0f;
            if (hipAngle == 100)
                shinAngle = 70;

            shin->desiredAngle = D2R (-shinAngle * direction);
            shin->motorSpeed = 2;

            shoulder->desiredAngle = D2R (30.0f * direction);
            shoulder->motorSpeed = 1;

            forearm->desiredAngle = D2R (40.0f * direction);
            forearm->motorSpeed = 1;

            if (hipAngle == 100)
                body->ApplyLinearImpulse(b2Vec2 (0, body->GetMass() * 1), body->GetWorldCenter(), true);
        }
        else{
            isAscendingLeg = false;

            shoulder->desiredAngle = D2R (-5.0f * direction);
            forearm->desiredAngle = 0;

            hip->desiredAngle = D2R (-15.0f * direction);
            shin->desiredAngle = 0;

            if (hipAngle == 100)
                body->ApplyLinearImpulse(b2Vec2 (body->GetMass() * 3 * direction, body->GetMass() * 5), body->GetWorldCenter(), true);

            else
                body->ApplyLinearImpulse(b2Vec2 (body->GetMass() * 8 * direction, body->GetMass() * 3), body->GetWorldCenter(), true);

        }

    }
    else{
        float lowerBound = 0;
        if (hipAngle == 100) lowerBound = 10;
        if ((hip->RJ->GetJointAngle() < D2R(lowerBound) && isRightDirection) ||
                (hip->RJ->GetJointAngle() > -D2R(lowerBound) && !isRightDirection)){
            isAscendingLeg = true;

            if (bodyParts.foot->RJ->GetJointAngle() > D2R(90.0f + 10.0f))
                hipAngle = 100.0f;
            else hipAngle = 70.0f;



            changeLeg();
        }
    }
}

void Player::useObject(){
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

bool Player::checkForLadder(){
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

void Player::setBody (b2Body* body){
    this->body = body;
}

void Player::chooseTexture(Textures *textures)
{/*
    TextureData* td = static_cast<TextureData*>(displayData);

    if (isOnLadder){
        if (moveStateVertical == MSV_STAND){
            td->setTexture(textures->getTexture(Textures::Type::CLIMBING_IDLE));
        }
        else
            td->setTexture(textures->getTexture(Textures::Type::CLIMBING));
    }
    else{
        if (vehicle)
            td->setTexture(textures->getTexture(Textures::Type::VEHICLE));
        else{
            if (moveState == MS_LEFT)
                td->isMirrored = true;
            if (moveState == MS_RIGHT)
                td->isMirrored = false;
            if (moveState == MS_LEFT
                    || moveState == MS_RIGHT){
                td->setTexture(textures->getTexture(Textures::Type::RUN));
            }
            else
                td->setTexture(textures->getTexture(Textures::Type::PLAYER));
        }
    }
*/
}

void Player::applyForce(){
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
            foot->angleDeviation = D2R(30.0f);
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


void Player::changeLeg(){
    if (isUsingLeftLeg)
        isUsingLeftLeg = false;
    else isUsingLeftLeg = true;
}

void Player::crouch(){
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

void Player::fall()
{
    body->ApplyTorque(30000, true);
}

bool Player::isGrounded(){
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

void Player::jump(){
    float jumpHeight;

    jumpHeight = 30;

    isOnLadder = false;
    if (!jumpCooldown){
        jumpCooldown = jumpCooldownMax;
        if (isGrounded())
            body->ApplyLinearImpulse(b2Vec2(0, body->GetMass() * jumpHeight), body->GetWorldCenter(), true);

    }
}

void Player::update(Textures* textures)
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
    chooseTexture(textures);
}
