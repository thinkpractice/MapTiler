#include "Feature.h"
#include <iostream>

Feature::Feature(OGRFeature* feature)
            :   _feature(feature)
{
}

Feature::Feature(const Feature& other)
            :   _feature(nullptr)
{
    if (other._feature)
        _feature = other._feature->Clone();
}

Feature::~Feature()
{
    OGRFeature::DestroyFeature(_feature);
}

string Feature::Name() const
{
    return string(FeatureDefinition()->GetName());
}

size_t Feature::NumberOfFields() const
{
    return FeatureDefinition()->GetFieldCount();
}

Feature& Feature::operator=(const Feature& other)
{
    _feature = nullptr;
    if (other._feature)
        _feature = other._feature->Clone();
    return *this;
}

Field Feature::operator[](size_t index)
{
    return GetFieldAtIndex(index);
}

const Field Feature::operator[](size_t index) const
{
    return GetFieldAtIndex(index);
}

bool Feature::operator==(const Feature& other) const
{
    if (_feature == nullptr && other._feature == nullptr)
        return true;
    _feature == other._feature;
}

Feature::FeatureGeometry::FeatureGeometry(OGRGeometry* geometry)
                            :   _geometry(geometry),
                                _geometryType(Other),
                                _hasPoint(false),
                                _hasPolygon(false),
                                _hasMultiPolygon(false)
{
    ParseGeometry(geometry);
}

Feature::FeatureGeometry::~FeatureGeometry()
{
}

Feature::FeatureGeometry::GeometryType Feature::FeatureGeometry::Type()
{
    return _geometryType;
}

void Feature::FeatureGeometry::ParseGeometry(OGRGeometry *geometry)
{
    _parsedGeometry = true;
    _geometryType = ParseGeometryType(geometry);
    if (geometry == nullptr || Type() == GeometryType::Other)
        return;
    
    if (Type() == PointType)
    {
        OGRPoint *poPoint = (OGRPoint*)geometry;
        _point = Point(poPoint->getX(), poPoint->getY());
        _hasPoint = true;
    }
    else if (Type() == PolygonType)
    {
        OGRPolygon* polygon = (OGRPolygon*)geometry;
        _polygon = ParsePolygon(polygon);
        _hasPolygon = true;
    }
    else if (Type() == MultiPolygonType)
    {
        OGRMultiPolygon* multiPolygon = (OGRMultiPolygon*)geometry;
        for (int i = 0; i < multiPolygon->getNumGeometries(); i++)
        {
            OGRPolygon* ogrPolygon = (OGRPolygon*)multiPolygon->getGeometryRef(i);
            Polygon polygon = ParsePolygon(ogrPolygon);
            _multiPolygon.AddPolygon(polygon);
        }
        _hasMultiPolygon = true;
    }
}

Polygon Feature::FeatureGeometry::ParsePolygon(OGRPolygon* ogrPolygon)
{
    //TODO Refactor to prevent code duplication
    Polygon polygon;

    OGRLinearRing* ring = ogrPolygon->getExteriorRing();
    for (int i = 0; i < ring->getNumPoints(); i++)
    {
        OGRPoint point;
        ring->getPoint(i, &point);

        polygon.ExternalRing().AddPoint(Point(point.getX(), point.getY()));
    }

    for (int i = 0; i < ogrPolygon->getNumInteriorRings(); i++)
    {
        OGRLinearRing* internalRing = ogrPolygon->getInteriorRing(i);
        Polygon::Ring newInternalRing;
        for (int i = 0; i < internalRing->getNumPoints(); i++)
        {
            OGRPoint point;
            ring->getPoint(i, &point);
            newInternalRing.AddPoint(Point(point.getX(), point.getY()));
        }
        polygon.InternalRings().push_back(newInternalRing);
    }
    return polygon;
}

