#ifndef TILEWRITERSTEP_H
#define TILEWRITERSTEP_H

#include <string>
#include <memory>
#include "ProcessingStep.h"
#include "GeoTile.h"

class TileWriterStep : public ProcessingStep
{
    public:
        TileWriterStep(std::string tileDirectory, std::string persistenceUrl);
        virtual ~TileWriterStep();

        void Run();
        void SaveTile(std::shared_ptr<GeoTile> tile, string tileFilename);

    private:
        string _tileDirectory;
        string _persistenceUrl;
        int _numberOfTilesWritten;
};

#endif /* TILEWRITERSTEP_H */
