#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()
#include <iostream>
#include <regex>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

struct Dataset
{
    string title;
    string url;
};

vector<string> splitKeyValuePair(const char* keyValueString)
{
    vector <string> matchResults;

    regex re(R"((\w+)=(.+))");
    cmatch matches;
    
    if (regex_match(keyValueString, matches, re))
    {
        for (const auto& matchResult : matches)
        {
            matchResults.push_back(string(matchResult));
        }
    }

    return matchResults;
}


string getKeyType(string key)
{
    regex re(R"((\w+_\d+_)(\w+))");
    smatch matches;
    if (regex_match(key, matches, re))
    {
        return matches[2];
    }
    return "";
}

void printMetadataList(GDALDataset* dataset)
{
    char** metadataList = dataset->GetMetadataDomainList();
    for (int i = 0; metadataList[i]; i++)
    {
        cout << metadataList[i] << endl;
    }
}

int main()
{
    GDALAllRegister();

    const char *pszFilename = u8"WMTS:https://geodata.nationaalgeoregister.nl/luchtfoto/rgb/wmts/1.0.0/WMTSCapabilities.xml";
    GDALDataset  *wmtsDataset = (GDALDataset *) GDALOpen( pszFilename, GA_ReadOnly );
    if( wmtsDataset == NULL )
    {
        cout << "Error occurred" << endl;
        return -1;
    }

    printMetadataList(wmtsDataset);
    char** metadata = wmtsDataset->GetMetadata("SUBDATASETS");

    vector<Dataset> datasets;
    if (metadata)
    {
        for (int i = 0; metadata[i]; i++)
        {
               vector<string> matches = splitKeyValuePair(metadata[i]);
               string key = matches[1];
               string value = matches[2];
               string keyType = getKeyType(key);
               Dataset dataset;
               if (keyType == "NAME")
               {
                   dataset.url = value;
                   datasets.push_back(dataset);
               }
               else if (keyType  == "DESC")
               {
                   Dataset& dataset = datasets.back();
                   dataset.title = value;
               }
        }

        int i = 0;
        for (auto& dataset : datasets)
        {
            cout << i << ") title=" << dataset.title << ", url=" << dataset.url << endl;
            i++;
        }

    }

    GDALDataset *poDataset = wmtsDataset;
    if (datasets.size() > 0)
    {
        string input;
        int datasetIndex = -1;
        do
        {
            cout << "Choose a dataset" << endl;
            getline(cin, input);
            datasetIndex = stoi(input);
        }
        while (datasetIndex < 0 || datasetIndex >= datasets.size());
        poDataset = (GDALDataset*)GDALOpen(datasets[datasetIndex].url.c_str(), GA_ReadOnly);
        GDALClose(wmtsDataset);
    }

    printMetadataList(poDataset);
    char** metadata2 = poDataset->GetMetadata("IMAGE_STRUCTURE");
    for (int i = 0; metadata2[i]; i++)
    {
        cout << metadata2[i] << endl;
    }

    cout << "===GeoTransform===" << endl;
    double geoTransform[6];
    poDataset->GetGeoTransform(geoTransform);
    for (int i = 0; i < 6; i++)
    {
        cout << geoTransform[i] << endl;
    }

    cout << "===GeoTransform===" << endl;



    cout << "raster count=" << poDataset->GetRasterCount() << endl;
    cout << "raster X size=" << poDataset->GetRasterXSize() << endl;
    cout << "raster Y size=" << poDataset->GetRasterYSize() << endl;
    cout << "layer count=" << poDataset->GetLayerCount() << endl;
    cout << "gcp count=" << poDataset->GetGCPCount() << endl;
    cout << "Get first raster band" << endl;
    GDALRasterBand* poBand = poDataset->GetRasterBand(1);
    cout << "poDataset = " << poDataset << "poBand = " << poBand << endl;
    
    if (!poBand)
    {
        cout << "poBand not initialized" << endl;
        return -1;
    }

    cout << "Get BlockSize" << endl;
    int nXBlockSize, nYBlockSize;
    poBand->GetBlockSize(&nXBlockSize, &nYBlockSize);

    cout << "Blocksize (" << nXBlockSize << "," << nYBlockSize << ")" << endl;

    cout << "Calculate number of blocks" << endl;
    int nXBlocks = (poDataset->GetRasterXSize() + nXBlockSize - 1) / nXBlockSize;
    int nYBlocks = (poDataset->GetRasterYSize() + nYBlockSize - 1) / nYBlockSize;
    
    cout << "blocksize (" << nXBlockSize << "," << nYBlockSize << ")";
    cout << "number of blocks (" << nXBlocks << "," << nYBlocks << ")";
    /*
    for (int y = 0; y < nYBlocks; y++)
    {
        GByte* data = (GByte*)CPLMalloc(nXBlockSize * nYBlockSize);
        for (int x = 0; x < nXBlocks; x++)
        {
            CPLErr error = poBand->ReadBlock(x, y, data);
            if (error != CPLErr::CE_None)
                CPLFree(data);
                continue;
        }
        CPLFree(data);
    }*/

    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

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

    do
    {
        // Draw nothing, see you in tutorial 2 !

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
    glfwWindowShouldClose(window) == 0 );

    GDALClose(poDataset);
    return 0;
}
