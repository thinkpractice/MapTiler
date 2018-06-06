#ifndef TILEGPUTRANSFERSTEP_H
#define TILEGPUTRANSFERSTEP_H

#include "ProcessingStep.h"
#include "VectorFile.h"
#include "GLWindow.h"
#include <vector>

using namespace std;

struct VA
{
    GLenum current_mode; // Drawing mode (GL_TRIANGLE, *_FAN or *_STRIP)
    vector<GLuint> triangle_face_indices;
    vector<GLuint> tristrip_face_indices;
    vector<GLuint> trifan_face_indices;
};

class TileGpuTransferStep : public ProcessingStep
{
public:
    TileGpuTransferStep(shared_ptr<VectorFile> vectorFile, int layerIndex);
    virtual ~TileGpuTransferStep();

    void Run();

private:
    void SetupShaders(GLuint* vao, GLuint* shaderProgram);
    void DrawOnScreen(GLuint shaderProgram, GLuint textureId, GLuint polygonTextureId);
    void TileToTexture(shared_ptr<GeoTile> geoTile, GLuint* textureId);
    shared_ptr<GeoTile> DrawPolygons(shared_ptr<GeoTile> geoTile, shared_ptr<Layer> layer, GLuint* textureId);

    void BeginVA(GLenum mode, VA* va);
    void EndVA(VA* va);
    void VertexVA(void* p, VA* va);

private:
    shared_ptr<VectorFile> _vectorFile;
    int _layerIndex;
};

#endif /* TILEGPUTRANSFERSTEP_H */
