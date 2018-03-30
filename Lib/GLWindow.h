#ifndef GL_WINDOW_H 
#define GL_WINDOW_H

#include <functional>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

using namespace std;

class GLWindow
{
    public:
        GLWindow(int width, int height);
        virtual ~GLWindow();

        void StartRendering(function<void(GLFWwindow*)> renderLoop);

    private:
        int _width;
        int _height;
};

#endif /* ifndef GL_WINDOW_H */

