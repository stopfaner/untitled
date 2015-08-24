#include "player.h"

Player::Player(Textures::Texture* texture_p) : Entity (texture_p)
{
    moveState = MS_STAND;
    moveStateVertical = MSV_STAND;
    isOnLadder = false;
    jumpCooldown = 0;
    jumpCooldownMax = 30;
}

void Player::useObject(){
    if (isOnLadder) isOnLadder = false;
    else
        for (b2ContactEdge* ce = body->GetContactList(); ce; ce = ce->next)
        {
            b2Contact* c = ce->contact;
            UserData* dataA = (UserData*)(c->GetFixtureA()->GetUserData());
            InteractiveObject* objA = dynamic_cast<InteractiveObject*> (dataA);
            if (objA){
                objA->use(this);
            }
            UserData* dataB = (UserData*)(c->GetFixtureB()->GetUserData());
            InteractiveObject* objB = dynamic_cast<InteractiveObject*> (dataB);
            if (objB){
                objB->use(this);
            }
        }
}

void Player::checkForLadder(){
    bool isLadder = false;
    for (b2ContactEdge* ce = body->GetContactList(); ce; ce = ce->next)
    {
        b2Contact* c = ce->contact;
        UserData* dataA = (UserData*)(c->GetFixtureA()->GetUserData());
        Ladder* objA = dynamic_cast<Ladder*> (dataA);
        UserData* dataB = (UserData*)(c->GetFixtureB()->GetUserData());
        Ladder* objB = dynamic_cast<Ladder*> (dataB);
        if (objA || objB){
            isLadder = true;
            break;
        }
    }
    if (!isLadder) isOnLadder = false;
}

void Player::setBody (b2Body* body){
    this->body = body;
}

void Player::chooseTexture(Textures *textures)
{
    if (isOnLadder){
        if (texture_p->type != Textures::Type::CLIMBING)
            setTexture(textures->getTexture(Textures::Type::CLIMBING));
    }
    else{
        if (moveState == MS_LEFT)
            isMirrored = true;
        if (moveState == MS_RIGHT)
            isMirrored = false;
        if (moveState == MS_LEFT
                || moveState == MS_RIGHT){
            if (texture_p->type != Textures::Type::RUN)
                setTexture(textures->getTexture(Textures::Type::RUN));
        }
        else
            if (texture_p->type != Textures::Type::PLAYER)
                setTexture(textures->getTexture(Textures::Type::PLAYER));
    }

}

void Player::applyForce(){
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
            jump();
        if (moveStateVertical == MSV_DOWN)
            crouch();
    }

}

void Player::crouch(){}

void Player::jump(){
    if (!jumpCooldown){
        jumpCooldown = jumpCooldownMax;
        for (b2ContactEdge* ce = body->GetContactList(); ce; ce = ce->next)
        {
            b2Contact* c = ce->contact;
            UserData* dataA = (UserData*) c->GetFixtureA()->GetUserData();
            BodyPart* bodyPartA = dynamic_cast <BodyPart*> (dataA);
            if (bodyPartA)
                if ( bodyPartA->type == BodyPart::Type::FootSensor) {
                    body->ApplyLinearImpulse(b2Vec2(0, body->GetMass()*10), b2Vec2(0,0), true);
                    break;
                }
            UserData* dataB = (UserData*) c->GetFixtureB()->GetUserData();
            BodyPart* bodyPartB = dynamic_cast <BodyPart*> (dataB);
            if (bodyPartB)
                if ( bodyPartB->type == BodyPart::Type::FootSensor) {
                    body->ApplyLinearImpulse(b2Vec2(0, body->GetMass()*10), b2Vec2(0,0), true);
                    break;
                }
        }
    }
}

void Player::update(Textures* textures)
{
    if (isOnLadder) checkForLadder();
    applyForce();
    chooseTexture(textures);
}
