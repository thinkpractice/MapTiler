#ifndef TILEGPUTRANSFERSTEP_H
#define TILEGPUTRANSFERSTEP_H

#include "ProcessingStep.h"
#include "VectorFile.h"
#include "GLWindow.h"
#include "ShaderProgram.h"
#include "AffineTransform.h"
#include "Tesselator.h"
#include <vector>
#include <memory>

class TileGpuTransferStep : public ProcessingStep
{
public:
    TileGpuTransferStep(const AffineTransform& affineTransform, std::string layerName, int tileWidth, int tileHeight);
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

private:
    void DrawPolygon(Tesselator& tesselator, std::shared_ptr<GeoTile> geoTile, Polygon& polygon);
    void DrawMultiPolygon(Tesselator& tesselator, std::shared_ptr<GeoTile> geoTile, std::shared_ptr<Geometry> geometry);

private:
    std::string _layerName;
    int _tileWidth;
    int _tileHeight;
    AffineTransform _affineTransform;
};

#endif /* TILEGPUTRANSFERSTEP_H */
