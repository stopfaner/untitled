#ifndef TRIANGLETEXTUREDATA_H
#define TRIANGLETEXTUREDATA_H

#include "texturedata.h"

class TriangleTextureData : public TextureData
{
public:
    TriangleTextureData(Layer layer);
    TriangleTextureData(Textures::Texture *texture_p, Layer layer);
};

#endif // TRIANGLETEXTUREDATA_H
