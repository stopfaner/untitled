#include "player.h"

#include <qdebug.h>

Player::Player(DisplayData* displayData) : Entity ()
{
    moveState = MS_STAND;
    moveStateVertical = MSV_STAND;
    isOnLadder = false;
    isJumping = false;
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
        if (vehicle) vehicle = nullptr;
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
{
    TextureData* td = static_cast<TextureData*>(displayData);
    if (isOnLadder){
        if (moveStateVertical == MSV_STAND){
            if (td->texture_p->type != Textures::Type::CLIMBING_IDLE)
                td->setTexture(textures->getTexture(Textures::Type::CLIMBING_IDLE));
        }
        else
            if (td->texture_p->type != Textures::Type::CLIMBING)
                td->setTexture(textures->getTexture(Textures::Type::CLIMBING));
    }
    else{
        if (moveState == MS_LEFT)
            td->isMirrored = true;
        if (moveState == MS_RIGHT)
            td->isMirrored = false;
        if (moveState == MS_LEFT
                || moveState == MS_RIGHT){
            if (td->texture_p->type != Textures::Type::RUN)
                td->setTexture(textures->getTexture(Textures::Type::RUN));
        }
        else
            if (td->texture_p->type != Textures::Type::PLAYER)
                td->setTexture(textures->getTexture(Textures::Type::PLAYER));
    }

}
#define M_PI		3.14159265358979323846
void Player::applyForce(){
    if (vehicle){
        float desiredVel = 0;
        float desiredVelMax = 30 * 360 / 180 * M_PI;
        switch (moveState)
        {
        case MS_LEFT:  desiredVel = desiredVelMax; break;
        case MS_STAND:  desiredVel =  0; break;
        case MS_RIGHT: desiredVel =  -desiredVelMax; break;
        }
        //    float velChange = desiredVel - vehicle->motor->GetMotorSpeed();
        //    vehicle->motor->SetMotorSpeed(velChange);
        vehicle->motor->SetMotorSpeed(desiredVel);
    }
    else {
        if (isJumping)
            jump();

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

void Player::crouch(){}

void Player::jump(){
    isOnLadder = false;
    if (!jumpCooldown){
        jumpCooldown = jumpCooldownMax;
        for (b2ContactEdge* ce = body->GetContactList(); ce; ce = ce->next)
        {
            b2Contact* c = ce->contact;

            UserData* dataA = static_cast<UserData*>(c->GetFixtureA()->GetUserData());
            GameObject* objA = dataA->gameObject;
            BodyPart* bodyPartA = dynamic_cast <BodyPart*> (objA);
            if (bodyPartA){
                if ( bodyPartA->type == BodyPart::Type::FOOT_SENSOR) {
                    body->ApplyLinearImpulse(b2Vec2(0, body->GetMass()*10), b2Vec2(0,0), true);
                    break;
                }
            }
            else{
                UserData* dataB = static_cast<UserData*>(c->GetFixtureB()->GetUserData());
                GameObject* objB = dataB->gameObject;
                BodyPart* bodyPartB = dynamic_cast <BodyPart*> (objB);
                if (bodyPartB)
                    if ( bodyPartB->type == BodyPart::Type::FOOT_SENSOR) {
                        body->ApplyLinearImpulse(b2Vec2(0, body->GetMass()*10), b2Vec2(0,0), true);
                        break;
                    }
            }
        }
    }
}

void Player::update(Textures* textures)
{
    if (jumpCooldown) --jumpCooldown;
    if (useCooldown) --useCooldown;
    if (isOnLadder && !checkForLadder()) isOnLadder = false;
    if (vehicle && ((body->GetWorldCenter().x - vehicle->centerBody->GetWorldCenter().x > 5)
                    || (body->GetWorldCenter().y - vehicle->centerBody->GetWorldCenter().y > 5) )){
        vehicle = nullptr;
        qDebug()<<"left vehicle";
    }
    applyForce();
    chooseTexture(textures);
}
