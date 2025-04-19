#pragma once

#include "Core/Rendering/Material/IMaterialFactory.hpp"
#include "Core/Rendering/Material/IO/IMaterialIO.hpp"
#include "Logging/IDwarfLogger.hpp"
#include "Utilities/FileHandler/IFileHandler.hpp"
#include <memory>

namespace Dwarf
{
  class MaterialIO : public IMaterialIO
  {
  private:
    std::shared_ptr<IMaterialFactory> mMaterialFactory;
    std::shared_ptr<IDwarfLogger>     mLogger;
    std::shared_ptr<IFileHandler>     mFileHandler;

  public:
    // Constructor
    MaterialIO(std::shared_ptr<IMaterialFactory> materialFactory,
               std::shared_ptr<IDwarfLogger>     logger,
               std::shared_ptr<IFileHandler>     fileHandler);

    // Destructor
    ~MaterialIO();

    /**
     * @brief Serializes and writes the material instance to a given path
     *
     * @param material Material to write
     * @param path Path to write to
     */
    void
    SaveMaterial(IMaterial&                   material,
                 const std::filesystem::path& path) const override;

    /**
     * @brief Loads a material from a material file
     *
     * @param path Path to the material file
     * @return Unique pointer to the loaded material file
     */
    [[nodiscard]] auto
    LoadMaterial(const std::filesystem::path& path) const
      -> std::unique_ptr<IMaterial> override;
  };
}; // namespace Dwarf