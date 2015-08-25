#ifndef INTERACTIVEOBJECT_H
#define INTERACTIVEOBJECT_H
#include "Sources/GameObjects/noninteractive.h"

class Player;

class InteractiveObject : public NonInteractive{

public:
    InteractiveObject();
    InteractiveObject(Textures::Texture* texture_p);
    virtual void use (Player* player) = 0;
};

#endif // INTERACTIVEOBJECT_H
