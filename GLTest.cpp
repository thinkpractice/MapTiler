#include "Lib/GLWindow.h"
#include <iostream>

using namespace std;

GLuint LoadShader(GLenum shaderType, const char* shaderSource)
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

GLuint CreateShaderProgram(const char* vertexSource, const char* fragmentSource)
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

void SetupPolygonShaders(GLuint* vao, GLuint* shaderProgram)
{
    glGenVertexArrays(1, vao);
    glBindVertexArray(*vao);

    const char* vertexSource = R"glsl(
    #version 130

    in vec2 position;

    void main()
    {
        gl_Position = vec4(position, 0.0, 1.0);
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

    GLint posAttrib = glGetAttribLocation(*shaderProgram, "position");
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(posAttrib);
}

int main(int argc, char** argv)
{
    GLWindow window(1024, 1024);
    window.StartRendering([&](GLFWwindow* window)
    {
        GLuint vao;
        GLuint shaderProgram;
        do
        {

            SetupPolygonShaders(&vao, &shaderProgram);
            float vertices[] = {-1.0, 1.0,
                                1.0, 1.0,
                                1.0, -1.0};
                                //-1.0, -1.0};
            
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            /*GLuint elements[] = {
                0, 1, 2,
                2, 3, 0
            };

            GLuint ebo;
            glGenBuffers(1, &ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/
            glDrawArrays(GL_TRIANGLES, 0, 3);

            glfwSwapBuffers(window);
            glfwPollEvents();

            //glDeleteBuffers(1, &vbo);
            //glDeleteBuffers(1, &ebo);
        }
        while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
            glfwWindowShouldClose(window) == 0);
    });
    return 0;
}
