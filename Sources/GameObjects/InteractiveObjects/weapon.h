#ifndef WEAPON_H
#define WEAPON_H
#include "interactiveobject.h"

class Weapon : public InteractiveObject
{
public:
    float damage;
    float density;
    Weapon();

};

#endif // WEAPON_H
