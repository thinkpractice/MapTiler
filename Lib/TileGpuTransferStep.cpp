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
        GLint maxSize;
        glGetIntegerv( GL_MAX_TEXTURE_SIZE, &maxSize );
        cout << "max texture size=" << maxSize << endl;
        while(auto geoTile = InQueue()->dequeue())
        {
            //glLoadIdentity();
            GLuint frameBuffer;
            glGenFramebuffers(1, &frameBuffer);
            glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

            GLuint textureId;
            TileToTexture(geoTile, &textureId);
            
            GLuint polygonBuffer;
            glGenFramebuffers(1, &polygonBuffer);
            glBindFramebuffer(GL_FRAMEBUFFER, polygonBuffer);
            GLuint polygonTextureId;
            auto maskTile = DrawPolygons(geoTile, layer, &polygonTextureId);

            //Do onscreen drawing
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glEnable(GL_TEXTURE_2D);
            //glBindTexture(GL_TEXTURE_2D, textureId);
            glBindTexture(GL_TEXTURE_2D, polygonTextureId);

            glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
            glBegin(GL_POLYGON);
                glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, 1.0, 0.0);
                glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 1.0, 0.0);
                glTexCoord2f(1.0, 1.0); glVertex3f(1.0, -1.0, 0.0);
                glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, -1.0, 0.0);
            glEnd();

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

shared_ptr<GeoTile> TileGpuTransferStep::DrawPolygons(shared_ptr<GeoTile> geoTile, shared_ptr<Layer> layer, GLuint* textureId)
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

    gluTessCallback(tess, GLU_TESS_VERTEX, (GLvoid (*) ()) &glVertex3dv);
    gluTessCallback(tess, GLU_TESS_BEGIN, (GLvoid (*) ()) &glBegin);
    gluTessCallback(tess, GLU_TESS_END, (GLvoid (*) ()) &glEnd);

    for (auto it = layer->begin(); it != layer->end(); ++it)
    {
        auto feature = *it;
        auto multiPolygon = feature.Geometry().GetMultiPolygon();
        cout << "multipolygon=" << multiPolygon << endl;
       
        glDisable(GL_TEXTURE_2D); 
        
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        for (auto polygon : multiPolygon)
        {
            GLdouble points[polygon.ExternalRing().Points().size()][3];
            gluTessBeginPolygon(tess, NULL);
                gluTessBeginContour(tess);
                int i = 0;
                for (auto point : polygon.ExternalRing())
                {
                    double width = geoTile->BoundingRect().Width() / 2.0;
                    double height = geoTile->BoundingRect().Height() / 2.0;
                    double x = -1.0 + (point.X - geoTile->BoundingRect().Left()) / width;
                    //double y = 1.0 - (point.Y - geoTile->BoundingRect().Top()) / height;
                    double y = -1.0 + (point.Y - geoTile->BoundingRect().Top()) / height;
                    cout << "Plotting point (" << x << "," << y << ")" << endl;
                    
                    points[i][0] = x;
                    points[i][1] = y;
                    points[i][2] = 0.0;
                    gluTessVertex(tess, points[i], points[i]);
                    i++;
                }
                gluTessEndContour(tess);
            gluTessEndPolygon(tess);
        }
    }
    gluDeleteTess(tess);
    
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    shared_ptr<GeoTile> maskTile = make_shared<GeoTile>(geoTile->BoundingRect(), geoTile->BoundingArea(), geoTile->NumberOfLayers());
    maskTile->SetUniqueId(geoTile->UniqueId() + "_mask");
    glReadPixels(0,0, textureWidth, textureHeight, GL_RGBA, GL_UNSIGNED_BYTE, maskTile->Data());

    return maskTile;
}
