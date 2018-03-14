#include "AreaLookup.h"
#include <QGeoCoordinate>
#include <QGeoAddress>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Point.h"

using namespace std;

AreaLookup::AreaLookup()
                :   _geoServiceProvider(nullptr),
                    _geoCodingManager(nullptr)
{
    SetGeoServiceProvider("osm");
}

AreaLookup::~AreaLookup()
{
    delete _geoServiceProvider;
}

vector<string> AreaLookup::ServiceProviders()
{
    if (_serviceProviders.size() > 0)
        return _serviceProviders;

    for (QString entry : QGeoServiceProvider::availableServiceProviders())
    {
        QGeoServiceProvider provider(entry);
        if (provider.geocodingFeatures().testFlag(QGeoServiceProvider::NoGeocodingFeatures))
            continue;
        _serviceProviders.push_back(entry.toUtf8().constData());
    }
    return _serviceProviders;
}

void AreaLookup::SetGeoServiceProvider(string serviceProvider)
{
    if (std::find(ServiceProviders().begin(), ServiceProviders().end(), serviceProvider) == ServiceProviders().end())
        return;

    QString providerName(serviceProvider.c_str());
    SetGeoServiceProvider(new QGeoServiceProvider(providerName));
}

void AreaLookup::SetGeoServiceProvider(QGeoServiceProvider* serviceProvider)
{
    if (_geoServiceProvider)
        delete _geoServiceProvider;
    _geoServiceProvider = serviceProvider;
    _geoCodingManager = _geoServiceProvider->geocodingManager();
    connect(_geoCodingManager, SIGNAL(error(QGeoCodeReply*,QGeoCodeReply::Error,QString)), this, SLOT(Error(QGeoCodeReply*,QGeoCodeReply::Error,QString)));
    connect(_geoCodingManager, SIGNAL(finished(QGeoCodeReply*)), this, SLOT(GeoLocationFound(QGeoCodeReply*)));
}

Area AreaLookup::GetAreaForAddress(string address)
{
    SpatialReference gpsReference;
    gpsReference.SetWellKnownGeogCS("EPSG:4326");

    QString qAddress(address.c_str());
    QGeoCodeReply* reply = _geoCodingManager->geocode(qAddress);
    return Area(gpsReference, Point(6.010840, 50.8903150), Point(6.0166710, 50.8901200));
}

void AreaLookup::GeoLocationFound(QGeoCodeReply* reply)
{
    cout << reply->locations().size() << "error=" << reply->errorString().toStdString() << endl;
    for (auto location : reply->locations())
    {
        QString locationString = QString("%1 - (%2, %3)").arg(location.address().text()).arg(location.coordinate().latitude()).arg(location.coordinate().longitude());
        cout << "location found:" << locationString.toStdString().c_str() << endl;

        //QGeoCoordinate coordinate = location.coordinate();
        //QPoint p (coordinate.latitude(), coordinate.longitude());

    }
}

void AreaLookup::Error(QGeoCodeReply *reply, QGeoCodeReply::Error error, QString errorString)
{
    cout << "error=" << errorString.toStdString() << endl;
}
