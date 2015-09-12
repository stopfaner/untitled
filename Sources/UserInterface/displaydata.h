#ifndef DISPLAYDATA_H
#define DISPLAYDATA_H

#include "Sources/UserInterface/textures.h"

class DisplayData
{
public:
    enum Layer {
        MAX = 0x0100,
        BACKGROUND = 0x00FF,
        BACKGROUND_OBJECT = 0x00FE,
        LANDSCAPE = 0x00FD,
        BUILDING = 0x00FC,
        TEST = 0x00FB,
        PLAYER_FAR_FAR = 0x00E9,
        PLAYER_FAR = 0x00E8,
        PLAYER = 0x00E5,
        PLAYER_NEAR = 0x00E1,
        OBJECT = 0x0007,
        JOINT = 0x002,
        HUD = 0x001,
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
