#ifndef ROOM_H
#define ROOM_H
#include "Sources/GameObjects/noninteractive.h"
#include <Box2D/Box2D.h>
class Room{
public:
    bool hasNPC;
    bool hasChest;
    bool hasEnemys;
    bool hasLight;
    bool hasLadder;
    float wallWidth=1;
    b2Vec2 size = b2Vec2(18, 10);
    float passageHeightLeft = 5;
    float passageHeightRight = 5;
    bool isUpWall=true;
    bool isDownWall=true;

    b2Body* room;
    b2Body* CreateRoom(b2Vec2 center);
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

class LeftAndRightDoorRoom : public Room{
public:
    LeftAndRightDoorRoom(b2Vec2 center, Textures* texture_p, b2World* world) : Room (texture_p,world)
    {
        CreateRoom(center);
    }
};

class RightDoorRoom : public Room{
public:
    RightDoorRoom(b2Vec2 center,Textures * texture_p, b2World *world) : Room (texture_p,world)
    {
        this->passageHeightLeft=0;
        CreateRoom(center);
    }
};

class LeftDoorRoom : public Room{
public:
    LeftDoorRoom(b2Vec2 center,Textures * texture_p, b2World *world) : Room (texture_p,world)
    {
        this->passageHeightRight=0;
        CreateRoom(center);
    }
};

class UpDoorRoom : public Room{
public:
    UpDoorRoom(b2Vec2 center,Textures * texture_p, b2World *world) : Room (texture_p,world)
    {

        this->passageHeightLeft=size.y-2*wallWidth;
        this->passageHeightRight=size.y-2*wallWidth;
        this->size.x=6;
        this->isUpWall=false;
        CreateRoom(center);
        this->size.x=6;
        this->isDownWall=true;
        this->isUpWall=true;
        CreateRoom(b2Vec2(center.x-5, center.y));
        this->size.x=6;
        this->isDownWall=true;
        this->isUpWall=true;
        CreateRoom(b2Vec2(center.x+5, center.y));
    }
};

class DownDoorRoom : public Room{
public:
    DownDoorRoom(b2Vec2 center,Textures * texture_p, b2World *world) : Room (texture_p,world)
    {
        this->passageHeightLeft=size.y-2*wallWidth;
        this->passageHeightRight=size.y-2*wallWidth;
        this->size.x=6;
        this->isDownWall=false;
        CreateRoom(center);
        this->size.x=6;
        this->isDownWall=true;
        this->isUpWall=true;
        CreateRoom(b2Vec2(center.x-5, center.y));
        this->size.x=6;
        this->isDownWall=true;
        this->isUpWall=true;
        CreateRoom(b2Vec2(center.x+5, center.y));
    }
};

class UpAndDownDoorRoom : public Room{
public:
    UpAndDownDoorRoom(b2Vec2 center,Textures * texture_p, b2World *world) : Room (texture_p,world)
    {
        this->passageHeightLeft=size.y-2*wallWidth;
        this->passageHeightRight=size.y-2*wallWidth;
        this->size.x=6;
        this->isDownWall=false;
        this->isUpWall=false;
        CreateRoom(center);
        this->size.x=6;
        this->isDownWall=true;
        this->isUpWall=true;
        CreateRoom(b2Vec2(center.x-5, center.y));
        this->size.x=6;
        this->isDownWall=true;
        this->isUpWall=true;
        CreateRoom(b2Vec2(center.x+5, center.y));

    }
};

// factory room
class FactoryRoom
{
public:
    virtual Room *createObject(b2Vec2 center, Textures * texture_p, b2World *world) = 0;
    virtual ~FactoryRoom() {}
};

class FactoryLeftAndRightDoorRoom : FactoryRoom
{
public:
    Room *createObject(b2Vec2 center, Textures* texture_p, b2World *world)
    {
        return new LeftAndRightDoorRoom(center, texture_p, world);
    }
};

class FactoryRightDoorRoom : FactoryRoom
{
public:
    Room *createObject(b2Vec2 center, Textures* texture_p, b2World *world)
    {
        return new RightDoorRoom(center, texture_p, world);
    }
};

class FactoryLeftDoorRoom : FactoryRoom
{
public:
    Room *createObject(b2Vec2 center, Textures* texture_p, b2World *world)
    {
        return new LeftDoorRoom(center, texture_p,world);
    }
};

class FactoryUpDoorRoom : FactoryRoom
{
public:
    Room *createObject(b2Vec2 center, Textures* texture_p, b2World *world)
    {
        return new UpDoorRoom(center, texture_p,world);
    }
};

class FactoryDownDoorRoom : FactoryRoom
{
public:
    Room *createObject(b2Vec2 center, Textures* texture_p, b2World *world)
    {
        return new DownDoorRoom(center, texture_p,world);
    }
};

class FactoryUpAndDownDoorRoom : FactoryRoom
{
public:
    Room *createObject(b2Vec2 center, Textures* texture_p, b2World *world)
    {
        return new UpAndDownDoorRoom(center, texture_p,world);
    }
};

#endif // ROOM_H
