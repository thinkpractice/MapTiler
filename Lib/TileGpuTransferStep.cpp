#include "TileGpuTransferStep.h"
#include "GeoTile.h"
#include "FrameBuffer.h"
#include <iostream>

using namespace std;

TileGpuTransferStep::TileGpuTransferStep(shared_ptr<VectorFile> vectorFile, int layerIndex, int tileWidth, int tileHeight)
                        :   ProcessingStep(PreProcessing),
                            _vectorFile(vectorFile),
                            _layerIndex(layerIndex),
                            _tileWidth(tileWidth),
                            _tileHeight(tileHeight)
{
}

TileGpuTransferStep::~TileGpuTransferStep()
{
}

void TileGpuTransferStep::Run()
{
    GLWindow window(_tileWidth, _tileHeight);

    auto layer = _vectorFile->Layers()[_layerIndex];
    window.StartRendering([&](GLFWwindow* window)
    {
        GLuint maskingVao;
		ShaderProgram maskingShaderProgram = SetupMaskingShaders(&maskingVao);
		maskingShaderProgram.Create();		
		
		GLuint vbo;
		GLuint ebo;
		
		BindMaskingVertices(maskingShaderProgram, &vbo, &ebo);
		
        GLuint polygonVao;
		ShaderProgram polygonShaderProgram = SetupPolygonShaders(&polygonVao);
		polygonShaderProgram.Create();

        while(auto geoTile = InQueue()->dequeue())
        {
			cout << "GeoTile ptr" << geoTile << endl;
            glBindVertexArray(polygonVao);
            polygonShaderProgram.Use();
            
			FrameBuffer polygonBuffer;
			polygonBuffer.Bind();
			
            GLuint polygonTextureId;
            auto maskTile = DrawPolygons(polygonShaderProgram, geoTile, layer, &polygonTextureId);
           
			//Do onscreen drawing
			glBindVertexArray(maskingVao);
            FrameBuffer frameBuffer;
			frameBuffer.Bind();
			
			maskingShaderProgram.Use();
			
            GLuint textureId;
            TileToTexture(geoTile, &textureId);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            DrawOnScreen(maskingShaderProgram, textureId, polygonTextureId);
            //TODO output is flipped!
            auto maskedTile = ReadImage(GL_COLOR_ATTACHMENT0, geoTile->BoundingRect(), geoTile->BoundingArea(), geoTile->NumberOfLayers());
            maskedTile->SetUniqueId(geoTile->UniqueId() + "_masked");
            // Swap buffers
            //glfwSwapBuffers(window);

            //Pass original and created tiles on to next step
            OutQueue()->enqueue(geoTile);
            OutQueue()->enqueue(maskTile);
            OutQueue()->enqueue(maskedTile);
			
			glDeleteTextures(1, &textureId);
			glDeleteTextures(1, &polygonTextureId);
		}
		
        while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
            glfwWindowShouldClose(window) == 0)
            glfwPollEvents();
		
		glDeleteBuffers(1, &ebo);
		glDeleteBuffers(1, &vbo);
    });
    OutQueue()->enqueue(nullptr);
}

ShaderProgram TileGpuTransferStep::SetupPolygonShaders(GLuint* vao)
{
    glGenVertexArrays(1, vao);
    glBindVertexArray(*vao);

	ShaderProgram shaderProgram;
	
    const char* vertexSource = R"glsl(
    #version 130

    in vec3 position;

    void main()
    {
        gl_Position = vec4(position, 1.0);
    }
    )glsl";
    
    Shader vertexShader(GL_VERTEX_SHADER, vertexSource);
    shaderProgram.AddShader(vertexShader);
	
    const char* fragmentSource = R"glsl(
    #version 130

    out vec4 outColor;

    void main()
    {
        outColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
    )glsl";
    Shader fragmentShader(GL_FRAGMENT_SHADER, fragmentSource);
    shaderProgram.AddShader(fragmentShader);
	
	return shaderProgram;
}

ShaderProgram TileGpuTransferStep::SetupMaskingShaders(GLuint* vao)
{
    glGenVertexArrays(1, vao);
    glBindVertexArray(*vao);
	
	ShaderProgram shaderProgram;
	
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
    Shader vertexShader(GL_VERTEX_SHADER, vertexSource);
	shaderProgram.AddShader(vertexShader);
    
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
        outColor = polygonColor * aerialColor;
    }
    )glsl";
    Shader fragmentShader(GL_FRAGMENT_SHADER, fragmentSource);
	shaderProgram.AddShader(fragmentShader);
	
	return shaderProgram;
}

