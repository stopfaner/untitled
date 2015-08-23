#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H
#include "Sources/GameObjects/noninteractive.h"

class DynamicObject : public NonInteractive
{
public:
    DynamicObject(Textures::Texture* texture_p);
};

#endif // DYNAMICOBJECT_H
