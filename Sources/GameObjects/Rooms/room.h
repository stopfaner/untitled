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

    b2Body* room;
    b2Body* CreateRoom(b2Vec2 center, b2Vec2 size = b2Vec2(18, 8),
                       float wallWidth = 1, float pasageHeightLeft = 6, float passageHeightRight = 6);
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
