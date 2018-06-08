#include "TileGpuTransferStep.h"
#include "GeoTile.h"
#include <iostream>

using namespace std;

TileGpuTransferStep::TileGpuTransferStep(shared_ptr<VectorFile> vectorFile, int layerIndex)
                        :   ProcessingStep(PreProcessing),
                            _vectorFile(vectorFile),
                            _layerIndex(layerIndex)
{
}

TileGpuTransferStep::~TileGpuTransferStep()
{
}

void TileGpuTransferStep::Run()
{
    GLWindow window(1024, 1024);

    auto layer = _vectorFile->Layers()[_layerIndex];
    window.StartRendering([&](GLFWwindow* window)
    {
        GLuint maskingVao;
        GLuint maskingShaderProgram;
        SetupMaskingShaders(&maskingVao, &maskingShaderProgram);

        GLuint polygonVao;
        GLuint polygonShaderProgram;
        SetupPolygonShaders(&polygonVao, &polygonShaderProgram);

        while(auto geoTile = InQueue()->dequeue())
        {
            glBindVertexArray(polygonVao);
            glUseProgram(polygonShaderProgram);
            GLuint polygonBuffer;
            glGenFramebuffers(1, &polygonBuffer);
            glBindFramebuffer(GL_FRAMEBUFFER, polygonBuffer);

            GLuint polygonTextureId;
            auto maskTile = DrawPolygons(polygonShaderProgram, geoTile, layer, &polygonTextureId);

            //Do onscreen drawing
            glBindVertexArray(maskingVao);
            glUseProgram(maskingShaderProgram);

            GLuint frameBuffer;
            glGenFramebuffers(1, &frameBuffer);
            glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

            GLuint textureId;
            TileToTexture(geoTile, &textureId);
            
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            
            DrawOnScreen(maskingShaderProgram, textureId, polygonTextureId);

            // Swap buffers
            glfwSwapBuffers(window);
            
            //Pass original and created tiles on to next step
            OutQueue()->enqueue(geoTile);
            OutQueue()->enqueue(maskTile);

            glDeleteFramebuffers(1, &frameBuffer);
            glDeleteFramebuffers(1, &polygonBuffer);
        }

        while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
            glfwWindowShouldClose(window) == 0)
            glfwPollEvents();
    });
    OutQueue()->enqueue(nullptr);
}

GLuint TileGpuTransferStep::LoadShader(GLenum shaderType, const char* shaderSource)
{

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status != GL_TRUE)
    {
        char buffer[512];
        glGetShaderInfoLog(shader, 512, NULL, buffer);
        cout << "shader error: " << buffer << endl;
    }
    return shader;
}

GLuint TileGpuTransferStep::CreateShaderProgram(const char* vertexSource, const char* fragmentSource)
{
    GLuint vertexShader = LoadShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fragmentSource);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    return shaderProgram;
}

void TileGpuTransferStep::SetupPolygonShaders(GLuint* vao, GLuint* shaderProgram)
{
    glGenVertexArrays(1, vao);
    glBindVertexArray(*vao);

    const char* vertexSource = R"glsl(
    #version 130

    in vec3 position;

    void main()
    {
        gl_Position = vec4(position, 1.0);
    }
    )glsl";
   
    const char* fragmentSource = R"glsl(
    #version 130

    out vec4 outColor;

    void main()
    {
        outColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
    )glsl";

    *shaderProgram = CreateShaderProgram(vertexSource, fragmentSource);
}

