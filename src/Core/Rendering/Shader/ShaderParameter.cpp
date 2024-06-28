#include "ShaderParameter.h"

namespace Dwarf
{
  template<typename T>
  ShaderParameter<T>::ShaderParameter(T value)
    : Value(value)
  {
    Value = value;
  }

  template<typename T>
  ShaderParameter<T>::~ShaderParameter()
  {
  }

  template<typename T>
  T
  ShaderParameter<T>::GetValue() const
  {
    return Value;
  }

  template<typename T>
  std::type_index
  ShaderParameter<T>::GetType() const
  {
    return std::type_index(typeid(T));
  }
}