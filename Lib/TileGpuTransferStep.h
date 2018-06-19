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
    vector<Point> triangle_face_indices;
    vector<Point> tristrip_face_indices;
    vector<Point> trifan_face_indices;
};

class TileGpuTransferStep : public ProcessingStep
{
public:
    TileGpuTransferStep(shared_ptr<VectorFile> vectorFile, int layerIndex, int tileWidth, int tileHeight);
    virtual ~TileGpuTransferStep();

    void Run();

private:
    GLuint LoadShader(GLenum shaderType, const char* shaderSource);
    GLuint CreateShaderProgram(const char* vertexShader, const char* fragmentShader);


    void SetupPolygonShaders(GLuint* vao, GLuint* shaderProgram);
    void SetupMaskingShaders(GLuint* vao, GLuint* shaderProgram);
    void DrawOnScreen(GLuint shaderProgram, GLuint textureId, GLuint polygonTextureId);
    void TileToTexture(shared_ptr<GeoTile> geoTile, GLuint* textureId);
    shared_ptr<GeoTile> DrawPolygons(GLuint shaderProgram, shared_ptr<GeoTile> geoTile, shared_ptr<Layer> layer, GLuint* textureId);
    shared_ptr<GeoTile> ReadImage(GLenum mode, Rect boundingRect, Area boundingArea, int numberOfLayers);

    void DrawElements(GLuint shaderProgram, GLenum mode, vector<Point>& elements);
    static void BeginVA(GLenum mode, VA* va);
    static void EndVA(VA* va);
    static void VertexVA(void* p, VA* va);
    static void EdgeFlagCallback(GLboolean flag, VA* va);
    static void CombineCallback(GLdouble coords[3],GLdouble *vertex_data[4],GLfloat weight[4], GLdouble **dataOut, VA* va);
    static void ErrorCallback(GLenum errorCode);

private:
    shared_ptr<VectorFile> _vectorFile;
    int _layerIndex;
    int _tileWidth;
    int _tileHeight;
};

#endif /* TILEGPUTRANSFERSTEP_H */
