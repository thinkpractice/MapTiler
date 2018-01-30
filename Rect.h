#ifndef RECT_H
#define RECT_H

class Rect
{
    public:
        Rect(double x, double y, double width, double height);

        double X();
        double Y();
        double Width();
        double Height();

    private:
        double _x, _y, _width, _height;

};

#endif

