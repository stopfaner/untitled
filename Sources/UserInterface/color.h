#ifndef COLOR_H
#define COLOR_H

class Color
{
public:
    float red;
    float green;
    float blue;
    float alpha;
    Color ();
    Color (short red, short green, short blue, short alpha = 255);
    setColor (short red, short green, short blue, short alpha = 255);
};

#endif // COLOR_H
