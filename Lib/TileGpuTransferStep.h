#ifndef TILEGPUTRANSFERSTEP_H
#define TILEGPUTRANSFERSTEP_H

#include "ProcessingStep.h"
#include "VectorFile.h"
#include "GLWindow.h"
#include "ShaderProgram.h"
#include <vector>
#include <memory>

class TileGpuTransferStep : public ProcessingStep
{
public:
    TileGpuTransferStep(int tileWidth, int tileHeight);
    virtual ~TileGpuTransferStep();

    void Run();

private:
    ShaderProgram SetupPolygonShaders(GLuint* vao);
    ShaderProgram SetupMaskingShaders(GLuint* vao);
    void TileToTexture(std::shared_ptr<GeoTile> geoTile);
    void DrawOnScreen(const ShaderProgram& shaderProgram, GLuint textureId, GLuint polygonTextureId);
    std::shared_ptr<GeoTile> DrawPolygons(const ShaderProgram& shaderProgram, std::shared_ptr<GeoTile> geoTile, const std::vector<Feature> polygonFeatures);
    std::shared_ptr<GeoTile> ReadImage(GLenum mode, Rect boundingRect, Area boundingArea, int numberOfLayers);
	void BindMaskingVertices(const ShaderProgram& shaderProgram, GLuint* vbo, GLuint* ebo);
	
    void DrawElements(const ShaderProgram& shaderProgram, GLenum mode, std::vector<Point>& elements);
   
    Point MapGeoTileCoordinateToGL(std::shared_ptr<GeoTile> geoTile, const Point& point);

    GLenum ColorFormatForTile(std::shared_ptr<GeoTile> geoTile);

private:
    int _tileWidth;
    int _tileHeight;
};

#endif /* TILEGPUTRANSFERSTEP_H */
