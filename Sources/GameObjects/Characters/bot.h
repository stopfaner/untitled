#ifndef BOT_H
#define BOT_H
#include <Box2D/Box2D.h>
#include "player.h"
class AI;

class Bot
{
public:
    enum MoveState{
        MS_STAND,
        MS_LEFT,
        MS_RIGHT
    };
    MoveState moveState;

    Bot();
    Bot(int x, int y, b2World *world);

    void jump();
    bool onGround();
    void allowJump();
    void updateBotJump();
    void applyForce();
    b2Body *body;

private:
    bool canJump;
};

class AI {
public:
    AI();
    AI(Player *player, Bot *bot);
    updateAI();
    Bot *bot;
private:
    void horisontalControl();
    void verticalControl();
    b2Body *player;
};

#endif // BOT_H
