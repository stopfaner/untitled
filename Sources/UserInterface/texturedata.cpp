#include "texturedata.h"

TextureData::TextureData() : DisplayData(){
    isMirrored = false;
    currentFrameN = 0;
    frameDelay = 0;
    frameDelayMax = 10;
}

TextureData::TextureData(Textures::Texture* texture_p) : TextureData(){
    setTexture(texture_p);
}

void TextureData::changeFrame (){
    if (frameDelay) --frameDelay;
    else
    {
        frameDelay = frameDelayMax;
        if (currentFrameN+1 < texture_p->framesQuantity)
            ++currentFrameN;
        else currentFrameN = 0;
    }
}

void TextureData::setTexture(Textures::Texture* texture_p){
    currentFrameN = 0;
    this->texture_p = texture_p;
    framesQuantity = texture_p->framesQuantity;
}
