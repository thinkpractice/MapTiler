#ifndef POINT_H
#define POINT_H

#include <iostream>

using namespace std;

struct Point
{
    double X;
    double Y;
	double Z;
	double data[3];
	
    Point()
        : Point(0.0, 0.0, 0.0)
    {
    }

    Point (double x, double y, double z = 0.0)
        : X(x), Y(y), Z(z)
    {
    }
    
    double* Data()
	{
		data[0] = X;
		data[1] = Y;
		data[2] = Z;
		return data;
	}
};

inline std::ostream& operator<<(std::ostream &strm, const Point &p) 
{
    return strm << "Point(" << p.X << "," << p.Y << "," << p.Z << ")";
}

#endif
