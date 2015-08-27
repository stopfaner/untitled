#ifndef USERDATA_H
#define USERDATA_H

#include "Sources/UserInterface/displaydata.h"

class UserData
{
public:
    UserData();
    UserData (DisplayData* displayData);

    DisplayData* displayData;

    virtual void use();

};

#endif // USERDATA_H
