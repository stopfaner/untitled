#ifndef DISPLAYDATA_H
#define DISPLAYDATA_H

#include "Sources/UserInterface/textures.h"

class DisplayData
{
public:
    DisplayData();
    bool isPlayer;
private:
    virtual void virtualMethod();
};

class NonDrawable : public DisplayData{};

#endif // DISPLAYDATA_H
