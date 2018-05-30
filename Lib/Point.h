#ifndef POINT_H
#define POINT_H

#include <iostream>

using namespace std;

struct Point
{
    double X;
    double Y;

    Point()
        : Point(0.0, 0.0)
    {
    }

    Point (double x, double y)
        : X(x), Y(y)
    {
    }
};

inline std::ostream& operator<<(std::ostream &strm, const Point &p) 
{
    return strm << "Point(" << p.X << "," << p.Y << ")";
}

#endif
