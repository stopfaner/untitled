#ifndef ENTITY_H
#define ENTITY_H
#include "Sources/GameObjects/userdata.h"


class Entity : public UserData
{
public:
    Entity(DisplayData *displayData);
};

#endif // ENTITY_H
