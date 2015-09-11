#ifndef BUILD_H
#define BUILD_H

#include <Box2D/Box2D.h>
#include <vector>
#include <time.h>

#include "Sources/GameObjects/Rooms/room.h"
#include "Sources/generalinfo.h"

class Build
{
public:
    Build();
    void generateDungeon(b2Vec2 center,int maxNumberRoomInHeight,int maxNumberRoomInWidth);
private:
    void generateMap(int maxNumberRoomInHeight, int maxNumberRoomInWidth);
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
    Textures* textures;
    b2World* world;

};

#endif // BUILD_H
