#include "player.h"
#include <QDebug>
Player::Player()
{
    canJump = true;
    moveState = MS_STAND;
    //contactListener = new MyContactListener;
}

void Player::allowJump(){
    canJump = 1;
}
/*
void Player::moveLeft(){
    if(body->GetLinearVelocity().x > -10)
       // body->ApplyForceToCenter(b2Vec2(-500,0),true);
    body->ApplyLinearImpulse(b2Vec2(-body->GetMass()*5,0), b2Vec2(0,0), true);
}

void Player::moveRight(){
    if(body->GetLinearVelocity().x < 10)
        body->ApplyForceToCenter(b2Vec2(500,0),true);
}
*/
void Player::setBody (b2Body* body){
    this->body = body;
}

void Player::applyForce(){
    b2Vec2 vel = body->GetLinearVelocity();
    float desiredVel = 0;
    switch ( moveState )
    {
    case MS_LEFT:  desiredVel = -5; break;
    case MS_STAND:  desiredVel =  0; break;
    case MS_RIGHT: desiredVel =  5; break;
    }
    float velChange = desiredVel - vel.x;
    float force = body->GetMass() * velChange; //disregard time factor
    body->ApplyForce( b2Vec2(force,0), body->GetWorldCenter(), true );
}

void Player::jump(){
    if(canJump && onGround())
    {
        //  if (contactListener->numFootContacts > 0)
        canJump = false;
        body->ApplyLinearImpulse(b2Vec2(0,body->GetMass()*10),b2Vec2(0,0),true);
    }
}

bool Player::onGround() {
    for (b2ContactEdge* ce = body->GetContactList(); ce; ce = ce->next)
    {
        b2Contact* c = ce->contact;
        if ((int) c->GetFixtureA()->GetUserData() == 3 || (int) c->GetFixtureB()->GetUserData() == 3) {
            return true;
        }
    }
    return false;

    /*
    for (b2ContactEdge* ce = body->GetContactList(); ce; ce = ce->next) {
        b2Contact* c = ce->contact;
        b2Body *otherBody = ce->other;
        b2Manifold *manifold = c->GetManifold();
        b2ManifoldPoint points[b2_maxManifoldPoints] = manifold->points;
        for(unsigned int i = 0; i < b2_maxManifoldPoints; i++) {
            float left = ((b2PolygonShape*)body->GetFixtureList()->GetShape())->GetVertex(0).x;
            float right = ((b2PolygonShape*)body->GetFixtureList()->GetShape())->GetVertex(0).x;
            for(unsigned int j = 1; j < 4; j++) {
                float temp = ((b2PolygonShape*)body->GetFixtureList()->GetShape())->GetVertex(j).x;
                if(temp < left)
                    left = temp;
                if(temp > right)
                    right = temp;
            }

            right += body->GetPosition().x;
            left += body->GetPosition().x;
            qDebug()<<left<<" "<<right<<" "<<points[i].localPoint.x + otherBody->GetPosition().x + 0.21;

            for(unsigned int j = 0; j < 4; j++) {
                if((points[i].localPoint.y + otherBody->GetPosition().y + 0.21) <
                        ((b2PolygonShape*)body->GetFixtureList()->GetShape())->GetVertex(j).y &&
                        (points[i].localPoint.x + otherBody->GetPosition().x + 0.21) < right &&
                        (points[i].localPoint.x + otherBody->GetPosition().x + 0.21) > left)
                    return true;
            }
        }
    }
    return false;
    */

}
