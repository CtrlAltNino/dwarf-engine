#include "ShaderParameterCollection.h"

namespace Dwarf
{
  ShaderParameterCollection::ShaderParameterCollection() {}

  ShaderParameterCollection::~ShaderParameterCollection() {}

  void
  ShaderParameterCollection::SetParameter(std::string_view identifier,
                                          ParameterValue   parameter)
  {
    m_Parameters[identifier.data()] = parameter;
  }

  std::optional<ParameterValue>
  ShaderParameterCollection::GetParameter(std::string const& name) const
  {
    auto it = m_Parameters.find(name);
    if (it != m_Parameters.end())
    {
      return it->second;
    }
    return std::nullopt;
  }

  const std::vector<std::string>
  ShaderParameterCollection::GetParameterIdentifiers() const
  {
    std::vector<std::string> keys;
    keys.reserve(m_Parameters.size());
    for (const auto& pair : m_Parameters)
    {
      keys.push_back(pair.first);
    }
    return keys;
  }

  void
  ShaderParameterCollection::RemoveParameter(std::string const& name)
  {
    m_Parameters.erase(name);
  }

  bool
  ShaderParameterCollection::HasParameter(std::string const& name) const
  {
    return m_Parameters.find(name) != m_Parameters.end();
  }

  void
  ShaderParameterCollection::ClearParameters()
  {
    m_Parameters.clear();
  }
}