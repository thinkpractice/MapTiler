#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()
#include <iostream>
#include <regex>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <ogr_spatialref.h>
#include <ogrsf_frmts.h>
#include "GeoMapProvider.h"

using namespace std;

void printMetadataList(GDALDataset* dataset)
{
    char** metadataList = dataset->GetMetadataDomainList();
    for (int i = 0; metadataList[i]; i++)
    {
        cout << metadataList[i] << endl;
    }
}


GByte* getDataForBand(GDALDataset *poDataset, int rasterIndex, int x, int y, int width, int height)
{
    GDALRasterBand* band = poDataset->GetRasterBand(rasterIndex);
     
    printf( "Type=%s, ColorInterp=%s\n",
    GDALGetDataTypeName(band->GetRasterDataType()),
    GDALGetColorInterpretationName(
            band->GetColorInterpretation()) );

    GByte* data = (GByte*)CPLMalloc(width * height);

    //CPLErr error = band->ReadBlock(x, y, data);
    CPLErr error = band->RasterIO(GDALRWFlag::GF_Read, x, y, width, height, data, width, height, GDALDataType::GDT_Byte,0,0);
    if (error != CPLErr::CE_None)
    {
        cout << "error=" << error << endl;
        CPLFree(data);
        return nullptr;
    }
    return data;
}
        
int main()
{
    GDALAllRegister();

    string filename = u8"WMTS:https://geodata.nationaalgeoregister.nl/luchtfoto/rgb/wmts/1.0.0/WMTSCapabilities.xml";
    GeoMapProvider mapProvider(filename);

    int i = 0;
    for (auto& dataset : mapProvider.Maps())
    {
        cout << i << ") title=" << dataset->Title() << ", url=" << dataset->Filename() << endl;
        i++;
    }

    string input;
    int datasetIndex = -1;
    do
    {
        cout << "Choose a dataset" << endl;
        getline(cin, input);
        datasetIndex = stoi(input);
    }
    while (datasetIndex < 0 || datasetIndex >= mapProvider.Maps().size());

    GeoMap* chosenMap = mapProvider.Maps()[datasetIndex];

    cout << "===GeoTransform===" << endl;
    double geoTransform[6];
    chosenMap->MapTransform().GetTransformMatrix(geoTransform);
    for (int i = 0; i < 6; i++)
    {
        cout << geoTransform[i] << endl;
    }

    cout << "===GeoTransform===" << endl;

    cout << "layer count=" << chosenMap->LayerCount() << endl;
    cout << "raster count=" << chosenMap->RasterCount() << endl;
    cout << "raster X size=" << chosenMap->WidthInPixels() << endl;
    cout << "raster Y size=" << chosenMap->HeightInPixels() << endl;

    /*cout << "Get first raster band" << endl;
    GDALRasterBand* poBand = poDataset->GetRasterBand(1);
    cout << "poDataset = " << poDataset << "poBand = " << poBand << endl;
    const char* projection = poDataset->GetProjectionRef();
    cout << "projection ref= " << projection << endl;

    cout << "Get BlockSize" << endl;
    int nXBlockSize, nYBlockSize;
    poBand->GetBlockSize(&nXBlockSize, &nYBlockSize);

    cout << "Blocksize (" << nXBlockSize << "," << nYBlockSize << ")" << endl;

    cout << "Calculate number of blocks" << endl;
    int nXBlocks = (poDataset->GetRasterXSize() + nXBlockSize - 1) / nXBlockSize;
    int nYBlocks = (poDataset->GetRasterYSize() + nYBlockSize - 1) / nYBlockSize;
    
    cout << "blocksize (" << nXBlockSize << "," << nYBlockSize << ")";
    cout << "number of blocks (" << nXBlocks << "," << nYBlocks << ")";

    int x = poDataset->GetRasterXSize() / 2 + 10000;
    int y = poDataset->GetRasterYSize() / 2 + 10000;
    int width = nXBlockSize * 10;
    int height = nYBlockSize * 10;
    GByte* redData = getDataForBand(poDataset, 1, x, y, width, height);
    GByte* blueData = getDataForBand(poDataset, 2, x, y, width, height);
    GByte* greenData = getDataForBand(poDataset, 3, x, y, width, height);
    GByte* alphaData = getDataForBand(poDataset, 4, x, y, width, height);
    if (!redData || !blueData || !greenData)
    {
        cout << "redData=" << redData << ",blueData=" << blueData << ",greenData=" << greenData << endl;
        return -1;
    }
    //ignore 4th raster for now
    int numberOfTilePixels = width*height;
    int arrayLength = numberOfTilePixels * rasterCount;

    GLubyte textureData[arrayLength];
    for (int x  = 0, i = 0; x < arrayLength; x += rasterCount, i++)
    {
        textureData[x] = redData[i];
        textureData[x+1] = blueData[i];
        textureData[x+2] = greenData[i];
        textureData[x+3] = alphaData[i];
    }

    CPLFree(redData);
    CPLFree(blueData);
    CPLFree(greenData);
    
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // Open a window and create its OpenGL context
    GLFWwindow* window; // (In the accompanying source code, this variable is global for simplicity)
    window = glfwCreateWindow( 1024, 768, "Tutorial 01", NULL, NULL);
    if( window == NULL )
    {
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // Initialize GLEW
    glewExperimental=true; // Needed in core profile
    if (glewInit() != GLEW_OK) 
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glClearStencil(0);
    do
    {
        glClearDepth(1.0);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Draw mask into the stencil buffer
        glLoadIdentity();
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_ALWAYS, 1, 1);
        glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
        
        glBegin(GL_TRIANGLES);
            glVertex2f(0.25, 0.0);
            glVertex2f(0.75, 0.0);
            glVertex2f(0.5, 0.5);
        glEnd();

        //Draw texture
        glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
        glStencilFunc(GL_EQUAL, 1, 1);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

        glEnable(GL_TEXTURE_2D);
        //glColor3f(1.0, 1.0, 1.0);
        glBindTexture( GL_TEXTURE_2D, textureID);
        glBegin(GL_POLYGON);
            glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
            glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 0.0, 0.0);
            glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
            glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
        glEnd();

        glDisable(GL_STENCIL_TEST);
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
    glfwWindowShouldClose(window) == 0 );
    
    GDALClose(poDataset);
    */
    return 0;
}
