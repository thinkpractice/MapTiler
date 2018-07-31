#ifndef RECT_H
#define RECT_H

#include "Point.h"

class Rect
{
    public:
        Rect();
        Rect(double x, double y, double width, double height);
        Rect(Point leftTop, Point bottomRight);

        double Left() const;
        double Top() const;
        double Right() const;
        double Bottom() const;

        double Width() const;
        void SetWidth(double width);

        double Height() const;
        void SetHeight(double height);

        int IntegerWidth() const;
        int IntegerHeight() const;

        Point LeftTop() const;
        Point BottomRight() const;

    private:
        double _x, _y, _width, _height;

};

#endif

