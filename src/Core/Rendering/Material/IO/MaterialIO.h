#pragma once

#include "Core/Rendering/Material/IMaterialFactory.h"
#include "Core/Rendering/Material/IO/IMaterialIO.h"
#include "Logging/IDwarfLogger.h"
#include "Utilities/FileHandler/IFileHandler.h"
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

    // Implement the SaveMaterial method
    virtual void
    SaveMaterial(IMaterial&                   material,
                 const std::filesystem::path& path) const override;

    virtual std::unique_ptr<IMaterial>
    LoadMaterial(const std::filesystem::path& path) const override;
  };
}; // namespace Dwarf