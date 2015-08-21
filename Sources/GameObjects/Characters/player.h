#ifndef PLAYER_H
#define PLAYER_H
#include <Box2D/Box2D.h>
#include "Sources/GameObjects/userdata.h"

class MyContactListener;

class Player
{
public:
    enum MoveState{
        MS_STAND,
        MS_LEFT,
        MS_RIGHT
    };
    MoveState moveState;

    Player();

  //  void moveLeft();
 //   void moveRight();
    void jump();
    bool onGround();
    void allowJump();
    void applyForce();
    void updatePlayerJump();
    void setBody (b2Body* body);
    //MyContactListener *contactListener;
    b2Body *body;
    UserData *userData;
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
