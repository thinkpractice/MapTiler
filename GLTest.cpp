#include "Lib/GLWindow.h"
#include <iostream>
#include <vector>

using namespace std;

struct Primitive
{
    GLenum mode;
    vector<GLdouble> points;
};

struct VA
{
    Primitive currentPrimitive;
    vector<Primitive> primitives;
};

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
        outColor = vec4(0.0, 1.0, 0.0, 1.0);
    }
    )glsl";

    *shaderProgram = CreateShaderProgram(vertexSource, fragmentSource);
}

void BeginVA(GLenum mode, VA* va)
{
    Primitive primitive;
    primitive.mode = mode;
    va->currentPrimitive = primitive;
}

void EndVA(VA* va)
{
    va->primitives.push_back(va->currentPrimitive);
}

void VertexVA(void* p, VA* va)
{
    GLdouble* vertex = (GLdouble*)p;
    va->currentPrimitive.points.push_back(vertex[0]);
    va->currentPrimitive.points.push_back(vertex[1]);
    va->currentPrimitive.points.push_back(vertex[2]);
}

void ErrorCallback(GLenum errorCode)
{
   const GLubyte *estring;

   estring = gluErrorString(errorCode);
   cout << "Tessellation Error: " << estring << endl;
}

GLuint PushPrimitivePoints(Primitive primitive)
{
    GLuint vbo1;
    glGenBuffers(1, &vbo1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    glBufferData(GL_ARRAY_BUFFER, primitive.points.size(), &primitive.points[0], GL_STATIC_DRAW);
    return vbo1;
}

void EdgeFlagCallback(GLboolean flag, VA* va)
{
 // Indicates which edges lie on the polygon boundary 
 // (so to enable us to draw outlines), also 
 // if this callback is provided triangle fans and strips are
 // converted to independent triangles
}

void CombineCallback(GLdouble coords[3],
                     GLdouble *vertex_data[4],
                     GLfloat weight[4], GLdouble **dataOut, VA* va)
{
   GLdouble vertex[3];
   vertex[0] = coords[0];
   vertex[1] = coords[1];
   vertex[2] = coords[2];
   va->currentPrimitive.points.push_back(coords[0]);
   va->currentPrimitive.points.push_back(coords[1]);
   va->currentPrimitive.points.push_back(coords[2]);
   *dataOut = vertex;
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

            GLdouble points[][3] = {
                {-1.28247,-1.01088, 0.0},
                {-1.76332,-0.671701, 0.0},
                {-1.67036,-0.539197, 0.0},
                {-1.62333,-0.572359, 0.0},
                {-1.2211,0.000491298, 0.0},
                {-1.20929,-0.76946, 0.0},
                {-1.14493,-0.814861, 0.0},
                {-0.849858,-0.256995, 0.0},
                {-1.17161,-0.0344123, 0.0},
                {-0.934818,0.303098, 0.0},
                {-0.615166,0.0776264, 0.0},
                {-0.849858,-0.256995, 0.0}
            };

            int numberOfPoints = 12; //sizeof(points) / 3;
            /*float triangle[] = {-1.17161,-0.0344123,1.0, 0.0, 0.0,
                 -1.20929,-0.76946,1.0, 1.0, 0.0,
                 -1.2211,0.000491298,1.0, 0.0, 1.0};

            float triangle_fan[] = {
                -1.28247,-1.01088,0.0, 0.0, 1.0,
                -1.76332,-0.671701,0.0, 0.0, 1.0,
                -1.67036,-0.539197,0.0, 0.0, 1.0,
                -1.62333,-0.572359,0.0, 0.0, 1.0,
                -1.2211,0.000491298,0.0, 0.0, 1.0,
                -1.20929,-0.76946,0.0, 0.0, 1.0,
                -1.14493,-0.814861,0.0, 0.0, 1.0,
                -0.849858,-0.256995,0.0, 0.0, 1.0,
                -1.20929,-0.76946,0.0, 0.0, 1.0,
                -1.17161,-0.0344123,0.0, 0.0, 1.0,
                -0.934818,0.303098,1.0, 0.0, 0.0,
                -0.615166,0.0776264,1.0, 0.0, 0.0};*/

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

            GLUtesselator *tess = gluNewTess(); // create a tessellator
            
            gluTessProperty(tess, GLU_TESS_BOUNDARY_ONLY, GL_FALSE);
            gluTessProperty(tess, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO);
            gluTessCallback(tess, GLU_TESS_BEGIN_DATA, (GLvoid (*)())BeginVA);
            gluTessCallback(tess, GLU_TESS_END_DATA, (GLvoid (*)())EndVA);
            gluTessCallback(tess, GLU_TESS_VERTEX_DATA, (GLvoid (*)())VertexVA);
            gluTessCallback(tess, GLU_TESS_EDGE_FLAG_DATA, (GLvoid(*)())EdgeFlagCallback);
            //Causes rendering artefacts!
            gluTessCallback(tess, GLU_TESS_COMBINE_DATA, (GLvoid (*)())CombineCallback);
            gluTessCallback(tess, GLU_TESS_ERROR, (GLvoid (*)())ErrorCallback);

            VA va;
            gluTessBeginPolygon(tess, &va);
                gluTessBeginContour(tess);
                for (int i = 0; i < numberOfPoints ; i++)
                {
                    gluTessVertex(tess, points[i], points[i]);
                }
                gluTessEndContour(tess);
            gluTessEndPolygon(tess);
            gluDeleteTess(tess);

            GLuint vbos[va.primitives.size()];
            int i = 0;
            for (auto primitive : va.primitives)
            {
                vbos[i] = PushPrimitivePoints(primitive);
                i++;
            }


            /*GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
            glEnableVertexAttribArray(colAttrib);
            glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
                       5*sizeof(float), (void*)(2*sizeof(float)));*/

            i = 0;
            for (auto primitive : va.primitives)
            {
                cout << "drawing: " << primitive.mode << endl;
                glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);
                GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
                glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
                glEnableVertexAttribArray(posAttrib);

                glDrawArrays(primitive.mode, 0, primitive.points.size() / 3);
                i++;
            }
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
