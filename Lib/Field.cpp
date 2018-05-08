#include "Field.h"

Field::Field(OGRFieldDefn* fieldDefinition)
        :   _fieldDefinition(fieldDefinition)
{
}

Field::~Field()
{
}

string Field::Name()
{
    return string(_fieldDefinition->GetNameRef());
}


