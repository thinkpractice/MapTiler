#ifndef TESSELATOR_H
#define TESSELATOR_H

#include "Point.h"
#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>

using namespace std;

struct Primitive
{
	GLenum mode;
	vector<GLdouble> indices;
};

struct VA
{
	Primitive currentPrimitive;
	vector<Point> points;
	vector<Primitive> primitives;
};

class Tesselator
{
public:
	Tesselator();
	virtual ~Tesselator();
	
	void BeginPolygon();
	void EndPolygon();
	
	void BeginContour();
	void EndContour();
	
	void AddVertex(const Point& point);
	
	vector<Point> Points();
	
private:
	static void BeginVA(GLenum mode, VA* va);
	static void EndVA(VA* va);
	static void VertexVA(void* p, VA* va);
	static void EdgeFlagCallback(GLboolean flag, VA* va);
	static void CombineCallback(GLdouble coords[3],GLdouble *vertex_data[4],GLfloat weight[4], GLdouble **dataOut, VA* va);
	static void ErrorCallback(GLenum errorCode);
	
private:
	GLUtesselator* _tesselator;
	VA _va;	
	vector<GLdouble[3]> _points;	
};

#endif