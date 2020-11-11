#pragma once

#include <cinttypes>
#include <string>

enum VAR_TYPE
{
    VAR_TYPE_UNKNOWN,  
    VAR_TYPE_INT,      
    VAR_TYPE_INT64,
    VAR_TYPE_FLOAT,    
    VAR_TYPE_DOUBLE,
    VAR_TYPE_STRING,
};

class IVarList
{
public:

    IVarList(){};
    virtual ~IVarList(){};

    virtual bool AddInt(int value) = 0;
    virtual IVarList& operator <<(int value)
     {

         AddInt(value);
         return *this;
     }

    virtual bool AddInt64(int64_t value) = 0;
    virtual IVarList& operator<<(int64_t value)
    {
        AddInt64(value);
        return *this;
    }

    virtual bool AddStr(const std::string &str) = 0;
    virtual IVarList& operator<<(const std::string &str)
    {
        AddStr(str);
        return *this;
    }

    virtual bool AddFloat(float value) = 0;
    virtual IVarList& operator<<(float value)
    {
        AddFloat(value);
        return *this;
    }

    virtual bool AddDouble(double value) = 0;
    virtual IVarList& operator<<(double value)
    {
        AddDouble(value);
        return *this;
    }

    virtual int IntValue(int index) const = 0;
    virtual int64_t Int64Value(int index) const = 0;
    virtual const std::string& StrValue(int index) const = 0;
    virtual float FloatValue(int index) const = 0;
    virtual double DoubleValue(int index) const = 0;
    virtual VAR_TYPE GetType(int index) const = 0;
    virtual bool IsEmpty() const = 0;
    virtual size_t GetCount() const = 0;
    virtual void Clear() = 0;
};
