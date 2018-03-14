#ifndef AREA_LOOKUP_H
#define AREA_LOOKUP_H

#include <string>
#include <vector>
#include "Area.h"

#include <QObject>
#include <QGeoServiceProvider>
#include <QGeoCodingManager>
#include <QGeoCodeReply>

class AreaLookup : public QObject
{
    public:
        AreaLookup();

        Area GetAreaForAddress(string address);
        vector<string> ServiceProviders();

    private:
        void SetGeoServiceProvider(QGeoServiceProvider* serviceProvider);

    private slots:
        void GeoLocationFound(QGeoCodeReply* reply);
        void Error(QGeoCodeReply *reply, QGeoCodeReply::Error error, QString errorString);

    private:
        vector<string> _serviceProviders;
        QGeoServiceProvider* _geoServiceProvider;
        QGeoCodingManager* _geoCodingManager;
};

#endif
