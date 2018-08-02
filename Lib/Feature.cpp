#include <iostream>
#include "Feature.h"

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

long long Feature::FeatureId() const
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

Field Feature::operator[](const char *fieldName)
{
    return GetFieldWithName(fieldName);
}

const Field Feature::operator[](const char *fieldName) const
{
    return GetFieldWithName(fieldName);
}

bool Feature::operator==(const Feature& other) const
{
    if (_feature == nullptr && other._feature == nullptr)
        return true;
    return _feature == other._feature;
}

void Feature::SetField(const Field &field)
{
    SetField(field.Name(), field.Value());
}

void Feature::SetField(string fieldName, const Field &field)
{
    SetField(fieldName, field.Value());
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
                                _geometryType(Other)
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
        _innerGeometry = make_shared<Point>(geometry);
    }
    else if (Type() == PolygonType)
    {
        _innerGeometry = make_shared<Polygon>(geometry);
    }
    else if (Type() == MultiPolygonType)
    {
        _innerGeometry = make_shared<MultiPolygon>(geometry);
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
    return _innerGeometry->GetType() == Geometry::PointType;
}

bool Feature::FeatureGeometry::HasPolygon() const
{
    return _innerGeometry->GetType() == Geometry::PolygonType;
}

bool Feature::FeatureGeometry::HasMultiPolygon() const
{
    return _innerGeometry->GetType() == Geometry::MultiPolygonType;
}

std::shared_ptr<Geometry> Feature::FeatureGeometry::InnerGeometry()
{
    return _innerGeometry;
}

void Feature::FeatureGeometry::MapGeometry(shared_ptr<CoordinateTransformation> transformation)
{
    _innerGeometry = transformation->MapGeometry(_innerGeometry);
}

void Feature::FeatureGeometry::MapGeometry(shared_ptr<CoordinateTransformation> transformation, AffineTransform affineTransform)
{
    MapGeometry(transformation);
    if (HasPoint())
    {
        _innerGeometry = affineTransform.ReverseTransform(_innerGeometry);
    }
    else if (HasPolygon())
    {
        _innerGeometry = affineTransform.ReverseTransform(_innerGeometry);
    }
    else if (HasMultiPolygon())
    {
        _innerGeometry = affineTransform.ReverseTransform(_innerGeometry);
    }
}

Feature::FieldIterator::FieldIterator(const Feature* owner, bool start)
                            :   _owner(owner),
                                _currentIndex(-1),
                                _currentField(nullptr, "")
{
    if (start)
    {
        _currentIndex = 0;
        NextField();
    }
}

Feature::FieldIterator::FieldIterator(const FieldIterator& iterator)
                            :   _owner(iterator._owner),
                                _currentIndex(iterator._currentIndex),
                                _currentField(iterator._currentField)
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

Feature::FeatureGeometry& Feature::GetGeometry()
{
    return _featureGeometry;
}

Field Feature::GetFieldWithName(const char *name) const
{
    size_t index = FeatureDefinition()->GetFieldIndex(name);
    return GetFieldAtIndex(index);
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

OGRFeatureDefn* Feature::FeatureDefinition() const
{
    return _feature->GetDefnRef();
}

