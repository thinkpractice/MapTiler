#include "Field.h"

Field::Field(OGRFieldDefn* fieldDefinition, string value)
        :   _fieldDefinition(fieldDefinition),
            _value(value)
{
}

Field::~Field()
{
}

string Field::Name() const
{
    return string(_fieldDefinition->GetNameRef());
}

//TODO: make value return value with true value type of field
string Field::Value() const
{
    return _value;
}
