#pragma once

#include "Core/Rendering/Material/IO/IMaterialIO.h"
#include "Core/Rendering/Material/IMaterialFactory.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class MaterialIO : public IMaterialIO
  {
  public:
    // Constructor
    MaterialIO(std::shared_ptr<IMaterialFactory> materialFactory,
               std::shared_ptr<IDwarfLogger>     logger);

    // Destructor
    ~MaterialIO();

    // Implement the SaveMaterial method
    virtual void
    SaveMaterial(std::shared_ptr<IMaterial>   material,
                 std::filesystem::path const& path) override;

    virtual std::shared_ptr<IMaterial>
    LoadMaterial(std::filesystem::path const& path) override;

  private:
    std::shared_ptr<IMaterialFactory> m_MaterialFactory;
    std::shared_ptr<IDwarfLogger>     m_Logger;
  };
}; // namespace Dwarf