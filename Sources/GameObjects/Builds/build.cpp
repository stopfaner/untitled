#include "build.h"

Build::Build(Textures* texture_p, b2World* world)
{
   this->texture_p=texture_p;
   this->world=world;

   downDoorRooms = new FactoryDownDoorRoom;
   leftDoorRooms = new FactoryLeftDoorRoom;
   rightDoorRooms = new FactoryRightDoorRoom;
   upDoorRooms = new FactoryUpDoorRoom;
   leftAndRightDoorRooms = new FactoryLeftAndRightDoorRoom;
   upAndDownDoorRooms = new FactoryUpAndDownDoorRoom;

}



void Build::genBuild(b2Vec2 center)
{

}
