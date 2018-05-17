#ifndef TILEWRITERSTEP_H
#define TILEWRITERSTEP_H

#include "ProcessingStep.h"

class TileWriterStep : public ProcessingStep
{
    public:
        TileWriterStep(string tileDirectory);
        virtual ~TileWriterStep();

        void ProcessTile(GeoTile* tile, Rect tileRect);

    private:
        string _tileDirectory;
};

#endif /* TILEWRITERSTEP_H */
