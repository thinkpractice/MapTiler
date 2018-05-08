#ifndef FEATURE_H
#define FEATURE_H

#include "ogrsf_frmts.h"
#include <string>
#include <iterator>

using namespace std;

class Feature
{
    public:
        Feature(OGRFeature *feature);
        virtual ~Feature();

        string Name();

        class FieldIterator
        {
            public:
                FieldIterator(const Feature& owner, bool start);
                FieldIterator(const FieldIterator& iterator);
                virtual ~FieldIterator();

                FieldIterator& operator=(const FieldIterator& iterator);

                reference operator*() const;

                FieldIterator& operator++();
                FieldIterator operator++(int);

                bool operator==(const FieldIterator& rhs);
                bool operator!=(const FieldIterator& rhs);


        };

        using iterator = FieldIterator;

        iterator begin();
        iterator end();
        
    private:
        OGRFeatureDefn *FeatureDefinition();

    private:
        OGRFeature* _feature;

};

#endif
