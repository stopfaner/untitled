#include "Bot.h"
#include <QDebug>

Bot::Bot()
{
    canJump = true;
    moveState = MS_STAND;
}

void Bot::setBody (b2Body* body){
    this->body = body;
}

Bot::Bot(int x, int y, b2World *world) : Bot() {
    Bot();
    b2BodyDef bodydef;
    bodydef.position.Set(x/20,y/20);
    bodydef.type = b2_dynamicBody;
    bodydef.fixedRotation = true;
    body = world->CreateBody(&bodydef);

    b2PolygonShape shape;
    shape.SetAsBox(1,2);

    b2FixtureDef fixturedef;
    fixturedef.shape = &shape;
    fixturedef.density = 1.0;

    b2Fixture* mainFixture = body->CreateFixture(&fixturedef);
    mainFixture->SetUserData( (void*)1 );
    b2PolygonShape polygonShape;
    polygonShape.SetAsBox(0.3, 0.3, b2Vec2(0,-2), 0);
    fixturedef.isSensor = true;
    b2Fixture* footSensorFixture = body->CreateFixture(&fixturedef);
    footSensorFixture->SetUserData( (void*)3 );
}

void Bot::allowJump(){
    canJump = true;
}

void Bot::updateBotJump() {
    if (body->GetLinearVelocity().y > -0.02 && body->GetLinearVelocity().y < 0.02)
        allowJump();
}

void Bot::applyForce(){
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

void Bot::jump(){
    if(canJump && onGround()) {
        canJump = false;
        body->ApplyLinearImpulse(b2Vec2(0,body->GetMass()*10),b2Vec2(0,0),true);
    }
}

bool Bot::onGround() {
    for (b2ContactEdge* ce = body->GetContactList(); ce; ce = ce->next) {
        b2Contact* c = ce->contact;
        if ((int) c->GetFixtureA()->GetUserData() == 3 || (int) c->GetFixtureB()->GetUserData() == 3) {
            return true;
        }
    }
    return false;
}






AI::AI() {

}

AI::AI(Player *player, Bot *bot) {
    this->player = player->body;
    this->bot = bot;
}

AI::updateAI() {
    horisontalControl();
    verticalControl();
}

void AI::horisontalControl() {
    if(player->GetPosition().x < bot->body->GetPosition().x)
        bot->moveState = Bot::MS_LEFT;
    else if(player->GetPosition().x > bot->body->GetPosition().x)
            bot->moveState = Bot::MS_RIGHT;
    else bot->moveState = Bot::MS_STAND;
}

void AI::verticalControl() {
    if(bot->moveState && bot->body->GetLinearVelocity().x > -0.01 && bot->body->GetLinearVelocity().x < 0.01)
        bot->jump();
}
