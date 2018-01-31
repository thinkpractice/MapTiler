#include "Rect.h"

Rect::Rect(int x, int y, int width, int height)
        : _x(x), _y(y), _width(width), _height(height)
{
}

int Rect::X()
{
    return _x;
}

int Rect::Y()
{
    return _y;
}

int Rect::Width()
{
    return _width;
}

int Rect::Height()
{
    return _height;
}

