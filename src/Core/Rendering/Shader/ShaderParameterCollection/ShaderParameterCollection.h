#pragma once
#include "IShaderParameterCollection.h"

namespace Dwarf
{
  class ShaderParameterCollection : public IShaderParameterCollection
  {
  private:
    std::map<std::string, ParameterValue> mParameters;

  public:
    ShaderParameterCollection() = default;
    ~ShaderParameterCollection() override = default;

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
    [[nodiscard]] auto
    GetParameterIdentifiers() const -> const std::vector<std::string> override;

    /**
     * @brief Removes a parameter
     *
     * @param name Name of the parameters to remove
     */
    void
    RemoveParameter(const std::string& name) override;

    /**
     * @brief Checks if a parameter is present
     *
     * @param name Key to check for
     * @return true If the parameter is present
     * @return false If the parameter is not present
     */
    [[nodiscard]] auto
    HasParameter(const std::string& name) const -> bool override;

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
    auto
    Serialize() -> nlohmann::json override;
  };
}