#include "Rect.h"

Rect::Rect(int x, int y, int width, int height)
        : _x(x), _y(y), _width(width), _height(height)
{
}

int Rect::Left() const
{
    return _x;
}

int Rect::Top() const
{
    return _y;
}

int Rect::Width() const
{
    return _width;
}

void Rect::SetWidth(int width)
{
    _width = width;
}

int Rect::Height() const
{
    return _height;
}

void Rect::SetHeight(int height)
{
    _height = height;
}

int Rect::Right() const
{
    return Left() + Width();
}

int Rect::Bottom() const
{
    return Top() + Height();
}

Point Rect::LeftTop() const
{
    return Point(Left(), Top());
}

Point Rect::RightBottom() const
{
    return Point(Right(), Bottom());
}
