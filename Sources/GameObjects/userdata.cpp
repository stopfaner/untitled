#include "userdata.h"
#include <QDebug>
UserData::UserData(){
    isMirrored = false;
    isPlayer = false;
    currentFrameN = 0;
    frameDelay = 0;
    frameDelayMax = 10;
}

UserData::UserData(Textures::Texture* texture_p) : UserData(){
    UserData();
    setTexture(texture_p);
}

void UserData::changeFrame (){
    if (frameDelay) --frameDelay;
    else
    {
        frameDelay = frameDelayMax;
        if (currentFrameN+1 < texture_p->framesQuantity)
            ++currentFrameN;
        else currentFrameN = 0;
    }
}

void UserData::setTexture(Textures::Texture* texture_p){
    currentFrameN = 0;
    this->texture_p = texture_p;
    framesQuantity = texture_p->framesQuantity;
}
