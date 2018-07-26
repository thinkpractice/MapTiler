#include "Feature.h"
#include <iostream>

Feature::Feature(OGRFeature* feature)
            :   _feature(feature)
{
    if (_feature)
    {
        _featureGeometry = FeatureGeometry(_feature->GetGeometryRef());
    }
}

Feature::Feature(const Feature& other)
            :   _feature(nullptr)
{
    if (other._feature)
    {
        _feature = other._feature->Clone();
        _featureGeometry = other._featureGeometry;
    }
}

Feature::~Feature()
{
    OGRFeature::DestroyFeature(_feature);
}

OGRFeature *Feature::InternalFeature() const
{
    return _feature;
}

long long Feature::FeatureId()
{
    return _feature->GetFID();
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
	if (_feature)
	{
		OGRFeature::DestroyFeature(_feature);
		_feature = nullptr;
	}
	
    if (other._feature)
    {
        _feature = other._feature->Clone();
        _featureGeometry = other._featureGeometry;
    }
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

void Feature::SetField(const Field &field)
{
   SetField(field.Name(), field.Value());
}

void Feature::SetField(std::string fieldName, string value)
{
    _feature->SetField(fieldName.c_str(), value.c_str());
}

void Feature::SetField(std::string fieldName, int value)
{
    _feature->SetField(fieldName.c_str(), value);
}

void Feature::SetField(std::string fieldName, double value)
{
    _feature->SetField(fieldName.c_str(), value);
}

Feature::FeatureGeometry::FeatureGeometry()
                            :   FeatureGeometry(nullptr)
{
}

Feature::FeatureGeometry::FeatureGeometry(OGRGeometry* geometry)
                            :   _geometry(geometry),
                                _geometryType(Other),
                                _hasPoint(false),
                                _hasPolygon(false),
                                _hasMultiPolygon(false)
{
    if (_geometry)
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
        _polygon.FromGdal(geometry);
        _hasPolygon = true;
    }
    else if (Type() == MultiPolygonType)
    {
        _multiPolygon.FromGdal(geometry);
        _hasMultiPolygon = true;
    }
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

void Feature::FeatureGeometry::MapGeometry(shared_ptr<CoordinateTransformation> transformation, AffineTransform affineTransform)
{
    MapGeometry(transformation);
    if (HasPoint())
    {
        _point = affineTransform.ReverseTransform(_point);
    }
    else if (HasPolygon())
    {
        _polygon = affineTransform.ReverseTransform(_polygon);
    }
    else if (HasMultiPolygon())
    {
        _multiPolygon = affineTransform.ReverseTransform(_multiPolygon);
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

Feature::FeatureGeometry& Feature::Geometry()
{
    return _featureGeometry;
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
