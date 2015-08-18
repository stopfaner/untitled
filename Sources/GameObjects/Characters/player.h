#ifndef PLAYER_H
#define PLAYER_H
#include <Box2D/Box2D.h>

class MyContactListener;



class Player
{
public:
    Player();
    Player(int x, int y, b2World *world);

    void moveLeft();
    void moveRight();
    void jump();
    bool onGround();
    void allowJump();
    //MyContactListener *contactListener;
    b2Body *body;

private:
    bool canJump;
};
/*
class MyContactListener : public b2ContactListener
{
public:
    void BeginContact(b2Contact* contact) {
        //check if fixture A was the foot sensor
        void* fixtureUserData = contact->GetFixtureA()->GetUserData();
        if ( (int)fixtureUserData == 3 )
            numFootContacts++;
        //check if fixture B was the foot sensor
        fixtureUserData = contact->GetFixtureB()->GetUserData();
        if ( (int)fixtureUserData == 3 )
            numFootContacts++;
    }
    void EndContact(b2Contact* contact) {
        //check if fixture A was the foot sensor
        void* fixtureUserData = contact->GetFixtureA()->GetUserData();
        if ( (int)fixtureUserData == 3 )
            numFootContacts--;
        //check if fixture B was the foot sensor
        fixtureUserData = contact->GetFixtureB()->GetUserData();
        if ( (int)fixtureUserData == 3 )
            numFootContacts--;
    }

    int numFootContacts = 0;
};
*/
#endif // PLAYER_H