Feature::FeatureGeometry::GeometryType Feature::FeatureGeometry::ParseGeometryType(OGRGeometry *geometry)
{
    switch(wkbFlatten(geometry->getGeometryType()))
    {
        case wkbPoint:
            return GeometryType::PointType;
        case wkbPolygon:
            return GeometryType::PolygonType;
        case wkbMultiPolygon:
            return GeometryType::MultiPolygonType;
    }
    return GeometryType::Other;
}


bool Feature::FeatureGeometry::HasPoint() const
{
    return _hasPoint;
}

Point Feature::FeatureGeometry::GetPoint() const
{
    return _point;
}

bool Feature::FeatureGeometry::HasPolygon() const
{
    return _hasPolygon;
}

Polygon Feature::FeatureGeometry::GetPolygon() const
{
    return _polygon;
}

bool Feature::FeatureGeometry::HasMultiPolygon() const
{
    return _hasMultiPolygon;
}

MultiPolygon Feature::FeatureGeometry::GetMultiPolygon() const
{
    return _multiPolygon;
}

void Feature::FeatureGeometry::MapGeometry(shared_ptr<CoordinateTransformation> transformation)
{
    if (HasPoint())
    {
        _point = transformation->MapCoordinate(_point);
    }
    else if (HasPolygon())
    {
        _polygon = transformation->MapPolygon(_polygon);
    }
    else if (HasMultiPolygon())
    {
        _multiPolygon = transformation->MapMultiPolygon(_multiPolygon);
    }
}

Feature::FieldIterator::FieldIterator(const Feature* owner, bool start)
                            :   _owner(owner),
                                _currentField(nullptr, ""),
                                _currentIndex(-1)
{
    if (start)
    {
        _currentIndex = 0;
        NextField();
    }
}

Feature::FieldIterator::FieldIterator(const FieldIterator& iterator)
                            :   _owner(iterator._owner),
                                _currentField(iterator._currentField),
                                _currentIndex(iterator._currentIndex)
{
}

Feature::FieldIterator::~FieldIterator()
{
}

Feature::FieldIterator& Feature::FieldIterator::operator=(const FieldIterator& iterator)
{
    _owner = iterator._owner;
    _currentField = iterator._currentField;
    _currentIndex = iterator._currentIndex;
    return (*this);
}

Feature::FieldIterator::reference Feature::FieldIterator::operator*() const
{
    return _currentField;
}

Feature::FieldIterator& Feature::FieldIterator::operator++()
{
    NextField();
    return *this;
}

Feature::FieldIterator Feature::FieldIterator::operator++(int)
{
    Feature::FieldIterator temp = *this;
    ++(*this);
    return temp;
}

bool Feature::FieldIterator::operator==(const FieldIterator& rhs)
{
    return _currentIndex == rhs._currentIndex;
}

bool Feature::FieldIterator::operator!=(const FieldIterator& rhs)
{
    return !(*this == rhs);
}

void Feature::FieldIterator::NextField()
{
    if (_currentIndex >= _owner->NumberOfFields())
    {
        _currentIndex = -1;
        return;
    }
    _currentField = (*_owner)[_currentIndex];
    _currentIndex++;
}

Feature::iterator Feature::begin() const
{
    return {this, true};
}

Feature::iterator Feature::end() const
{
    return {this, false};
}

Feature::FeatureGeometry Feature::Geometry()
{
    OGRGeometry *geometry = _feature->GetGeometryRef();
    return FeatureGeometry(geometry);
}

OGRFeatureDefn* Feature::FeatureDefinition() const
{
    return _feature->GetDefnRef();
}

Field Feature::GetFieldAtIndex(size_t index) const
{
    OGRFieldDefn* fieldDefinition = FeatureDefinition()->GetFieldDefn(index);
    auto value = _feature->GetFieldAsString(index);
    /*if(fieldDefinition->GetType() == OFTInteger)
        value = _feature->GetFieldAsInteger(index);
    else if(fieldDefinition->GetType() == OFTInteger64 )
        value = _feature->GetFieldAsInteger64(index);
    else if(fieldDefinition->GetType() == OFTReal )
        value = _feature->GetFieldAsDouble(index); */
    return Field(fieldDefinition, string(value));
}
