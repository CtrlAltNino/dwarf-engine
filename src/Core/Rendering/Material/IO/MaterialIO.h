#pragma once

#include "Core/Rendering/Material/IO/IMaterialIO.h"
#include "Core/Rendering/Material/IMaterialFactory.h"
#include "Logging/IDwarfLogger.h"
#include <memory>

namespace Dwarf
{
  class MaterialIO : public IMaterialIO
  {
  private:
    std::shared_ptr<IMaterialFactory> m_MaterialFactory;
    std::shared_ptr<IDwarfLogger>     m_Logger;

  public:
    // Constructor
    MaterialIO(std::shared_ptr<IMaterialFactory> materialFactory,
               std::shared_ptr<IDwarfLogger>     logger);

    // Destructor
    ~MaterialIO();

    // Implement the SaveMaterial method
    virtual void
    SaveMaterial(IMaterial&                   material,
                 const std::filesystem::path& path) override;

    virtual std::unique_ptr<IMaterial>
    LoadMaterial(const std::filesystem::path& path) override;
  };
}; // namespace Dwarf