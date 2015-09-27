#ifndef BODYTEXTUREDATA_H
#define BODYTEXTUREDATA_H

#include "texturedata.h"

#include "Box2D/Box2D.h"
#include "Sources/Process/triangulation.h"

class BodyTextureData : public TextureData
{
public:
    BodyTextureData(Textures::Texture *texture_p, Layer layer, std::vector <Point*> &polyline, b2Body *body);
    float width, height;
    b2Body *centerBody;
};

#endif // BODYTEXTUREDATA_H
