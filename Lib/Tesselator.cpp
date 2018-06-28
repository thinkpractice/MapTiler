#include "Tesselator.h"
#include <iostream>

Tesselator::Tesselator()
{
	_tesselator = gluNewTess(); // create a _tesselatorellator
	
	gluTessProperty(_tesselator, GLU_TESS_BOUNDARY_ONLY, GL_FALSE);
	gluTessProperty(_tesselator, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO);
	gluTessCallback(_tesselator, GLU_TESS_BEGIN_DATA, (GLvoid (*)())Tesselator::BeginVA);
	gluTessCallback(_tesselator, GLU_TESS_END_DATA, (GLvoid (*)())Tesselator::EndVA);
	gluTessCallback(_tesselator, GLU_TESS_VERTEX_DATA, (GLvoid (*)())Tesselator::VertexVA);
	//Causes rendering artefacts!
	gluTessCallback(_tesselator, GLU_TESS_EDGE_FLAG_DATA, (GLvoid(*)())Tesselator::EdgeFlagCallback);
	gluTessCallback(_tesselator, GLU_TESS_COMBINE_DATA, (GLvoid (*)())Tesselator::CombineCallback);
	gluTessCallback(_tesselator, GLU_TESS_ERROR, (GLvoid (*)())Tesselator::ErrorCallback);
}

Tesselator::~Tesselator()
{
	gluDeleteTess(_tesselator);
}

void Tesselator::BeginPolygon()
{
	gluTessBeginPolygon(_tesselator, &_va);
}

void Tesselator::EndPolygon()
{
	_va = VA();
	_points.clear();
	gluTessEndPolygon(_tesselator);
}

void Tesselator::BeginContour()
{
	gluTessBeginContour(_tesselator);
}

void Tesselator::EndContour()
{
	gluTessEndContour(_tesselator);
}

void Tesselator::AddVertex(const Point& point)
{	
	GLdouble p[3];
	p[0] = point.X;
	p[1] = point.Y;
	p[2] = point.Z;
	_points.push_back(p);
	
	size_t i = _points.size() - 1;
	gluTessVertex(_tesselator, p, p);
}

std::vector<Point> Tesselator::Points()
{
	return _va.points;
}

void Tesselator::BeginVA(GLenum mode, VA *va)
{
	Primitive primitive;
	primitive.mode = mode;
	va->currentPrimitive = primitive;
}

void Tesselator::EndVA(VA *va)
{
	va->primitives.push_back(va->currentPrimitive);
}

void Tesselator::VertexVA(void *p, VA *va)
{
	GLdouble* vertex = (GLdouble*)p;
	
	Point point = {vertex[0], vertex[1], vertex[2]};
	va->points.push_back(point);
	va->currentPrimitive.indices.push_back(va->points.size()-1);		
}

void Tesselator::CombineCallback(GLdouble coords[3],
										  GLdouble *vertex_data[4],
										  GLfloat weight[4], GLdouble **dataOut, VA* va)
{
	//TODO need to free these?
	GLdouble *vertex;
	vertex = (GLdouble *) malloc(3 * sizeof(GLdouble));
	vertex[0] = coords[0];
	vertex[1] = coords[1];
	vertex[2] = coords[2];
	
	*dataOut = vertex;
}

void Tesselator::EdgeFlagCallback(GLboolean flag, VA* va)
{
	// Indicates which edges lie on the polygon boundary
	// (so to enable us to draw outlines), also
	// if this callback is provided triangle fans and strips are
	// converted to independent triangles
}

void Tesselator::ErrorCallback(GLenum errorCode)
{
	const GLubyte *estring;
	
	estring = gluErrorString(errorCode);
	cerr << "Tessellation Error: " << estring << endl;
}