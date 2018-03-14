#include "AreaLookup.h"
#include <QGeoCoordinate>
#include <QGeoAddress>
#include <iostream>
#include <vector>
#include "Point.h"

using namespace std;

AreaLookup::AreaLookup()
{
    for (auto& providerString : QGeoServiceProvider::availableServiceProviders())
    {
        cout << providerString.toUtf8().constData() << endl;
    }
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
