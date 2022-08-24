#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "DviInterpreter_global.h"

class DVIINTERPRETER_EXPORT Rectangle{
public:
    int height;
    int width;
    int h;
    int v;
    Rectangle(int height, int width, int h, int v) : height(height), width(width), h(h), v(v){}
};

#endif // RECTANGLE_H
