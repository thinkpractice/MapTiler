#ifndef RECT_H
#define RECT_H

#include "Point.h"

class Rect
{
    public:
        Rect(int x, int y, int width, int height);

        int Left() const;
        int Top() const;
        int Right() const;
        int Bottom() const;

        int Width() const;
        void SetWidth(int width);

        int Height() const;
        void SetHeight(int height);

        Point LeftTop() const;
        Point RightBottom() const;

    private:
        int _x, _y, _width, _height;

};

#endif

