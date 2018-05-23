#include "TileGpuTransferStep.h"
#include "GeoTile.h"
#include "GLWindow.h"

TileGpuTransferStep::TileGpuTransferStep()
                        :   ProcessingStep(PreProcessing)
{
}

TileGpuTransferStep::~TileGpuTransferStep()
{
}

void TileGpuTransferStep::Run()
{
    GLWindow window(1024, 1024);

    window.StartRendering([&](GLFWwindow* window)
    {
        glEnable(GL_TEXTURE_2D);
        while(GeoTile* geoTile = InQueue()->dequeue())
        {
            glClearDepth(1.0);
            
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //Draw mask into the stencil buffer
            glLoadIdentity();
            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

            //Transfer texture to GPU
            GLuint textureId;
            glGenTextures(1, &textureId);

            glBindTexture(GL_TEXTURE_2D, textureId);

            int textureWidth = geoTile->BoundingRect().Width();
            int textureHeight = geoTile->BoundingRect().Height();

            glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, geoTile->Data());
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
            glBegin(GL_POLYGON);
                glTexCoord2f(-1.0, 1.0); glVertex3f(-1.0, 1.0, 0.0);
                glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
                glTexCoord2f(1.0, -1.0); glVertex3f(1.0, -1.0, 0.0);
                glTexCoord2f(-1.0, -1.0); glVertex3f(-1.0, -1.0, 0.0);
            glEnd();
             
            // Swap buffers
            glfwSwapBuffers(window);
            
            //TODO: For now pass tile on to next step
            //OutQueue()->enqueue(geoTile);
        }

        glfwPollEvents();
        while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
            glfwWindowShouldClose(window) == 0);
    });
    OutQueue()->enqueue(nullptr);
}
