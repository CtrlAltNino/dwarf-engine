#pragma once

#include "Core/Base.h"

class IShaderParameter
{
private:
    std::string m_Identifier;
    T m_Value;

public:
    IShaderParameter(std::string identifier, T value) : m_Identifier(identifier), m_Value(value)
    {
    }

    void SetValue(T value)
    {
        m_Value = value;
    }

    void SetIdentifier(std::string identifier)
    {
        m_Identifier = identifier;
    }
};