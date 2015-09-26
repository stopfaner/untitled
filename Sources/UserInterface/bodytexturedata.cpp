#include "bodytexturedata.h"


BodyTextureData::BodyTextureData(Textures::Texture *texture_p, Layer layer, float width, float height, b2Body *centerBody) : TextureData (texture_p, layer){
    this->width = width;
    this->height = height;
    this->centerBody = centerBody;
}

