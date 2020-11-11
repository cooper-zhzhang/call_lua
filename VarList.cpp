#include "VarList.h"

VarData::VarData()
{
    m_Type = VAR_TYPE_UNKNOWN;
    m_Data.int64Value = 0;
}

VarData::~VarData()
{
    TryDeleteStr();
}

bool VarData::SetInt(int value)
{
    TryDeleteStr();

    m_Type = VAR_TYPE_INT;
    m_Data.intValue = value;
    return true;
}

bool VarData::SetInt64(int64_t value)
{
    TryDeleteStr();

    m_Type = VAR_TYPE_INT64;
    m_Data.int64Value = value;
    return true;
}

bool VarData::SetStr(const std::string &str)
{
    m_Type = VAR_TYPE_STRING;
    if (m_Data.pStrValue == nullptr)
    {
        m_Data.pStrValue = new std::string(str);
    }

    *(m_Data.pStrValue) = str;

    return true;
}

bool VarData::SetFloat(float value)
{
    TryDeleteStr();

    m_Type = VAR_TYPE_FLOAT;
    m_Data.floatValue = value;

    return true;
}

bool VarData::SetDouble(double value)
{
    TryDeleteStr();

    m_Type = VAR_TYPE_DOUBLE;
    m_Data.doubleValue = value;

    return true;
}

void VarData::TryDeleteStr()
{
    if (m_Type == VAR_TYPE_STRING && m_Data.pStrValue != nullptr)
    {
        delete m_Data.pStrValue;

        m_Data.pStrValue = nullptr;
        m_Type = VAR_TYPE_UNKNOWN;
    }
}

int VarData::IntValue() const
{
    switch (m_Type)
    {
    case VAR_TYPE_INT:
        return m_Data.intValue;
    case VAR_TYPE_INT64:
        return (int)m_Data.int64Value;
    case VAR_TYPE_FLOAT:
        return (int)m_Data.floatValue;
    case VAR_TYPE_DOUBLE:
        return (int)m_Data.doubleValue;
    case VAR_TYPE_STRING:
        return std::stoi(*(m_Data.pStrValue));
    default:
        break;
    }

    return 0;
}

int64_t VarData::Int64Value() const
{
    switch (m_Type)
    {
    case VAR_TYPE_INT:
        return m_Data.intValue;
    case VAR_TYPE_INT64:
        return m_Data.int64Value;
    case VAR_TYPE_FLOAT:
        return (int64_t)m_Data.floatValue;
    case VAR_TYPE_DOUBLE:
        return (int64_t)m_Data.doubleValue;
    case VAR_TYPE_STRING:
        return std::stoll(*(m_Data.pStrValue));
    default:
        break;
    }
    
    return 0;
}

float VarData::FloatValue() const
{
    switch (m_Type)
    {
    case VAR_TYPE_INT:
        return (float)m_Data.intValue;
    case VAR_TYPE_INT64:
        return (float)m_Data.int64Value;
    case VAR_TYPE_FLOAT:
        return m_Data.floatValue;
    case VAR_TYPE_DOUBLE:
        return (float)m_Data.doubleValue;
    case VAR_TYPE_STRING:
        return std::stof(*(m_Data.pStrValue));
    default:
        break;
    }

    return 0.0f;
}

double VarData::DoubleValue() const
{
    switch (m_Type)
    {
    case VAR_TYPE_INT:
        return (double)m_Data.intValue;
    case VAR_TYPE_INT64:
        return (double)m_Data.int64Value;
    case VAR_TYPE_FLOAT:
        return m_Data.floatValue;
    case VAR_TYPE_DOUBLE:
        return m_Data.doubleValue;
    case VAR_TYPE_STRING:
        return std::stod(*(m_Data.pStrValue));
    default:
        break;
    }
    return 0.0;
}

const std::string &VarData::StrValue() const
{
    if (m_Type == VAR_TYPE_STRING)
    {
        return *m_Data.pStrValue;
    }
    
    return EMPTY_STR;
}

VAR_TYPE VarData::GetType() const
{
    return m_Type;
}
