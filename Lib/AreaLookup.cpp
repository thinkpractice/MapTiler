#include "AreaLookup.h"
#include <QGeoCoordinate>
#include <iostream>

using namespace std;

AreaLookup::AreaLookup()
{
    for (auto& providerString : QGeoServiceProvider::availableServiceProviders())
    {
        cout << providerString.toUtf8().constData() << endl;
    }
}

Area AreaLookup::GetAreaForAddress(string address)
{
}
