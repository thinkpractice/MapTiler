#include "Rect.h"

Rect::Rect(int x, int y, int width, int height)
        : _x(x), _y(y), _width(width), _height(height)
{
}

int Rect::X() const
{
    return _x;
}

int Rect::Y() const
{
    return _y;
}

int Rect::Width() const
{
    return _width;
}

int Rect::Height() const
{
    return _height;
}

int Rect::Right() const
{
    return X() + Width();
}

int Rect::Bottom() const
{
    return Y() + Height();
}
