#ifndef POINT_H
#define POINT_H

struct Point
{
    double X;
    double Y;

    Point (double x, double y)
        : X(x), Y(y)
    {
    }
};

#endif
