#ifndef USERDATA_H
#define USERDATA_H

#include "GameObjects/gameobject.h"
#include "UserInterface/displaydata.h"

class UserData
{
public:
    UserData();
    UserData(DisplayData *displayData);
    UserData(GameObject* gameObject, DisplayData* displayData = nullptr);
    GameObject* gameObject;
    DisplayData* displayData;
};

#endif // USERDATA_H
