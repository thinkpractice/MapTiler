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
        void SaveTile(GeoTile* tile);

    private:
        string _tileDirectory;
};

#endif /* TILEWRITERSTEP_H */
