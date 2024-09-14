#pragma once
#include "IShaderParameterCollection.h"

namespace Dwarf
{
  class ShaderParameterCollection : public IShaderParameterCollection
  {
  private:
    std::map<std::string, ParameterValue> m_Parameters;

  public:
    ShaderParameterCollection();
    ~ShaderParameterCollection() override;

    /**
     * @brief Sets a parameter to a direct Value.
     */
    void
    SetParameter(std::string_view identifier,
                 ParameterValue   parameter) override;

    /**
     * @brief Gets a parameter from the collection.
     */
    ParameterValue&
    GetParameter(const std::string& name) override;

    /**
     * @brief Gets the list of parameter identifiers.
     */

    const std::vector<std::string>
    GetParameterIdentifiers() const override;

    void
    RemoveParameter(const std::string& name) override;

    bool
    HasParameter(const std::string& name) const override;

    /**
     * @brief Clears the parameters.
     */
    void
    ClearParameters() override;

    /**
     * @brief Serialize the object.
     *
     * @return The serialized object.
     */

    nlohmann::json
    Serialize() const override;
  };
}