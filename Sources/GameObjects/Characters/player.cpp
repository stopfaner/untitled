#include "player.h"

#include <qdebug.h>

#define M_PI		3.14159265358979323846


Player::Player(DisplayData* displayData) : Entity ()
{
    moveState = MS_STAND;
    moveStateVertical = MSV_STAND;
    isOnLadder = false;
    isJumping = false;
    isUsingLeftLeg = true;
    isAscendingLeg = true;
    jumpCooldown = 0;
    jumpCooldownMax = 40;
    useCooldown = 0;
    useCooldownMax = 10;
    vehicle = nullptr;
    this->displayData = displayData;
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
#define M_PI		3.14159265358979323846

float D2R (float degrees){
    return degrees * M_PI / 180.0f;
}
float R2D (float radians){
    return radians * 180.0f / M_PI;
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
        //    float velChange = desiredVel - vehicle->motor->GetMotorSpeed();
        //    vehicle->motor->SetMotorSpeed(velChange);
        vehicle->motor->SetMotorSpeed(desiredVel);
    }
    else {
        b2RevoluteJoint* jointFoot;
        b2RevoluteJoint* jointHip;
        b2RevoluteJoint* jointKnee;
        b2RevoluteJoint* jointShoulder;
        b2RevoluteJoint* jointElbow;
        for (int i = 0; i < 2; ++i){
            if (i){
                jointHip = bodyParts.hip->RJ;
                jointKnee = bodyParts.shin->RJ;
                jointShoulder = bodyParts.shoulder->RJ;
                jointElbow = bodyParts.forearm->RJ;
                jointFoot = bodyParts.foot->RJ;
            }
            else {
                jointHip = bodyParts.hip2->RJ;
                jointKnee = bodyParts.shin2->RJ;
                jointShoulder = bodyParts.shoulder2->RJ;
                jointElbow = bodyParts.forearm2->RJ;
                jointFoot = bodyParts.foot2->RJ;
            }
            //straight arms
            if (moveState == MoveState::MS_STAND){


                //weld foot
                bodyParts.foot->body->SetFixedRotation(true);
                bodyParts.foot2->body->SetFixedRotation(true);
                //



                if (jointFoot->GetJointAngle() > 0.2){
                    jointFoot->SetMotorSpeed(-1);
                }
                else
                    if (jointFoot->GetJointAngle() < - 0.2){
                        jointFoot->SetMotorSpeed(1);
                    }
                    else jointFoot->SetMotorSpeed(0);


                if (jointShoulder->GetJointAngle() > 0.2){
                    jointShoulder->SetMotorSpeed(-1);
                }
                else
                    if (jointShoulder->GetJointAngle() < - 0.2){
                        jointShoulder->SetMotorSpeed(1);
                    }
                    else jointShoulder->SetMotorSpeed(0);


                if (jointElbow->GetJointAngle() > 0.2){
                    jointElbow->SetMotorSpeed(-1);
                }
                else
                    if (jointElbow->GetJointAngle() < - 0.2){
                        jointElbow->SetMotorSpeed(1);
                    }
                    else jointElbow->SetMotorSpeed(0);
            }

            //
            //hips rotate
            if (jointHip->GetJointAngle() > 0.2){
                jointHip->SetMotorSpeed(-5);
            }
            else
                if (jointHip->GetJointAngle() < - 0.2){
                    jointHip->SetMotorSpeed(5);
                }
                else jointHip->SetMotorSpeed(0);
            //

            //knee rotate
            if (jointKnee->GetJointAngle() > 0){

                jointKnee->SetMotorSpeed(-2);
                jointKnee->EnableMotor(true);
            }
            else
                if (jointKnee->GetJointAngle() < - 0.3){

                    jointKnee->SetMotorSpeed(2);
                    jointKnee->EnableMotor(true);
                }
                else jointKnee->SetMotorSpeed(0);
        }

        if (isJumping)
            jump();

        //moving legs
        switch ( moveState ){
        case MS_RIGHT :
            b2RevoluteJoint *hip, *shin;
            b2RevoluteJoint *shoulder, *elbow;
            if (isUsingLeftLeg){
                hip = bodyParts.hip->RJ;
                shin = bodyParts.shin->RJ;
                shoulder = bodyParts.shoulder2->RJ;
                elbow = bodyParts.forearm2->RJ;
            }
            else{
                hip = bodyParts.hip2->RJ;
                shin = bodyParts.shin2->RJ;
                shoulder = bodyParts.shoulder->RJ;
                elbow = bodyParts.forearm->RJ;
            }
            if (isAscendingLeg){

                if (R2D(hip->GetJointAngle()) < 60.0f){
                    hip->SetMotorSpeed(2);

                    shoulder->SetMotorSpeed(0.5);
                    elbow->SetMotorSpeed(1);
                }
                else{
                    shoulder->SetMotorSpeed(-0.5/4);
                    elbow->SetMotorSpeed(-1.0/4);

                    isAscendingLeg = false;
                    hip->SetMotorSpeed(-2);
                    body->ApplyLinearImpulse(b2Vec2 (body->GetMass() * 9, 0), body->GetWorldCenter(), true);

                }

                if (R2D(shin->GetJointAngle()) > -40.0f)
                    shin->SetMotorSpeed(-6);
            }
            else{

                hip->SetMotorSpeed(-1.5);
                if (R2D(hip->GetJointAngle()) < 10.0f){
                    isAscendingLeg = true;
                    changeLeg();
                }

            }
            break;
        }

        //

        /*
        //horizontal forces
        b2Vec2 vel = body->GetLinearVelocity();
        float desiredVel = 0;
        switch ( moveState )
        {
        case MS_LEFT:  desiredVel = -10; break;
        case MS_STAND:  desiredVel =  0; break;
        case MS_RIGHT: desiredVel =  10; break;
        }
        float velChange = desiredVel - vel.x;
        float force = body->GetMass() * velChange;
        if (isOnLadder) force *= 5;
        body->ApplyForce( b2Vec2(force,0), body->GetWorldCenter(), true );
*/
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
           // body->ApplyLinearImpulse(b2Vec2(-0.1, 0), body->GetWorldCenter(), true);
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
           // body->ApplyLinearImpulse(b2Vec2(-0.1, 0), b2Vec2(body->GetWorldCenter().x, body->GetWorldCenter().y - 0.2), true);
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
    body->SetFixedRotation(false);
    body->ApplyTorque(30000, true);
}

void Player::jump(){
    float jumpHeight;
    //if (body->IsFixedRotation())
    jumpHeight = 30;
    // else jumpHeight = 0;
    isOnLadder = false;
    if (!jumpCooldown){
        jumpCooldown = jumpCooldownMax;
        for (b2ContactEdge* ce = bodyParts.foot->body->GetContactList(); ce; ce = ce->next)
        {
            body->ApplyLinearImpulse(b2Vec2(0, body->GetMass() * jumpHeight), body->GetWorldCenter(), true);
            break;
            /*
            b2Contact* c = ce->contact;

            UserData* dataA = static_cast<UserData*>(c->GetFixtureA()->GetUserData());
            GameObject* objA = dataA->gameObject;
            BodyPart* bodyPartA = dynamic_cast <BodyPart*> (objA);
            if (bodyPartA){
                if ( bodyPartA->type == BodyPart::Type::FOOT) {
                    body->ApplyLinearImpulse(b2Vec2(0, body->GetMass()* jumpHeight), b2Vec2(0,0), true);
                    break;
                }
            }
            else{
                UserData* dataB = static_cast<UserData*>(c->GetFixtureB()->GetUserData());
                GameObject* objB = dataB->gameObject;
                BodyPart* bodyPartB = dynamic_cast <BodyPart*> (objB);
                if (bodyPartB)
                    if ( bodyPartB->type == BodyPart::Type::FOOT) {
                        body->ApplyLinearImpulse(b2Vec2(0, body->GetMass() * jumpHeight), b2Vec2(0,0), true);
                        break;
                    }
            }
            */
        }
    }
}

void Player::update(Textures* textures)
{

    if (!vehicle && !body->IsFixedRotation()){
        if (body->GetAngle() > 0.01)
            body->SetAngularVelocity (-3 - body->GetAngularVelocity());
        else
            if (body->GetAngle() < - 0.01)
                body->SetAngularVelocity (3 - body->GetAngularVelocity());
        if (fabs(body->GetAngle()) < 0.02)
            body->SetAngularVelocity(body->GetAngularVelocity() / 2.0f);

        //    if (body->GetAngle() < 0.01 && body->GetAngle() > - 0.01)
        //        body->SetFixedRotation(true);
    }
    if (jumpCooldown) --jumpCooldown;
    if (useCooldown) --useCooldown;
    if (isOnLadder && !checkForLadder()) isOnLadder = false;

    applyForce();
    chooseTexture(textures);
}
