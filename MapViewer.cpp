#include <QApplication>
#include <iostream>
#include <iomanip>
#include <regex>
#include <vector>
#include <string>
#include <chrono>
#include <atomic>
#include "Lib/GLWindow.h"
#include "Lib/Menu.h"
#include "Lib/GeoMapProvider.h"
#include "Lib/Area.h"
#include "Lib/SpatialReference.h"
#include "Lib/Point.h"
#include "Lib/TileWriter.h"
#include "Lib/AreaLookup.h"
#include "Lib/ThreadPool.h"
#include "Lib/SafeQueue.h"

using namespace std;

void WriteTile(GeoTile* tile, string tileDirectory, int currentIndex, int maxIndex)
{
    TileWriter tileWriter;
    double onePercent = maxIndex / 100.0;
    double totalNumberOfBytes = currentIndex * tile->NumberOfBytes();
    if (currentIndex % (int)onePercent == 0)
    {
        //auto t2 = std::chrono::high_resolution_clock::now();
        //auto duration = std::chrono::duration_cast<std::chrono::seconds>(t2-t1).count();
        int imagePercentage = (currentIndex / onePercent) + 1;
        cout << "Wrote " << imagePercentage << "\% out of " << maxIndex << " images(" << currentIndex << " Images, " << fixed << setprecision(2) << (totalNumberOfBytes / 1024) << "KB in " << endl; // duration << " seconds)" << endl;
        
    }
    string tileFilename = tileDirectory + to_string(currentIndex) + ".png" ;
    tileWriter.Save(tile, tileFilename);
    delete tile;
}

template <class T>
vector<T> subVector(vector<T> originalVector, int startIndex, int numberOfItems)
{
    cout << "Copying from: " << startIndex << " to: " << startIndex + numberOfItems - 1 <<endl;
        typename vector<T>::const_iterator first = originalVector.begin() + startIndex;
        typename vector<T>::const_iterator last = originalVector.begin() + startIndex + numberOfItems - 1;
        vector<T> newVec(first, last);
        return newVec;
}

void DownloadTilesForArea(GeoMap* chosenMap, const Area& area, string tileDirectory)
{
    vector<Rect> tileRects = chosenMap->GetTileRectsForArea(area);

    SafeQueue<GeoMap*> mapsPerThread;
    {
        auto t1 = std::chrono::high_resolution_clock::now();

        int numberOfThreads = 4;
        ThreadPool threadPool(numberOfThreads);
        
        mapsPerThread.enqueue(chosenMap);

        for (int i = 0; i < numberOfThreads-1; i++)
        {
            mapsPerThread.enqueue(chosenMap->Clone());
        }

        atomic<int> currentIndex (0);
        for (auto& tileRect : tileRects)
        {
            auto result = threadPool.enqueue([&, tileRect]{
                        GeoMap* map = mapsPerThread.dequeue();
                        try
                        {
                            GeoTile* tile = map->GetTileForRect(tileRect);
                            WriteTile(tile, tileDirectory, currentIndex, tileRects.size());

                            currentIndex += 1;
                        }
                        catch (...)
                        {
                        }
                        mapsPerThread.enqueue(map);
                    });
        }
    }

    while (!mapsPerThread.Empty())
    {
        GeoMap* map = mapsPerThread.dequeue();
        delete map;
    }
}



int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    string tileDirectory = "/media/tim/Data/Work/CBS/Tiles/tile";
    if (argc > 1)
    {
        tileDirectory = string(argv[1]);
        cout << "output directory: " << tileDirectory << endl;
    }

    string filename = u8"WMTS:https://geodata.nationaalgeoregister.nl/luchtfoto/rgb/wmts/1.0.0/WMTSCapabilities.xml";
    GeoMapProvider mapProvider(filename);

    GeoMap* chosenMap = Menu<GeoMap*>::ShowMenu(mapProvider.Maps(), [&](int i, GeoMap* dataset){
                string menuItem = to_string(i) + ") title=" + dataset->Title() + ", url=" + dataset->Filename() + "\n";
                return menuItem;
            });

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
    
    Area mapArea = chosenMap->GetMapArea();
    cout << "MapArea(" << mapArea.LeftTop().X << "," << mapArea.LeftTop().Y << "," << mapArea.BottomRight().X << "," << mapArea.BottomRight().Y << ")" << endl;

    AreaLookup areaLookup;
    areaLookup.AddListener([&](vector<Area> areas){

                if (areas.size() > 0)
                {
                    Area chosenArea = Menu<Area>::ShowMenu(areas, [](int i, Area area)
                            {
                                return to_string(i) + ") " + area.Description();
                            });
                    DownloadTilesForArea(chosenMap, chosenArea, tileDirectory);
                }
            });
    for (auto& serviceProvider : areaLookup.ServiceProviders())
    {
        std::cout << serviceProvider << std::endl;
    }
    //areaLookup.GetAreaForAddress("Landgraaf");
    areaLookup.GetAreaForAddress("Heerlen");

    /*GLWindow window(1024, 768);
    window.StartRendering([&](GLFWwindow* window)
    {
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
            //glBindTexture( GL_TEXTURE_2D, textureID);
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
    });*/

    /*cout << "Calculate number of blocks" << endl;
    
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
    
    */

    //getline(cin, input);
    return app.exec();
}
