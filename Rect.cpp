#include "Rect.h"

Rect::Rect(double x, double y, double width, double height)
        : _x(x), _y(y), _width(width), _height(height)
{
}

double Rect::X()
{
    return _x;
}

double Rect::Y()
{
    return _y;
}

double Rect::Width()
{
    return _width;
}

double Rect::Height()
{
    return _height;
}