void TileGpuTransferStep::SetupMaskingShaders(GLuint* vao, GLuint* shaderProgram)
{
    glGenVertexArrays(1, vao);
    glBindVertexArray(*vao);

    float vertices[] = {-1.0, 1.0, 0.0, 0.0,
                        1.0, 1.0, 1.0, 0.0,
                        1.0, -1.0, 1.0, 1.0,
                        -1.0, -1.0, 0.0, 1.0};
    
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint elements[] = {
        0, 1, 2,
        2, 3, 0
    };

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    const char* vertexSource = R"glsl(
    #version 130

    in vec2 texcoord;
    in vec2 position;

    out vec2 Texcoord;

    void main()
    {
        Texcoord = texcoord;
        gl_Position = vec4(position, 0.0, 1.0);
    }
    )glsl";
   
    const char* fragmentSource = R"glsl(
    #version 130

    in vec2 Texcoord;
    out vec4 outColor;

    uniform sampler2D aerialTex;
    uniform sampler2D polygonTex;

    void main()
    {
        vec4 aerialColor = texture(aerialTex, Texcoord);
        vec4 polygonColor = texture(polygonTex, Texcoord);
        outColor = polygonColor;
    }
    )glsl";

    *shaderProgram = CreateShaderProgram(vertexSource, fragmentSource);

    GLint posAttrib = glGetAttribLocation(*shaderProgram, "position");
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(posAttrib);

    GLint texAttrib = glGetAttribLocation(*shaderProgram, "texcoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE,
                           4*sizeof(float), (void*)(2*sizeof(float)));
}
    
void TileGpuTransferStep::DrawOnScreen(GLuint shaderProgram, GLuint textureId, GLuint polygonTextureId)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glUniform1i(glGetUniformLocation(shaderProgram, "aerialTex"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, polygonTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glUniform1i(glGetUniformLocation(shaderProgram, "polygonTex"), 1);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void TileGpuTransferStep::TileToTexture(shared_ptr<GeoTile> geoTile, GLuint* textureId)
{

    glEnable(GL_TEXTURE_2D);
    //Transfer texture to GPU
    glGenTextures(1, textureId);

    glBindTexture(GL_TEXTURE_2D, *textureId);

    int textureWidth = geoTile->BoundingRect().Width();
    int textureHeight = geoTile->BoundingRect().Height();

    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, geoTile->Data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *textureId, 0);
}

shared_ptr<GeoTile> TileGpuTransferStep::DrawPolygons(GLuint shaderProgram, shared_ptr<GeoTile> geoTile, shared_ptr<Layer> layer, GLuint* textureId)
{
    glEnable(GL_TEXTURE_2D);
    GLuint polygonTextureId;
    glGenTextures(1, textureId);

    glBindTexture(GL_TEXTURE_2D, *textureId);

    int textureWidth = geoTile->BoundingRect().Width();
    int textureHeight = geoTile->BoundingRect().Height();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *textureId, 0);

    //Get geometries for this tile
    layer->SetSpatialFilter(geoTile->BoundingArea());
    GLUtesselator *tess = gluNewTess(); // create a tessellator

    //gluTessProperty(tess, GLU_TESS_BOUNDARY_ONLY, GL_FALSE);
    //gluTessProperty(tess, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO);

    gluTessCallback(tess, GLU_TESS_BEGIN_DATA, (GLvoid (*)())TileGpuTransferStep::BeginVA);
    gluTessCallback(tess, GLU_TESS_END_DATA, (GLvoid (*)())TileGpuTransferStep::EndVA);
    gluTessCallback(tess, GLU_TESS_VERTEX_DATA, (GLvoid (*)())TileGpuTransferStep::VertexVA);
    //Causes rendering artefacts!
    //gluTessCallback(tess, GLU_TESS_COMBINE_DATA, (GLvoid (*)())TileGpuTransferStep::CombineCallback);
    gluTessCallback(tess, GLU_TESS_ERROR, (GLvoid (*)())TileGpuTransferStep::ErrorCallback);

    glDisable(GL_TEXTURE_2D); 
    for (auto it = layer->begin(); it != layer->end(); ++it)
    {
        auto feature = *it;
        auto multiPolygon = feature.Geometry().GetMultiPolygon();
        
        for (auto polygon : multiPolygon)
        {
            GLdouble points[polygon.ExternalRing().Points().size()][2];
            VA va;
            gluTessBeginPolygon(tess, &va);
                gluTessBeginContour(tess);
                int i = 0;
                for (auto point : polygon.ExternalRing())
                {
                    double width = geoTile->BoundingRect().Width() / 2.0;
                    double height = geoTile->BoundingRect().Height() / 2.0;
                    double x = -1.0 + (point.X - geoTile->BoundingRect().Left()) / width;
                    //double y = 1.0 - (point.Y - geoTile->BoundingRect().Top()) / height;
                    double y = -1.0 + (point.Y - geoTile->BoundingRect().Top()) / height;
                    cout << "(" << x << "," << y << ")" << endl;
                    
                    points[i][0] = x;
                    points[i][1] = y;
                    points[i][2] = 0;
                    
                    gluTessVertex(tess, points[i], points[i]);
                    i++;
                }
                gluTessEndContour(tess);
            gluTessEndPolygon(tess);

            for (auto index : va.trifan_face_indices)
            {
                cout << "idx: " << index << endl; // " = (" << points[index][0] << "," << points[index][1]<< endl;
            }

            DrawElements(shaderProgram, GL_TRIANGLES, va.triangle_face_indices);
            DrawElements(shaderProgram, GL_TRIANGLE_STRIP, va.tristrip_face_indices);
            DrawElements(shaderProgram, GL_TRIANGLE_FAN, va.trifan_face_indices);

            //glDeleteBuffers(1, &vbo);
        }
    }
    gluDeleteTess(tess);
    
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    shared_ptr<GeoTile> maskTile = make_shared<GeoTile>(geoTile->BoundingRect(), geoTile->BoundingArea(), geoTile->NumberOfLayers());
    maskTile->SetUniqueId(geoTile->UniqueId() + "_mask");
    glReadPixels(0,0, textureWidth, textureHeight, GL_RGBA, GL_UNSIGNED_BYTE, maskTile->Data());

    return maskTile;
}