void TileGpuTransferStep::BindMaskingVertices(const ShaderProgram& shaderProgram, GLuint* vbo, GLuint* ebo)
{
	float vertices[] = {-1.0, -1.0, 0.0, 0.0,
		1.0, -1.0, 1.0, 0.0,
		1.0, 1.0, 1.0, 1.0,
		-1.0, 1.0, 0.0, 1.0};
		
	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0
	};
	
	glGenBuffers(1, ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
	
	GLint posAttrib = glGetAttribLocation(shaderProgram.ShaderProgramId(), "position");
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glEnableVertexAttribArray(posAttrib);
	
	GLint texAttrib = glGetAttribLocation(shaderProgram.ShaderProgramId(), "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE,
						  4*sizeof(float), (void*)(2*sizeof(float)));
}

void TileGpuTransferStep::DrawOnScreen(const ShaderProgram& shaderProgram, GLuint textureId, GLuint polygonTextureId)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glUniform1i(glGetUniformLocation(shaderProgram.ShaderProgramId(), "aerialTex"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, polygonTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glUniform1i(glGetUniformLocation(shaderProgram.ShaderProgramId(), "polygonTex"), 1);

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

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, geoTile->Data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *textureId, 0);
}

shared_ptr<GeoTile> TileGpuTransferStep::DrawPolygons(const ShaderProgram& shaderProgram, shared_ptr<GeoTile> geoTile, shared_ptr<Layer> layer, GLuint* textureId)
{
    glEnable(GL_TEXTURE_2D);
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

    gluTessProperty(tess, GLU_TESS_BOUNDARY_ONLY, GL_FALSE);
    gluTessProperty(tess, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO);
    gluTessCallback(tess, GLU_TESS_BEGIN_DATA, (GLvoid (*)())TileGpuTransferStep::BeginVA);
    gluTessCallback(tess, GLU_TESS_END_DATA, (GLvoid (*)())TileGpuTransferStep::EndVA);
    gluTessCallback(tess, GLU_TESS_VERTEX_DATA, (GLvoid (*)())TileGpuTransferStep::VertexVA);
    //Causes rendering artefacts!
    gluTessCallback(tess, GLU_TESS_EDGE_FLAG_DATA, (GLvoid(*)())TileGpuTransferStep::EdgeFlagCallback);
    gluTessCallback(tess, GLU_TESS_COMBINE_DATA, (GLvoid (*)())TileGpuTransferStep::CombineCallback);
    gluTessCallback(tess, GLU_TESS_ERROR, (GLvoid (*)())TileGpuTransferStep::ErrorCallback);

    for (auto it = layer->begin(); it != layer->end(); ++it)
    {
        auto feature = *it;
        auto multiPolygon = feature.Geometry().GetMultiPolygon();

        for (auto polygon : multiPolygon)
        {
            GLdouble points[polygon.ExternalRing().Points().size()][3];

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

                    points[i][0] = x;
                    points[i][1] = y;
                    points[i][2] = 0;
                    gluTessVertex(tess, points[i], points[i]);
                    i++;
                }
                gluTessEndContour(tess);
            gluTessEndPolygon(tess);

            DrawElements(shaderProgram, GL_TRIANGLES, va.triangle_face_indices);

            //glDeleteBuffers(1, &vbo);
        }
    }
    gluDeleteTess(tess);

    auto maskTile = ReadImage(GL_COLOR_ATTACHMENT0, geoTile->BoundingRect(), geoTile->BoundingArea(), geoTile->NumberOfLayers());
    maskTile->SetUniqueId(geoTile->UniqueId() + "_mask");

    return maskTile;
}

shared_ptr<GeoTile> TileGpuTransferStep::ReadImage(GLenum mode, Rect boundingRect, Area boundingArea, int numberOfLayers)
{
   glReadBuffer(mode);
   shared_ptr<GeoTile> readTile = make_shared<GeoTile>(boundingRect, boundingArea, numberOfLayers);
   glReadPixels(0,0, boundingRect.Width(), boundingRect.Height(), GL_RGBA, GL_UNSIGNED_BYTE, readTile->Data());
   return readTile;
}

void TileGpuTransferStep::DrawElements(const ShaderProgram& shaderProgram, GLenum mode, vector<Point>& elements)
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STREAM_DRAW);

    GLint posAttrib = glGetAttribLocation(shaderProgram.ShaderProgramId(), "position");
    glVertexAttribPointer(posAttrib, 2, GL_DOUBLE, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(posAttrib);

    glDrawArrays(mode, 0, elements.size());
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
  //TODO need to free these?
  GLdouble *vertex;
  vertex = (GLdouble *) malloc(3 * sizeof(GLdouble));
  vertex[0] = coords[0];
  vertex[1] = coords[1];
  vertex[2] = coords[2];

  *dataOut = vertex;
}

void TileGpuTransferStep::EdgeFlagCallback(GLboolean flag, VA* va)
{
 // Indicates which edges lie on the polygon boundary
 // (so to enable us to draw outlines), also
 // if this callback is provided triangle fans and strips are
 // converted to independent triangles
}

void TileGpuTransferStep::ErrorCallback(GLenum errorCode)
{
   const GLubyte *estring;

   estring = gluErrorString(errorCode);
   fprintf (stderr, "Tessellation Error: %s\n", estring);
}
