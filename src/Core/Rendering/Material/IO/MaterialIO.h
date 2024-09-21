#pragma once

#include "Core/Rendering/Material/IO/IMaterialIO.h"
#include "Core/Rendering/Material/IMaterialFactory.h"
#include "Logging/IDwarfLogger.h"
#include "Utilities/FileHandler/IFileHandler.h"
#include <memory>

namespace Dwarf
{
  class MaterialIO : public IMaterialIO
  {
  private:
    std::shared_ptr<IMaterialFactory> m_MaterialFactory;
    std::shared_ptr<IDwarfLogger>     m_Logger;
    std::shared_ptr<IFileHandler>     m_FileHandler;

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