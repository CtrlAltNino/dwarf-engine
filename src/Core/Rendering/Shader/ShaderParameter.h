#pragma once
#include "IShaderParameter.h"

namespace Dwarf
{
  template<typename T>
  class ShaderParameter
  {
  public:
    ShaderParameter(T value);
    ~ShaderParameter();

    T
    GetValue() const;

    std::type_index
    GetType() const;

  private:
    T Value;
  };
}