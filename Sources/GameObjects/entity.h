#ifndef ENTITY_H
#define ENTITY_H
#include <Box2D/Box2D.h>
#include "Sources/GameObjects/gameobject.h"
#include "Sources/GameObjects/Characters/bodypart.h"
#include "Sources/GameObjects/InteractiveObjects/interactiveobject.h"
#include "Sources/GameObjects/InteractiveObjects/ladder.h"
#include "Sources/GameObjects/InteractiveObjects/Vehicles/vehicle.h"
#include "Sources/UserInterface/keylinedata.h"
#include "Sources/UserInterface/texturedata.h"
#include "Sources/userdata.h"
#include "Sources/GameObjects/Characters/bodyparts.h"
#include "enum.h"

class Entity : public GameObject
{
public:
    Entity();
    Entity(Textures* textures, b2World* world);


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
    void applyForce();
};

#endif // ENTITY_H
