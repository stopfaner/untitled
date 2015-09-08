#ifndef DISPLAYDATA_H
#define DISPLAYDATA_H

#include "Sources/UserInterface/textures.h"

class DisplayData
{
public:
    enum Layer {
        BACKGROUND = 0x00F0,
        BACKGROUND_OBJECT = 0x0010,
        LANDSCAPE = 0x0009,
        PLAYER = 0x0008,
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
