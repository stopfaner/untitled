#ifndef USERDATA_H
#define USERDATA_H
#include "Sources/UserInterface/textures.h"

class UserData
{
public:
    UserData();
    UserData(Textures::Type);

    Textures::Type textureType;
    bool isMirrored;
};

#endif // USERDATA_H
