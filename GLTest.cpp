#include "Lib/GLWindow.h"
#include <iostream>
#include <vector>

using namespace std;

struct Primitive
{
    GLenum mode;
    vector<GLdouble> indices;
};

struct Point
{
    double x;
    double y;
    double z;
    bool original;
};

struct VA
{
    Primitive currentPrimitive;
    vector<Point> points;
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

GLuint CreateShaderProgram(const char* vertexSource, const char* fragmentSource, GLuint* vertexShader, GLuint* fragmentShader)
{
    *vertexShader = LoadShader(GL_VERTEX_SHADER, vertexSource);
    *fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fragmentSource);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, *vertexShader);
    glAttachShader(shaderProgram, *fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    return shaderProgram;
}

void SetupPolygonShaders(GLuint* vao, GLuint* shaderProgram, GLuint* vertexShader, GLuint* fragmentShader)
{
    glGenVertexArrays(1, vao);
    glBindVertexArray(*vao);

    const char* vertexSource = R"glsl(
    #version 130

    in vec3 position;

    void main()
    {
        gl_Position = vec4(position, 1.0);
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

    *shaderProgram = CreateShaderProgram(vertexSource, fragmentSource, vertexShader, fragmentShader);
}

void BeginVA(GLenum mode, VA* va)
{
    cout << "begin va" << endl;
    Primitive primitive;
    primitive.mode = mode;
    va->currentPrimitive = primitive;
}

void EndVA(VA* va)
{
    cout << "end va" << endl;
    va->primitives.push_back(va->currentPrimitive);
}

void VertexVA(void* p, VA* va)
{
    GLdouble* vertex = (GLdouble*)p;
    cout << "vertex" << "[" << vertex[0] << "," << vertex[1] << "," << vertex[2] << "]" << endl;

    Point point = {vertex[0], vertex[1], vertex[2], true};
    va->points.push_back(point);
    va->currentPrimitive.indices.push_back(va->points.size()-1);
}

void ErrorCallback(GLenum errorCode)
{
   const GLubyte *estring;

   estring = gluErrorString(errorCode);
   cout << "Tessellation Error: " << estring << endl;
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
    cout << "combine vertex" << endl;
   //TODO need to free these?
   GLdouble *vertex;
   vertex = (GLdouble *) malloc(3 * sizeof(GLdouble));
   vertex[0] = coords[0];
   vertex[1] = coords[1];
   vertex[2] = coords[2];

   *dataOut = vertex;
}

int main(int argc, char** argv)
{
    GLWindow window(1024, 1024);
    window.StartRendering([&](GLFWwindow* window)
    {
        GLuint vao;
        GLuint shaderProgram;
        GLuint vertexShader;
        GLuint fragmentShader;
        SetupPolygonShaders(&vao, &shaderProgram, &vertexShader, &fragmentShader);

        glBindVertexArray(vao);

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

        int numberOfPoints = 12;
        
        GLUtesselator *tess = gluNewTess(); // create a tessellator

        //gluTessProperty(tess, GLU_TESS_BOUNDARY_ONLY, GL_FALSE);
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


        size_t numberOfVertexPoints = va.points.size() * 3;
        GLdouble vertexPoints[numberOfVertexPoints];
        int vertexIndex = 0;
        for (auto p : va.points)
        {
            vertexPoints[vertexIndex] = p.x;
            vertexPoints[vertexIndex+1] = p.y;
            vertexPoints[vertexIndex+2] = p.z;

            cout << "{" << p.x << "," << p.y << "," << p.z << "," << p.original << "}" << endl;
            vertexIndex += 3;
        }
        cout << "vertices size=" << vertexIndex / 3 << endl;

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPoints), vertexPoints, GL_STATIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
        cout << "number of primitives" << va.primitives.size() << endl;

        GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
        glVertexAttribPointer(posAttrib, 3, GL_DOUBLE, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(posAttrib);
        do
        {


            /*GLdouble points[][3] = {
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
            };*/

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glDrawArrays(GL_TRIANGLES, 0, va.points.size());
            /*for (auto primitive : va.primitives)
            {
                GLuint indices[primitive.indices.size()];
                int j = 0;
                for (auto index : primitive.indices)
                {
                    indices[j] = index;
                    j++;
                }
                cout << "size=" << j << endl;
                GLuint ebo;
                glGenBuffers(1, &ebo);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

                GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
                glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
                glEnableVertexAttribArray(posAttrib);

                glDrawElements(primitive.mode, primitive.indices.size(), GL_UNSIGNED_INT, 0);
            }*/

            glfwSwapBuffers(window);
            glfwPollEvents();

            //glDeleteBuffers(1, &ebo);

        }
        while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
            glfwWindowShouldClose(window) == 0);

        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);

        glDeleteProgram(shaderProgram);
        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);
    });
    return 0;
}
