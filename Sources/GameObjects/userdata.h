#ifndef USERDATA_H
#define USERDATA_H
#include "Sources/UserInterface/textures.h"

class UserData
{
public:
    UserData();
    UserData(Textures::Texture* texture_p);

    Textures::Texture* texture_p;
    bool hasTexture;
    bool isMirrored;
    bool isPlayer;
    int currentFrameN;
    int framesQuantity;
    int frameDelay;
    int frameDelayMax;
    void changeFrame ();
    void setTexture (Textures::Texture *texture_p);


};

#endif // USERDATA_H
