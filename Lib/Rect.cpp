#include "Rect.h"
#include <cmath>

using namespace std;

Rect::Rect()
        :   Rect(0.0,0.0,0.0,0.0)
{
}

Rect::Rect(double x, double y, double width, double height)
        : _x(x), _y(y), _width(width), _height(height)
{
}

Rect::Rect(Point leftTop, Point bottomRight)
        : Rect( leftTop.X,
                leftTop.Y,
                abs(bottomRight.X - leftTop.X),
                abs(bottomRight.Y - leftTop.Y)
              )
{
}

double Rect::Left() const
{
    return _x;
}

double Rect::Top() const
{
    return _y;
}

double Rect::Width() const
{
    return _width;
}

void Rect::SetWidth(double width)
{
    _width = width;
}

double Rect::Height() const
{
    return _height;
}

void Rect::SetHeight(double height)
{
    _height = height;
}

int Rect::IntegerWidth() const
{
    return static_cast<int>(Width());
}

int Rect::IntegerHeight() const
{
    return static_cast<int>(Height());
}

double Rect::Right() const
{
    return Left() + Width();
}

double Rect::Bottom() const
{
    return Top() + Height();
}

Point Rect::LeftTop() const
{
    return Point(Left(), Top());
}

Point Rect::BottomRight() const
{
    return Point(Right(), Bottom());
}
