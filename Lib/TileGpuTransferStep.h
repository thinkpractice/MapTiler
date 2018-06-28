#ifndef TILEGPUTRANSFERSTEP_H
#define TILEGPUTRANSFERSTEP_H

#include "ProcessingStep.h"
#include "VectorFile.h"
#include "GLWindow.h"
#include "ShaderProgram.h"
#include <vector>

using namespace std;

class TileGpuTransferStep : public ProcessingStep
{
public:
    TileGpuTransferStep(shared_ptr<VectorFile> vectorFile, int layerIndex, int tileWidth, int tileHeight);
    virtual ~TileGpuTransferStep();

    void Run();

private:
    ShaderProgram SetupPolygonShaders(GLuint* vao);
    ShaderProgram SetupMaskingShaders(GLuint* vao);
    void TileToTexture(shared_ptr<GeoTile> geoTile, GLuint* textureId);
    void DrawOnScreen(const ShaderProgram& shaderProgram, GLuint textureId, GLuint polygonTextureId);
    shared_ptr<GeoTile> DrawPolygons(const ShaderProgram& shaderProgram, shared_ptr<GeoTile> geoTile, shared_ptr<Layer> layer, GLuint* textureId);
    shared_ptr<GeoTile> ReadImage(GLenum mode, Rect boundingRect, Area boundingArea, int numberOfLayers);
	void BindMaskingVertices(const ShaderProgram& shaderProgram, GLuint* vbo, GLuint* ebo);
	
    void DrawElements(const ShaderProgram& shaderProgram, GLenum mode, vector<Point>& elements);
   
	Point MapGeoTileCoordinateToGL(shared_ptr<GeoTile> geoTile, const Point& point);

private:
    shared_ptr<VectorFile> _vectorFile;
    int _layerIndex;
    int _tileWidth;
    int _tileHeight;
};

#endif /* TILEGPUTRANSFERSTEP_H */
