#include "player.h"
#include <QDebug>
Player::Player(Textures::Texture* texture_p) : Entity (texture_p)
{
    moveState = MS_STAND;
    jumpCooldown = 0;
    jumpCooldownMax = 30;
}

void Player::setBody (b2Body* body){
    this->body = body;
}

void Player::applyForce(){
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
    body->ApplyForce( b2Vec2(force,0), body->GetWorldCenter(), true );
}

void Player::jump(){
    if (!jumpCooldown){
        jumpCooldown = jumpCooldownMax;
        for (b2ContactEdge* ce = body->GetContactList(); ce; ce = ce->next)
        {
            b2Contact* c = ce->contact;
            if (((int) c->GetFixtureA()->GetUserData() == 3) || ((int) c->GetFixtureB()->GetUserData() == 3)) {
                body->ApplyLinearImpulse(b2Vec2(0, body->GetMass()*10), b2Vec2(0,0), true);
                break;
            }
        }
    }
}
