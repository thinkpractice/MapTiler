#ifndef RECT_H
#define RECT_H

class Rect
{
    public:
        Rect(int x, int y, int width, int height);

        int X() const;
        int Y() const;
        int Width() const;
        int Height() const;

    private:
        int _x, _y, _width, _height;

};

#endif

