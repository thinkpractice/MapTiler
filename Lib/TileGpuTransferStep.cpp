#include "TileGpuTransferStep.h"
#include "GeoTile.h"
#include "GLWindow.h"
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
        while(GeoTile* geoTile = InQueue()->dequeue())
        {
            glClearDepth(1.0);
            
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glLoadIdentity();
            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

            glEnable(GL_TEXTURE_2D);
            //Transfer texture to GPU
            GLuint textureId;
            glGenTextures(1, &textureId);

            glBindTexture(GL_TEXTURE_2D, textureId);

            int textureWidth = geoTile->BoundingRect().Width();
            int textureHeight = geoTile->BoundingRect().Height();

            cout << "texture size = (" << textureWidth << "," << textureHeight << ")" << endl;
            glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, geoTile->Data());
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
            glBegin(GL_POLYGON);
                glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, 1.0, 0.0);
                glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 1.0, 0.0);
                glTexCoord2f(1.0, 1.0); glVertex3f(1.0, -1.0, 0.0);
                glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, -1.0, 0.0);
            glEnd();
            
            //Get geometries for this tile
            
            cout << "boundingArea=" << geoTile->BoundingArea().LeftTop() << "," << geoTile->BoundingArea().BottomRight() << endl;
            layer->SetSpatialFilter(geoTile->BoundingArea());
            glDisable(GL_TEXTURE_2D);
            glBegin(GL_POLYGON);
                glColor3f(1.0f, 0.0f, 0.0f);
                for (auto it = layer->begin(); it != layer->end(); ++it)
                {
                    cout << "here" << endl;
                    auto feature = *it;
                    auto multiPolygon = feature.Geometry().GetMultiPolygon();
                    cout << "multipolygon=" << multiPolygon << endl;
                    for (auto polygon : multiPolygon)
                    {
                        for (auto point : polygon.ExternalRing())
                        {
                            double x = -1.0 + (point.X - geoTile->BoundingRect().Left()) / 512.0;
                            double y = 1.0 - (point.Y - geoTile->BoundingRect().Top()) / 512.0;
                            cout << "Plotting point (" << x << "," << y << ")" << endl;
                            glVertex3f(x, y, 0.0);
                        }
                    }
                }
            glEnd();
                

            // Swap buffers
            glfwSwapBuffers(window);
            
            //TODO: For now pass tile on to next step
            //OutQueue()->enqueue(geoTile);
            //return;
        }

        while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
            glfwWindowShouldClose(window) == 0)
            glfwPollEvents();
    });
    OutQueue()->enqueue(nullptr);
}
