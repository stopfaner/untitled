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
    Bot(Textures::Texture* texture_p);
    Bot(int x, int y, b2World *world, Textures::Texture* texture_p);
    void jump();
    bool onGround();
    void allowJump();
    void updateBotJump();
    void applyForce();
    void setBody (b2Body* body);
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
