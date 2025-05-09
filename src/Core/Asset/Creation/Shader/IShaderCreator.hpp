#pragma once

#include <filesystem>
#include <optional>

namespace Dwarf
{
  enum class ShaderType : uint8_t
  {
    VertexType,
    FragmentType
  };

  enum class ShaderSource : uint8_t
  {
    PbrSource,
    UnlitSource
  };

  class IShaderCreator
  {
  public:
    virtual ~IShaderCreator() = default;

    /**
     * @brief Create a new shader asset
     *
     * @param type Type of the shader
     * @param source Source of the shader
     * @param directoryPath Optional directory path
     */
    virtual void
    CreateShaderAsset(
      ShaderType                           type,
      ShaderSource                         source,
      std::optional<std::filesystem::path> directoryPath = std::nullopt) = 0;
  };
} // namespace Dwarf