#ifndef PLAYER_H
#define PLAYER_H
#include <Box2D/Box2D.h>
#include "Sources/GameObjects/entity.h"

class MyContactListener;

class Player :  public Entity
{
public:
    enum MoveState{
        MS_STAND,
        MS_LEFT,
        MS_RIGHT
    };
    MoveState moveState;
    Player(Textures::Texture* texture_p);
    void jump();
    void applyForce();
    void setBody (b2Body* body);
    b2Body *body;
    int jumpCooldown;
    int jumpCooldownMax;
};

#endif // PLAYER_H
