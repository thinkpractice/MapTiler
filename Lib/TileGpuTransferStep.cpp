#include "TileGpuTransferStep.h"
#include "GeoTile.h"
#include "FrameBuffer.h"
#include <iostream>

using namespace std;

TileGpuTransferStep::TileGpuTransferStep(const AffineTransform& affineTransform, std::string layerName, int tileWidth, int tileHeight)
                        :   ProcessingStep(Processing),
                            _layerName(layerName),
                            _tileWidth(tileWidth),
                            _tileHeight(tileHeight),
                            _affineTransform(affineTransform)
{
}

TileGpuTransferStep::~TileGpuTransferStep()
{
}

void TileGpuTransferStep::Run()
{
    GLWindow window(_tileWidth, _tileHeight);

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
            vector<Feature> polygonFeatures = stepData->GetMetadataFeatures(_layerName);
			for (auto tilePair : stepData->Tiles())
			{
				auto geoTile = tilePair.second;
                GLenum colorFormat = ColorFormatForTile(geoTile.tile);

                glBindVertexArray(polygonVao);

                FrameBuffer polygonBuffer(_tileWidth, _tileHeight, colorFormat);
                polygonBuffer.Bind();
				polygonBuffer.Clear();

				polygonShaderProgram.Use();

                auto maskTile = DrawPolygons(polygonShaderProgram, geoTile.tile, polygonFeatures);
			
				//Do onscreen drawing
                glBindVertexArray(maskingVao);
                FrameBuffer frameBuffer(_tileWidth, _tileHeight, colorFormat);
                frameBuffer.Bind();
                frameBuffer.Clear();

                TileToTexture(geoTile.tile);

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
                /*glClearColor(0,0,0,1);
                glClear(GL_COLOR_BUFFER_BIT);*/
				
				maskingShaderProgram.Use();
                DrawOnScreen(maskingShaderProgram, frameBuffer.TextureId(), polygonBuffer.TextureId());
				
                auto maskedTile = ReadImage(GL_COLOR_ATTACHMENT0, geoTile.tile->BoundingRect(), geoTile.tile->BoundingArea(), 4);
				// Swap buffers
				glfwSwapBuffers(window);

				//Pass original and created tiles on to next step
                stepData->AddProcessedTile(tilePair.first + "_mask", maskTile, geoTile.year);
                stepData->AddProcessedTile(tilePair.first + "_masked", maskedTile, geoTile.year);

				glfwPollEvents();

				/*if (glfwGetKey(window, GLFW_KEY_ESCAPE ) == GLFW_PRESS || glfwWindowShouldClose(window) != 0)
					break;*/
			}
			OutQueue()->enqueue(stepData);
        }

        glDeleteBuffers(1, &ebo);
        glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &maskingVao);
		glDeleteVertexArrays(1, &polygonVao);
		
    });
    DoneProcessing();
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
        outColor = aerialColor * polygonColor;
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
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
	glEnableVertexAttribArray(posAttrib);
	
    GLint texAttrib = glGetAttribLocation(shaderProgram.ShaderProgramId(), "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE,
                          4*sizeof(float), reinterpret_cast<void*>(2*sizeof(float)));
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

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void TileGpuTransferStep::TileToTexture(shared_ptr<GeoTile> geoTile)
{
    int textureWidth = geoTile->BoundingRect().IntegerWidth();
    int textureHeight = geoTile->BoundingRect().IntegerHeight();

    GLenum colorFormat = ColorFormatForTile(geoTile);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureWidth, textureHeight, 0, colorFormat, GL_UNSIGNED_BYTE, geoTile->Data());
}

void TileGpuTransferStep::DrawPolygon(Tesselator& tesselator, std::shared_ptr<GeoTile> geoTile, Polygon& polygon)
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
    tesselator.EndPolygon();
}

void TileGpuTransferStep::DrawMultiPolygon(Tesselator& tesselator, std::shared_ptr<GeoTile> geoTile, std::shared_ptr<Geometry> geometry)
{
    auto multiPolygon = dynamic_pointer_cast<MultiPolygon>(geometry);
    for (auto polygon : *multiPolygon)
    {
        DrawPolygon(tesselator, geoTile, polygon);
    }
}

shared_ptr<GeoTile> TileGpuTransferStep::DrawPolygons(const ShaderProgram& shaderProgram, shared_ptr<GeoTile> geoTile, const vector<Feature> polygonFeatures)
{
	Tesselator tesselator;
	int numberOfPolygons = 0;
    for (auto feature : polygonFeatures)
    {
        auto geometry = feature.GetGeometry();

        if (!geometry->IsPolygon() && !geometry->IsMultiPolygon())
            continue;

        auto mappedGeometry = _affineTransform.ReverseTransform(geometry);
        if (geometry->IsPolygon())
        {
            auto polygon = dynamic_pointer_cast<Polygon>(mappedGeometry);
            DrawPolygon(tesselator, geoTile, *polygon);
        }
        else if (geometry->IsMultiPolygon())
        {
            DrawMultiPolygon(tesselator, geoTile, mappedGeometry);
        }
        else
        {
            std::cerr << "MapTiler doesn't support drawing geometry of this type" << endl;
        }
        numberOfPolygons++;
    }

    for (auto primitive : tesselator.Primitives())
    {
        vector<Point> triangles = primitive.points;
        DrawElements(shaderProgram, GL_TRIANGLES, triangles);
    }
    return ReadImage(GL_COLOR_ATTACHMENT0, geoTile->BoundingRect(), geoTile->BoundingArea(), 4);
}

shared_ptr<GeoTile> TileGpuTransferStep::ReadImage(GLenum mode, Rect boundingRect, Area boundingArea, int numberOfLayers)
{
   glReadBuffer(mode);
   shared_ptr<GeoTile> readTile = make_shared<GeoTile>(boundingRect, boundingArea, numberOfLayers);
   glReadPixels(0,0, boundingRect.IntegerWidth(), boundingRect.IntegerHeight(), GL_RGBA, GL_UNSIGNED_BYTE, readTile->Data());
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
    glVertexAttribPointer(posAttrib, 2, GL_DOUBLE, GL_FALSE, 0, nullptr);
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

GLenum TileGpuTransferStep::ColorFormatForTile(shared_ptr<GeoTile> geoTile)
{
    return geoTile->NumberOfLayers() == 4 ? GL_RGBA : GL_RGB;
}
