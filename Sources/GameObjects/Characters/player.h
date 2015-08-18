#ifndef PLAYER_H
#define PLAYER_H
#include <Box2D/Box2D.h>


class Player
{
public:
    Player();
    Player(int x, int y, b2World *world);
    void moveLeft();
    void moveRight();
    void jump();
    bool onGround();
    void updatePos();
private:
    b2Body *body;
    float x,y;
};

#endif // PLAYER_H
