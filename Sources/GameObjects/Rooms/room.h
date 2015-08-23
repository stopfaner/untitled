#ifndef ROOM_H
#define ROOM_H
#include "Sources/GameObjects/noninteractive.h"
#include <Box2D/Box2D.h>
class Room
{
public:
    bool hasNPC;
    bool hasChest;
    bool hasEnemys;
    bool hasLight;

    class Point{
    public:
        float x;
        float y;
        Point(float x,float y){
            this->x = x;
            this->y = y;
        }

    };
    b2Body* room;
    b2Body* CreateRoom(float x,float y);
    enum TypeRoom {
        LEFT_DOOR,
        RIGHT_DOOR,
        LEFT_RIGHT_DOOR,
        UP_DOOR,
        DOWN_DOOR,
        UP_DOWN_DOOR
    };
    TypeRoom type;
    Textures* texture_p;
    b2World *world;
    Room(Textures * texture_p, b2World *world);
};


#endif // ROOM_H
