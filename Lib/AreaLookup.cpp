#include "AreaLookup.h"
#include <QGeoAddress>
#include <iostream>
#include <vector>
#include <algorithm>

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

Point AreaLookup::PointForGeoCoordinate(const QGeoCoordinate& coordinate)
{
    return Point(coordinate.latitude(), coordinate.longitude());
}

Area AreaLookup::AreaForGeoRectangle(const QGeoRectangle& geoRectangle)
{
    SpatialReference gpsReference;
    gpsReference.SetWellKnownGeogCS("EPSG:4326");

    Point leftTop = PointForGeoCoordinate(geoRectangle.topLeft());
    Point bottomRight = PointForGeoCoordinate(geoRectangle.bottomRight());

    return Area(gpsReference, leftTop, bottomRight);
}

void AreaLookup::GeoLocationFound(QGeoCodeReply* reply)
{
    cout << reply->locations().size() << "error=" << reply->errorString().toStdString() << endl;
    for (auto location : reply->locations())
    {
        QGeoRectangle boundingBox = location.boundingBox();
        QGeoCoordinate topLeft = boundingBox.topLeft();
        QGeoCoordinate bottomRight = boundingBox.bottomRight();


        QString locationString = QString("%1 - (%2, %3, %4, %5)").arg(location.address().text()).arg(topLeft.latitude()).arg(topLeft.longitude()).arg(bottomRight.latitude()).arg(bottomRight.longitude());
        cout << "location found:" << locationString.toStdString().c_str() << endl;
        
        Area area = AreaForGeoRectangle(location.boundingBox());
    }
}

void AreaLookup::Error(QGeoCodeReply *reply, QGeoCodeReply::Error error, QString errorString)
{
    cout << "error=" << errorString.toStdString() << endl;
}
