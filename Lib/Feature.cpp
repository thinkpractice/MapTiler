#include "Feature.h"

Feature::Feature(OGRFeature* feature)
            :   _feature(feature)
{
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

Field Feature::operator[](size_t index)
{
    return GetFieldAtIndex(index);
}

const Field Feature::operator[](size_t index) const
{
    return GetFieldAtIndex(index);
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
