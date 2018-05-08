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

int Feature::NumberOfFields() const
{
    return FeatureDefinition()->GetFieldCount();
}

Field Feature::operator[](size_t index)
{
    return Field(FeatureDefinition()->GetFieldDefn(index));
}

const Field Feature::operator[](size_t index) const
{
    return Field(FeatureDefinition()->GetFieldDefn(index));
}

Feature::FieldIterator::FieldIterator(const Feature* owner, bool start)
                            :   _owner(owner),
                                _currentField(nullptr),
                                _currentIndex(-1)
{
    if (start)
    {
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
    (*this)++;
    return temp;
}

bool Feature::FieldIterator::operator==(const FieldIterator& rhs)
{
    return _owner == rhs._owner && _currentIndex == rhs._currentIndex;
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
    _currentIndex++;
    _currentField = (*_owner)[_currentIndex];
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

