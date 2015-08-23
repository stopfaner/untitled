#ifndef INTERACTIVEOBJECT_H
#define INTERACTIVEOBJECT_H
#include "Sources/GameObjects/noninteractive.h"

class InteractiveObject : public NonInteractive
{
public:
    InteractiveObject(Textures::Texture* texture_p);
};

#endif // INTERACTIVEOBJECT_H
