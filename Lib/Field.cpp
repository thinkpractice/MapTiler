#include "Field.h"

Field::Field(OGRFieldDefn* fieldDefinition, string value)
        :   _fieldDefinition(fieldDefinition),
            _value(value)
{
}

Field::~Field()
{
}

string Field::Name()
{
    return string(_fieldDefinition->GetNameRef());
}

string Field::Value()
{
    return _value;
}
