#ifndef BUILD_H
#define BUILD_H
#include <Box2D/Box2D.h>
#include "Sources/GameObjects/Rooms/room.h"
#include <vector>
#include <time.h>

class Build
{
private:
     enum TypeRoom {
        EMPTY,
        LEFT_DOOR,
        RIGHT_DOOR,
        LEFT_AND_RIGHT_DOOR,
        UP_DOOR,
        DOWN_DOOR,
        UP_AND_DOWN_DOOR
    };
    TypeRoom type;
    TypeRoom **planBuild;
    Room* room;
    FactoryDownDoorRoom* downDoorRooms;
    FactoryUpDoorRoom* upDoorRooms;
    FactoryRightDoorRoom* rightDoorRooms;
    FactoryLeftDoorRoom* leftDoorRooms;
    FactoryUpAndDownDoorRoom* upAndDownDoorRooms;
    FactoryLeftAndRightDoorRoom* leftAndRightDoorRooms;
    Textures* texture_p;
    b2World* world;
public:
    Build(Textures * texture_p, b2World *world);
    void generationDangeon(int maxNumberRoomInHeight, int maxNumberRoomInWidth);
    void paint(b2Vec2 center,int maxNumberRoomInHeight,int maxNumberRoomInWidth);

};

#endif // BUILD_H
