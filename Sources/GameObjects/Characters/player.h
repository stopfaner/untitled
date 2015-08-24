#ifndef PLAYER_H
#define PLAYER_H
#include <Box2D/Box2D.h>

#include "Sources/GameObjects/Characters/bodypart.h"
#include "Sources/GameObjects/entity.h"
#include "Sources/GameObjects/InteractiveObjects/interactiveobject.h"
#include "Sources/GameObjects/InteractiveObjects/ladder.h"
//class Ladder;

class Player : public Entity
{

public:

    enum MoveState{
        MS_STAND,
        MS_LEFT,
        MS_RIGHT,
    };
    enum MoveStateVertical{
        MSV_STAND,
        MSV_UP,
        MSV_DOWN,
    };
    bool isOnLadder;
    bool isJumping;
    MoveState moveState;
    MoveStateVertical moveStateVertical;
    Player(Textures::Texture* texture_p);
    void useObject();
    void jump();
    void crouch();
    void update(Textures* textures);
    void setBody (b2Body* body);
    b2Body *body;
    int jumpCooldown;
    int jumpCooldownMax;
    bool checkForLadder();
private:
    void chooseTexture(Textures *textures);
    void applyForce();
};

#endif // PLAYER_H
