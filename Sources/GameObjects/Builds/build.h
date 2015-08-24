#ifndef BUILD_H
#define BUILD_H
#include <Box2D/Box2D.h>
#include "Sources/GameObjects/Rooms/room.h"
#include <vector>
class Build
{
private:
    int maxNumberRoomInHeight;
    int maxNumberRoomInWigth;

    FactoryDownDoorRoom* downDoorRooms;
    FactoryUpDoorRoom* upDoorRooms;
    FactoryRightDoorRoom* rightDoorRooms;
    FactoryLeftDoorRoom* leftDoorRooms;
    FactoryUpAndDownDoorRoom* upAndDownDoorRooms;
    FactoryLeftAndRightDoorRoom* leftAndRightDoorRooms;

    Textures* texture_p;
    b2World* world;
    Build(Textures * texture_p, b2World *world);
    void genBuild(b2Vec2 center);

};

#endif // BUILD_H
