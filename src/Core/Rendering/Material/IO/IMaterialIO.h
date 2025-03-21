#pragma once

#include "Core/Rendering/Material/IMaterial.h"

namespace Dwarf
{
  /**
   * @brief Class that read and writes material files
   *
   */
  class IMaterialIO
  {
  public:
    virtual ~IMaterialIO() = default;

    /**
     * @brief Serializes and writes the material instance to a given path
     *
     * @param material Material to write
     * @param path Path to write to
     */
    virtual void
    SaveMaterial(IMaterial&                   material,
                 const std::filesystem::path& path) const = 0;

    /**
     * @brief Loads a material from a material file
     *
     * @param path Path to the material file
     * @return Unique pointer to the loaded material file
     */
    [[nodiscard]] virtual auto
    LoadMaterial(const std::filesystem::path& path) const
      -> std::unique_ptr<IMaterial> = 0;
  };
};