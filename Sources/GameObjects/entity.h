#ifndef ENTITY_H
#define ENTITY_H

#include <Box2D/Box2D.h>

#include "Sources/GameObjects/gameobject.h"
#include "Sources/GameObjects/Characters/bodypart.h"
#include "Sources/GameObjects/Characters/bodyparts.h"
#include "Sources/GameObjects/InteractiveObjects/interactiveobject.h"
#include "Sources/GameObjects/InteractiveObjects/ladder.h"
#include "Sources/GameObjects/InteractiveObjects/Vehicles/vehicle.h"
#include "Sources/GameObjects/Sensors/groundsensor.h"
#include "Sources/UserInterface/keylinedata.h"
#include "Sources/UserInterface/texturedata.h"
#include "Sources/userdata.h"
#include "enum.h"
#include "entitylist.h"


class Entity : public GameObject
{
public:
    Entity(float x = 0, float y = 10);


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
    void update();
    void setBody (b2Body* body);
    b2Body *body;
    bool checkForLadder();
    void changeLeg();
    bool isGrounded();
    bool isGrounded(bool leftLeg);
    void constructBody();
private:
    int jumpCooldown, jumpCooldownMax;
    int useCooldown, useCooldownMax;
    float x, y;
    bool isSitting;
    bool isStanding;
    float surfaceAngle;
    void applyForce();
    virtual Textures::Type getTextureType(BodyPart::Type bodyPart) = 0;
};

#endif // ENTITY_H
