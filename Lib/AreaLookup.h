#ifndef AREA_LOOKUP_H
#define AREA_LOOKUP_H

#include <string>
#include <vector>
#include "Point.h"
#include "Area.h"

#include <QObject>
#include <QGeoServiceProvider>
#include <QGeoCodingManager>
#include <QGeoCodeReply>
#include <QGeoCoordinate>
#include <QGeoRectangle>

typedef std::function<void(vector<Area>)> CallbackFunction;

class AreaLookup : public QObject
{
    Q_OBJECT

    public:
        AreaLookup();
        virtual ~AreaLookup();

        void AddListener(CallbackFunction callback);

        vector<string> ServiceProviders();
        void SetGeoServiceProvider(string serviceProvider);

        void GetAreaForAddress(string address);

    private:
        void SetGeoServiceProvider(QGeoServiceProvider* serviceProvider);
        Point PointForGeoCoordinate(const QGeoCoordinate& coordinate);
        Area AreaForGeoRectangle(const QGeoRectangle& geoRectangle);

    public slots:
        void GeoLocationFound(QGeoCodeReply* reply);
        void Error(QGeoCodeReply *reply, QGeoCodeReply::Error error, QString errorString);

    private:
        vector<string> _serviceProviders;
        QGeoServiceProvider* _geoServiceProvider;
        QGeoCodingManager* _geoCodingManager;
        vector<CallbackFunction> _callbackFunctions;
};

#endif
