#ifndef DISPLAYDATA_H
#define DISPLAYDATA_H

#include "Sources/UserInterface/textures.h"

class DisplayData
{
public:
    enum Layer {
        BACKGROUND = 0x00F0,
        BACKGROUND_OBJECT = 0x00E0,
        LANDSCAPE = 0x00D0,
        PLAYER_FAR = 0x00E9,
        PLAYER = 0x00E5,
        PLAYER_NEAR = 0x00E1,
        OBJECT = 0x0007,
        JOINT = 0x002,
        HUD = 0x001,
        MAX = 0x0100,
        NEAREAST = 0x000
    };
    DisplayData();
    DisplayData(Layer layer);
    bool isShifting;
    Layer layer;

private:
    virtual void virtualMethod();
};

#endif // DISPLAYDATA_H
