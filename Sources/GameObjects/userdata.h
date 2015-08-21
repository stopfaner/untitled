#ifndef USERDATA_H
#define USERDATA_H
#include "Sources/UserInterface/textures.h"

class UserData
{
public:
    Textures::Type textureType;
    UserData();
    UserData(Textures::Type);

};

#endif // USERDATA_H
