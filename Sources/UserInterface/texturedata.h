#ifndef TEXTUREDATA_H
#define TEXTUREDATA_H
#include "displaydata.h"


class TextureData : public DisplayData
{
public:
     TextureData(Textures::Texture *texture_p);

     bool isMirrored;

     int currentFrameN;
     int framesQuantity;
     int frameDelay;
     int frameDelayMax;

     Textures::Texture* texture_p;

     void changeFrame ();
     void setTexture (Textures::Texture *texture_p);
private:
     TextureData();
};

#endif // TEXTUREDATA_H
