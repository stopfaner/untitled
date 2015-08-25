#include "color.h"

Color::Color()
{
    red = 1;
    green = 1;
    blue = 1;
    alpha = 1;
}

Color::Color(short red, short green, short blue, short alpha)
{
    setColor (red, green, blue, alpha);
}

Color::setColor(short red, short green, short blue, short alpha)
{
    this->red = (float) red / 255;
    this->green = (float) green / 255;
    this-> blue = (float) blue / 255;
    this-> alpha = (float) alpha / 255;
}
