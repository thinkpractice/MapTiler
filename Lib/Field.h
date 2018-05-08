#ifndef FIELD_H
#define FIELD_H

#include "ogrsf_frmts.h"
#include <string>

using namespace std;

class Field
{
    public:
        Field(OGRFieldDefn* fieldDefinition,string value);
        virtual ~Field();

        string Name();
        string Value();

    private:
        OGRFieldDefn* _fieldDefinition;
        string _value;
};

#endif
