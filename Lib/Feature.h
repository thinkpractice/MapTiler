#ifndef FEATURE_H
#define FEATURE_H

#include "ogrsf_frmts.h"
#include <string>
#include <iterator>
#include "Field.h"

using namespace std;

class Feature
{
    public:
        Feature(OGRFeature *feature);
        virtual ~Feature();

        string Name() const;
        int NumberOfFields() const;

        Field operator[](size_t index);
        const Field operator[](size_t index) const;

        class FieldIterator
        {
            public:
                using value_type = Field;
                using difference_type = ptrdiff_t;
                using pointer = Field*;
                using reference = const Field&;
                using iterator_category = input_iterator_tag;

                FieldIterator(const Feature* owner, bool start);
                FieldIterator(const FieldIterator& iterator);
                virtual ~FieldIterator();

                FieldIterator& operator=(const FieldIterator& iterator);

                reference operator*() const;

                FieldIterator& operator++();
                FieldIterator operator++(int);

                bool operator==(const FieldIterator& rhs);
                bool operator!=(const FieldIterator& rhs);

            private:
                void NextField();

            private:
                const Feature* _owner;
                size_t _currentIndex;
                Field _currentField;
        };

        using iterator = FieldIterator;

        iterator begin() const;
        iterator end() const;
        
    private:
        OGRFeatureDefn *FeatureDefinition() const;

    private:
        OGRFeature* _feature;

};

#endif
