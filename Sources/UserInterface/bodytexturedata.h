#ifndef BODYTEXTUREDATA_H
#define BODYTEXTUREDATA_H


#include "texturedata.h"

class b2Body;

class BodyTextureData : public TextureData
{
public:
    BodyTextureData(Textures::Texture *texture_p, Layer layer, float width, float height, b2Body* centerBody);
    float width, height;
    b2Body *centerBody;
};

#endif // BODYTEXTUREDATA_H
