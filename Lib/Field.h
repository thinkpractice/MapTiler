#ifndef FIELD_H
#define FIELD_H

#include "ogrsf_frmts.h"
#include <string>

using namespace std;

class Field
{
    public:
        Field(OGRFieldDefn* fieldDefinition);
        virtual ~Field();

        string Name();

    private:
        OGRFieldDefn* _fieldDefinition;
};

#endif
