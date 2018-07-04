#ifndef TILEWRITERSTEP_H
#define TILEWRITERSTEP_H

#include "ProcessingStep.h"
#include "GeoTile.h"

class TileWriterStep : public ProcessingStep
{
    public:
        TileWriterStep(string tileDirectory);
        virtual ~TileWriterStep();

        void Run();
        void SaveTile(shared_ptr<GeoTile> tile, string tileFilename);

    private:
        string _tileDirectory;
        int _numberOfTilesWritten;
};

#endif /* TILEWRITERSTEP_H */
