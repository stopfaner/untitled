#ifndef KEYLINEDATA_H
#define KEYLINEDATA_H

#include "displaydata.h"
#include "color.h"

class KeyLineData : public DisplayData
{
public:
    KeyLineData(Color color);
    Color color;
};

#endif // KEYLINEDATA_H
