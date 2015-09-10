#ifndef PLAYER_H
#define PLAYER_H

#include <Box2D/Box2D.h>

#include "Sources/GameObjects/Characters/bodypart.h"
#include "Sources/GameObjects/entity.h"
#include "Sources/GameObjects/InteractiveObjects/interactiveobject.h"
#include "Sources/GameObjects/InteractiveObjects/ladder.h"
#include "Sources/GameObjects/InteractiveObjects/Vehicles/vehicle.h"
#include "Sources/UserInterface/texturedata.h"
#include "Sources/userdata.h"
#include "bodyparts.h"
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
    bool isUsingLeftLeg;
    bool isAscendingLeg;
    bool isRightDirection;
    MoveState moveState;
    MoveStateVertical moveStateVertical;
    Player(DisplayData *displayData);
    Vehicle* vehicle;
    DisplayData* displayData;
    BodyParts bodyParts;

    void rotate (bool right);
    void move ();
    void useObject();
    void jump();
    void crouch();
    void fall();
    void update(Textures* textures);
    void setBody (b2Body* body);
    b2Body *body;
    bool checkForLadder();
    void changeLeg();
    bool isGrounded();
private:
    int jumpCooldown, jumpCooldownMax;
    int useCooldown, useCooldownMax;
    void chooseTexture(Textures *textures);
    void applyForce();
};

#endif // PLAYER_H
