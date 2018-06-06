#ifndef TILEGPUTRANSFERSTEP_H
#define TILEGPUTRANSFERSTEP_H

#include "ProcessingStep.h"
#include "VectorFile.h"
#include "GLWindow.h"

class TileGpuTransferStep : public ProcessingStep
{
public:
    TileGpuTransferStep(shared_ptr<VectorFile> vectorFile, int layerIndex);
    virtual ~TileGpuTransferStep();

    void Run();

private:
    void DrawOnScreen();
    void TileToTexture(shared_ptr<GeoTile> geoTile, GLuint* textureId);
    shared_ptr<GeoTile> DrawPolygons(shared_ptr<GeoTile> geoTile, shared_ptr<Layer> layer, GLuint* textureId);

private:
    shared_ptr<VectorFile> _vectorFile;
    int _layerIndex;
};

#endif /* TILEGPUTRANSFERSTEP_H */
