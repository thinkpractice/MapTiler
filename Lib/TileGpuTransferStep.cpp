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
        while(auto geoTile = InQueue()->dequeue())
        {
            glClearDepth(1.0);
            
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glLoadIdentity();
            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

            GLuint frameBuffer;
            glGenFramebuffers(1, &frameBuffer);
            glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

            glEnable(GL_TEXTURE_2D);
            //Transfer texture to GPU
            GLuint textureId;
            glGenTextures(1, &textureId);

            glBindTexture(GL_TEXTURE_2D, textureId);

            int textureWidth = geoTile->BoundingRect().Width();
            int textureHeight = geoTile->BoundingRect().Height();

            glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, geoTile->Data());
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);


            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glBindTexture(GL_TEXTURE_2D, textureId);

            glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
            glBegin(GL_POLYGON);
                glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, 1.0, 0.0);
                glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 1.0, 0.0);
                glTexCoord2f(1.0, 1.0); glVertex3f(1.0, -1.0, 0.0);
                glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, -1.0, 0.0);
            glEnd();


            GLuint polygonBuffer;
            glGenFramebuffers(1, &polygonBuffer);
            glBindFramebuffer(GL_FRAMEBUFFER, polygonBuffer);

            glEnable(GL_TEXTURE_2D);
            //Transfer texture to GPU
            GLuint polygonTextureId;
            glGenTextures(1, &polygonTextureId);

            glBindTexture(GL_TEXTURE_2D, polygonTextureId);

            glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, polygonTextureId, 0);

            //Get geometries for this tile
            layer->SetSpatialFilter(geoTile->BoundingArea());
            glDisable(GL_TEXTURE_2D);
            GLUtesselator *tess = gluNewTess(); // create a tessellator
            if (!tess)
                cout << "tess null" << endl;

            gluTessCallback(tess, GLU_TESS_VERTEX, (GLvoid (*) ()) &glVertex3dv);
            gluTessCallback(tess, GLU_TESS_BEGIN, (GLvoid (*) ()) &glBegin);
            gluTessCallback(tess, GLU_TESS_END, (GLvoid (*) ()) &glEnd);

            for (auto it = layer->begin(); it != layer->end(); ++it)
            {
                auto feature = *it;
                auto multiPolygon = feature.Geometry().GetMultiPolygon();
                cout << "multipolygon=" << multiPolygon << endl;
               
                glDisable(GL_TEXTURE_2D); 
                
                glColor3f(1.0f, 1.0f, 1.0f);
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
                            double y = 1.0 - (point.Y - geoTile->BoundingRect().Top()) / height;
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
            glReadPixels(0,0, textureWidth, textureHeight, GL_RGBA8, GL_UNSIGNED_BYTE, geoTile->Data());

            glDeleteFramebuffers(1, &frameBuffer);
            glDeleteFramebuffers(1, &polygonBuffer);

            // Swap buffers
            glfwSwapBuffers(window);
            
            //TODO: For now pass tile on to next step
            OutQueue()->enqueue(geoTile);
            OutQueue()->enqueue(maskTile);
            //return;
        }

        while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
            glfwWindowShouldClose(window) == 0)
            glfwPollEvents();
    });
    OutQueue()->enqueue(nullptr);
}


