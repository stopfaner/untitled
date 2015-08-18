#include "player.h"
#include <QDebug>
Player::Player()
{

}

Player::Player(int x, int y, b2World *world) {
    b2BodyDef bodydef;
    bodydef.position.Set(x/20,y/20);
    bodydef.type = b2_dynamicBody;
    bodydef.fixedRotation = true;
    body = world->CreateBody(&bodydef);

    b2PolygonShape shape;
    shape.SetAsBox(1,2);

    b2FixtureDef fixturedef;
    fixturedef.friction = 5;
    fixturedef.shape = &shape;
    fixturedef.density = 1.0;

    body->CreateFixture(&fixturedef);
    b2PolygonShape polygonShape;
    polygonShape.SetAsBox(0.3, 0.3, b2Vec2(0,-2), 0);
    b2FixtureDef myFixtureDef;
    myFixtureDef.isSensor = true;
    b2Fixture* footSensorFixture = body->CreateFixture(&myFixtureDef);
    footSensorFixture->SetUserData( (void*)3 );
}

void Player::moveLeft(){
    if(body->GetLinearVelocity().x > -10)
        body->ApplyForceToCenter(b2Vec2(-500,0),true);
}

void Player::moveRight(){
    if(body->GetLinearVelocity().x < 10)
        body->ApplyForceToCenter(b2Vec2(500,0),true);
}

void Player::jump(){
    if(onGround())
        body->ApplyLinearImpulse(b2Vec2(0,105),b2Vec2(0,0),true);
}

bool Player::onGround() {
        return true;
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
