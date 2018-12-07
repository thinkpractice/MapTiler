#ifndef VALUE_H
#define VALUE_H

#include <string>

class BaseValue
{
public:
    BaseValue();
    virtual ~BaseValue();

    virtual operator int() = 0;
    virtual operator long long() = 0;
    virtual operator double() = 0;
    virtual operator std::string() = 0;

    virtual BaseValue& operator=(const int value) = 0;
    virtual BaseValue& operator=(const long long value) = 0;
    virtual BaseValue& operator=(const double value) = 0;
    virtual BaseValue& operator=(const std::string& value) = 0;
};

template <class T>
class Value : public BaseValue
{
public:
    Value(T value) : _value(value) {}
    virtual ~Value() {}

    operator T()
    {
        return _value;
    }

    Value& operator=(const T& value)
    {
        _value = value;
        return *this;
    }

private:
    T _value;
};

#endif // VALUE_H
