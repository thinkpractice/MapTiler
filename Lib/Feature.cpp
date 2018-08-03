#include <iostream>
#include "Feature.h"

Feature::Feature(OGRFeature* feature)
            :   _feature(feature)
{
    if (_feature)
    {
        OGRGeometry* geometry = _feature->GetGeometryRef();
        _innerGeometry = Geometry::NewGeometry(geometry);
    }
}

Feature::Feature(const Feature& other)
            :   _feature(nullptr)
{
    if (other._feature)
    {
        _feature = other._feature->Clone();
        _innerGeometry = other._innerGeometry;
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

int Feature::NumberOfFields() const
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
        _innerGeometry = other._innerGeometry;
    }
    return *this;
}

Field Feature::operator[](size_t index)
{
    return GetFieldAtIndex(static_cast<int>(index));
}

const Field Feature::operator[](size_t index) const
{
    return GetFieldAtIndex(static_cast<int>(index));
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

std::shared_ptr<Geometry> Feature::GetGeometry() const
{
    return _innerGeometry;
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
    _currentField = (*_owner)[static_cast<size_t>(_currentIndex)];
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


Field Feature::GetFieldWithName(const char *name) const
{
    int index = FeatureDefinition()->GetFieldIndex(name);
    return GetFieldAtIndex(index);
}

Field Feature::GetFieldAtIndex(int index) const
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

