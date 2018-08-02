#ifndef TESSELATOR_H
#define TESSELATOR_H

#include "Point.h"
#include "BufferFactory.h"
#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>

struct Primitive
{
	GLenum mode;
    std::vector<Point> points;
};

class Tesselator
{
public:
	Tesselator();
	virtual ~Tesselator();
	
    void BeginPolygon(int numberOfPoints);
	void EndPolygon();
	
	void BeginContour();
	void EndContour();
	
	void AddVertex(const Point& point);
	
    std::vector<Primitive> Primitives();
	
private:
	static void BeginVA(GLenum mode, Tesselator* va);
	static void EndVA(Tesselator* va);
	static void VertexVA(void* p, Tesselator* va);
	static void EdgeFlagCallback(GLboolean flag, Tesselator* va);
	static void CombineCallback(GLdouble coords[3],GLdouble *vertex_data[4],GLfloat weight[4], GLdouble **dataOut, Tesselator* va);
	static void ErrorCallback(GLenum errorCode);
	
private:
	GLUtesselator* _tesselator;
    int _currentIndex;
    GLdouble* _points;
	
	BufferFactory _bufferFactory;
	
	Primitive _currentPrimitive;
    std::vector<Primitive> _primitives;
};

#endif
