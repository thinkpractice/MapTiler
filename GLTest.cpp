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
    in vec3 color;
    out vec3 Color;

    void main()
    {
        Color = color;
        gl_Position = vec4(position, 0.0, 1.0);
    }
    )glsl";
   
    const char* fragmentSource = R"glsl(
    #version 130

    in vec3 Color;
    out vec4 outColor;

    void main()
    {
        outColor = vec4(Color, 1.0);
    }
    )glsl";

    *shaderProgram = CreateShaderProgram(vertexSource, fragmentSource);

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
            /*float vertices[] = {-1.28247,-1.01088, 1.0, 0.0, 0.0,
                                -1.76332,-0.671701,1.0, 0.0, 0.0,
                                -1.67036,-0.539197,1.0, 0.0, 0.0,
                                -1.62333,-0.572359,1.0, 0.0, 0.0,
                                -1.2211,0.000491298,0.0, 1.0, 0.0,
                                -1.20929,-0.76946,0.0, 1.0, 0.0,
                                -1.14493,-0.814861,0.0, 1.0, 0.0,
                                -0.849858,-0.256995,0.0, 1.0, 0.0,
                                -1.20929,-0.76946, 0.0, 0.0, 1.0,
                                -1.17161,-0.0344123,0.0, 0.0, 1.0,
                                -0.934818,0.303098,0.0, 0.0, 1.0,
                                -0.615166,0.0776264,0.0, 0.0, 1.0};*/

            float vertices[] = {-1.0,-0.5, 1.0, 0.0, 0.0,
                                -0.5,-0.5, 1.0, 0.0, 0.0,
                                -0.5,-1.0, 1.0, 0.0, 0.0,
                                -1.0,-1.0, 1.0, 0.0, 0.0,
                                -1.2211,0.000491298,0.0, 1.0, 0.0,
                                -1.20929,-0.76946,0.0, 1.0, 0.0,
                                -1.14493,-0.814861,0.0, 1.0, 0.0,
                                -0.849858,-0.256995,0.0, 1.0, 0.0,
                                -1.20929,-0.76946, 0.0, 0.0, 1.0,
                                -1.17161,-0.0344123,0.0, 0.0, 1.0,
                                -0.934818,0.303098,0.0, 0.0, 1.0,
                                -0.615166,0.0776264,0.0, 0.0, 1.0};
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
            glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
            glEnableVertexAttribArray(posAttrib);

            GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
            glEnableVertexAttribArray(colAttrib);
            glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
                       5*sizeof(float), (void*)(2*sizeof(float)));

            GLuint elements[] = {
                0, 1, 2,
                2, 3, 0
            };

            GLuint ebo;
            glGenBuffers(1, &ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            //glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(vertices));

            GLuint elements2[] = {
                4, 5, 6,
                6, 7, 4
            };

            GLuint ebo2;
            glGenBuffers(1, &ebo2);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo2);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements2), elements2, GL_STATIC_DRAW);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
