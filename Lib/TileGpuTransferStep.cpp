#include "TileGpuTransferStep.h"
#include "GeoTile.h"
#include "FrameBuffer.h"
#include "Tesselator.h"
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

        while(auto stepData = InQueue()->dequeue())
        {
			auto geoTile = stepData->Tile();
            glBindVertexArray(polygonVao);

            FrameBuffer polygonBuffer;
            polygonBuffer.Bind();
			polygonBuffer.Clear();

            polygonShaderProgram.Use();

            GLuint polygonTextureId;
            auto maskTile = DrawPolygons(polygonShaderProgram, geoTile, layer, &polygonTextureId);
           
			//Do onscreen drawing
            FrameBuffer frameBuffer;
			frameBuffer.Bind();
			frameBuffer.Clear();
			
            GLuint textureId;
            TileToTexture(geoTile, &textureId);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glBindVertexArray(maskingVao);
			glClearColor(0,0,0,1);
			glClear(GL_COLOR_BUFFER_BIT);
			
			maskingShaderProgram.Use();
            DrawOnScreen(maskingShaderProgram, textureId, polygonTextureId);
			
            auto maskedTile = ReadImage(GL_COLOR_ATTACHMENT0, geoTile->BoundingRect(), geoTile->BoundingArea(), geoTile->NumberOfLayers());
            maskedTile->SetUniqueId(geoTile->UniqueId() + "_masked");
            // Swap buffers
            glfwSwapBuffers(window);

            //Pass original and created tiles on to next step
			stepData->SetMaskTile(maskTile);
			stepData->SetMaskedTile(maskedTile);
            OutQueue()->enqueue(stepData);

            glDeleteTextures(1, &textureId);
            glDeleteTextures(1, &polygonTextureId);

            glfwPollEvents();

            /*if (glfwGetKey(window, GLFW_KEY_ESCAPE ) == GLFW_PRESS || glfwWindowShouldClose(window) != 0)
                break;*/
        }

        glDeleteBuffers(1, &ebo);
        glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &maskingVao);
		glDeleteVertexArrays(1, &polygonVao);
		
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

	Tesselator tesselator;
	int numberOfPolygons = 0;
    for (auto it = layer->begin(); it != layer->end(); ++it)
    {
        auto feature = *it;
        auto multiPolygon = feature.Geometry().GetMultiPolygon();

        for (auto polygon : multiPolygon)
        {
            tesselator.BeginPolygon(polygon.ExternalRing().Points().size());
                tesselator.BeginContour();
                int i = 0;
                for (auto point : polygon.ExternalRing())
                {
                    Point glPoint = MapGeoTileCoordinateToGL(geoTile, point);
                    tesselator.AddVertex(glPoint);
                    i++;
                }
                tesselator.EndContour();

				vector<Point> triangles = tesselator.Points();
				DrawElements(shaderProgram, GL_TRIANGLES, triangles);
            tesselator.EndPolygon();
        }
        numberOfPolygons++;
    }

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);// GL_STREAM_DRAW);

    GLint posAttrib = glGetAttribLocation(shaderProgram.ShaderProgramId(), "position");
    glVertexAttribPointer(posAttrib, 2, GL_DOUBLE, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(posAttrib);

    glDrawArrays(mode, 0, elements.size());
    glDeleteBuffers(1, &vbo);
}

Point TileGpuTransferStep::MapGeoTileCoordinateToGL(shared_ptr< GeoTile > geoTile, const Point& point)
{
	double width = geoTile->BoundingRect().Width() / 2.0;
	double height = geoTile->BoundingRect().Height() / 2.0;
	double x = -1.0 + (point.X - geoTile->BoundingRect().Left()) / width;
	//double y = 1.0 - (point.Y - geoTile->BoundingRect().Top()) / height;
	double y = -1.0 + (point.Y - geoTile->BoundingRect().Top()) / height;
	
	return Point(x, y);
}

