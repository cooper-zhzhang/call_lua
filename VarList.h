#pragma once

#include "IVarList.h"
#include <string>
#include <vector>
#include <iostream>

const static std::string EMPTY_STR = "";

// TODO 还有很多的接口需要实现

struct VarData
{
public:

    VarData();
    
    ~VarData();

    bool SetInt(int value);

    bool SetInt64(int64_t value);

    bool SetStr(const std::string &str);

    bool SetFloat(float value);

    bool SetDouble(double value);

    int IntValue() const;

    int64_t Int64Value() const;

    float FloatValue() const;

    double DoubleValue() const;

    const std::string& StrValue() const;

    VAR_TYPE GetType() const;

    void TryDeleteStr();

private:

    union Data
    {
        int intValue;
        int64_t int64Value;
        float floatValue;
        double doubleValue;
        std::string *pStrValue;
    };


    Data m_Data;    
    VAR_TYPE m_Type;
    // TODO 这里使用c++11的新用法 没有使用c++17的方式
};

template <int N>
class TVarList : public IVarList
{
public:
    TVarList()
    {
        m_ValueUsed = 0;
    }

    virtual ~TVarList()
    {
        for (auto it : m_HeadValue)
        {
            delete &(*it);
        }
        m_HeadValue.clear();

        for (size_t i = 0; i < m_ValueUsed && i < N; i++)
        {
            m_StackValue[i].TryDeleteStr();
        }

        m_ValueUsed = 0;
    }

    virtual size_t GetCount() const
    {
        return m_ValueUsed;
    }

    virtual bool AddInt(int value)
    {
        VarData *p = GetNewVarData();
        return p->SetInt(value);
    }

    virtual bool AddInt64(int64_t value)
    {
        VarData *p = GetNewVarData();
        return p->SetInt64(value);
    }

    virtual bool AddStr(const std::string &str)
    {
        VarData *p = GetNewVarData();
        return p->SetStr(str);
    }

    virtual bool AddFloat(float value)
    {
        VarData *p = GetNewVarData();
        return p->SetFloat(value);
    }

    virtual bool AddDouble(double value)
    {
        VarData *p = GetNewVarData();
        return p->SetDouble(value);
    }

    virtual int IntValue(int index) const 
    {
        const VarData *p = GetVarData(index);
        return p ? p->IntValue() : 0;
    }

    virtual int64_t Int64Value(int index) const
    {
        const VarData *p = GetVarData(index);
        return p ? p->Int64Value() : 0;
    }

    virtual const std::string& StrValue(int index) const
    {
        const VarData *p = GetVarData(index);
        return p ? p->StrValue() : EMPTY_STR;
    }

    virtual float FloatValue(int index) const
    {
        const VarData *p = GetVarData(index);
        return p ? p->FloatValue() : 0.0f;
    }

    virtual double DoubleValue(int index) const 
    { 
        const VarData *p = GetVarData(index);
        return p ? p->DoubleValue() : 0.0;
    }

    virtual VAR_TYPE GetType(int index) const
    {
        const VarData *p = GetVarData(index);
        return p ? p->GetType() : VAR_TYPE_UNKNOWN;
    }

    virtual bool IsEmpty() const
    {
        return m_ValueUsed == 0;
    }

    virtual void Clear()
    {
        // TODO: 优化应该保留原先new出来的数据以及vector中的数据
        for (auto it : m_HeadValue)
        {
            delete &(*it);
        }
        m_HeadValue.clear();

        for (size_t i = 0; i < m_ValueUsed && i < N; i++)
        {
            m_StackValue[i].TryDeleteStr();
        }

        m_ValueUsed = 0;
    }

private:

    VarData* GetNewVarData()
    {
        if (m_ValueUsed < N)
        {// 本地堆栈未使用完
            return m_StackValue + m_ValueUsed++;
        }

        auto p = new VarData();
        m_HeadValue.push_back(p);
        ++ m_ValueUsed;

        return p;
    }

    const VarData* GetVarData(int index) const
    {
        if (index < 0 || (unsigned int)index >= m_ValueUsed)
        {
            return nullptr;
        }

        if (index < N)
        {
            return &(m_StackValue[index]);
        }

        return m_HeadValue[index - N];
    }

    unsigned int m_ValueUsed;
    VarData m_StackValue[N];
    std::vector<VarData*> m_HeadValue;
};

typedef TVarList<64> CVarList;