void TileGpuTransferStep::DrawElements(GLuint shaderProgram, GLenum mode, vector<Point>& elements)
{
    if (elements.size() == 0)
        return;

    GLdouble points[elements.size()][2];
    int i = 0;
    for (auto point : elements)
    {
        points[i][0] = point.X;
        points[i][1] = point.Y;
        i++;
    }

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    /*GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(GLuint), &elements.front(), GL_STATIC_DRAW);
    glDrawElements(mode, elements.size(), GL_UNSIGNED_INT, 0);

    glDeleteBuffers(1, &ebo);*/
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(posAttrib);

    glDrawArrays(mode, 0, elements.size() * 2);
    glDeleteBuffers(1, &vbo);
}

void TileGpuTransferStep::BeginVA(GLenum mode, VA *va)
{
    va->current_mode = mode;
}

void TileGpuTransferStep::EndVA(VA *va)
{
    va->current_mode = 0;
}

void TileGpuTransferStep::VertexVA(void *p, VA *va)
{
  GLdouble* pointData = (GLdouble*) p;

  Point point(pointData[0], pointData[1]);
  switch(va->current_mode) 
  {
      case GL_TRIANGLES: 
          va->triangle_face_indices.push_back(point);
          break;
      case GL_TRIANGLE_STRIP: 
          va->tristrip_face_indices.push_back(point);
          break;
      case GL_TRIANGLE_FAN: 
          va->trifan_face_indices.push_back(point);
          break;
  }
}

void TileGpuTransferStep::CombineCallback(GLdouble coords[3], 
                     GLdouble *vertex_data[4],
                     GLfloat weight[4], GLdouble **dataOut, VA* va)
{
   GLdouble vertex[3];
   vertex[0] = coords[0];
   vertex[1] = coords[1];
   vertex[2] = coords[2];
   //vertex[3] = (GLuint)((intptr_t)vertex_data[0]);
   /*for (int i = 3; i < 7; i++)
      vertex[i] = weight[0] * vertex_data[0][i] 
                  + weight[1] * vertex_data[1][i]
                  + weight[2] * vertex_data[2][i] 
                  + weight[3] * vertex_data[3][i];*/
   *dataOut = vertex;
}

void TileGpuTransferStep::ErrorCallback(GLenum errorCode)
{
   const GLubyte *estring;

   estring = gluErrorString(errorCode);
   fprintf (stderr, "Tessellation Error: %s\n", estring);
}
