#ifndef BOT_H
#define BOT_H
#include <Box2D/Box2D.h>
#include "player.h"
#include "Sources/GameObjects/entity.h"
class AI;

class Bot : public Entity
{
public:
    enum MoveState{
        MS_STAND,
        MS_LEFT,
        MS_RIGHT
    };
    MoveState moveState;
    Bot(DisplayData* displayData);
    Bot(int x, int y, b2World *world, DisplayData* displayData);
    void jump();
    bool onGround();
    void allowJump();
    void updateBotJump();
    void applyForce();
    void setBody (b2Body* body);
    b2Body *body;
private:
    Textures::Type getTextureType(BodyPart::Type bodyPart);
    bool canJump;
};

#endif // BOT_H
